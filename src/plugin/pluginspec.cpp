#include <plugin/pluginspec.h>
#include <logger/logger.h>
#include <json/json.h>
#include <plugin/plugin.h>
#include <plugin/pluginmanager.h>

#include <boost/filesystem.hpp>
#include <boost/dll.hpp>

#if defined(__unix__) 
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#elif defined(__APPLE__)
#include <dlfcn.h>
#include <mach-o/getsect.h>
#elif defined(_WIN32)
#include <windows.h>
#include "pluginspec.h"
#endif

namespace extension{

    namespace core{

        namespace{
            const char _plugin_metadata_section[] = ".metadata";
            const char _plugin_metadata_iid[] = "IID";
            const char _plugin_metadata_name[] = "Name";
            const char _plugin_metadata_version[] = "Version";
            const char _plugin_metadata_description[] = "Description";
            const char _plugin_metadata_url[] = "Url";
            const char _plugin_metadata_enabled[] = "Enabled";
            const char _plugin_metadata_dependency[] = "Dependency";
        }

        class PluginSpecPrivate
        {
            DECLARE_PUBLIC(PluginSpec)
        public:

            PluginSpecPrivate(PluginSpec* q);
            ~PluginSpecPrivate();

            bool read(const std::string& path);
            bool readMetaData(const Json::Value& json);

            bool loadLibrary();
            bool initializePlugin();
            bool initializeExtensions();
            bool delayedInitialize();
            Plugin::ShutdownFlag stop();
            void kill();
            
            std::string GetCustomSectionContent(const std::string& libraryPath, const std::string &sectionName);

            PluginSpec*const                q_ptr;
            bool                            _hasError;
            std::string                     _errorString;
            std::string                     _pluginName;
            std::string                     _version;
            std::string                     _description;
            std::string                     _url;
            Json::Value                     _metaData;
            std::string                     _location;
            bool                            _enabled;
            PluginSpec::State               _state;
            std::vector<PluginDependency>   _dependencys;
            boost::shared_ptr<Plugin>       _plugin;
        };

        PluginSpecPrivate::PluginSpecPrivate(PluginSpec *q)
            : q_ptr(q)
        {

        }

        PluginSpecPrivate::~PluginSpecPrivate()
        {
            
        }

        bool PluginSpecPrivate::read(const std::string &path)
        {
            _hasError = false;
            _errorString = _pluginName = _version = _description = _url = "";
            _plugin = DECL_EQ_NULLPTR;
            _state = PluginSpec::Invalid;
            _location = boost::filesystem::complete(path).string();
            if(_location.empty()){
                _hasError = true;
                _errorString = "Invalid path";
                LOG_WRN(extension.core.plugin) << "Invalid path: " << path;
                return false;
            }
            _metaData = Json::Value(GetCustomSectionContent(_location,_plugin_metadata_section));
            if(!readMetaData(_metaData)){
                _hasError = true;
                _errorString = "Invalid metadata";
                LOG_WRN(extension.core.plugin) << "Invalid metadata: " << _location;
                return false;
            }
            _state = PluginSpec::Read;
            LOG_INF(extension.core.plugin) << _pluginName <<" Read successful";
            return true;
        }

        bool PluginSpecPrivate::readMetaData(const Json::Value &json)
        {
            LOG_DBG(extension.core.plugin) << "Read metadata: " << json;
            if(!json.isObject()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(!json.isMember(_plugin_metadata_iid) || !json[_plugin_metadata_iid].isString()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(json[_plugin_metadata_iid].asString() != PluginManager::inst()->interfaceIdentifier()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(!json.isMember(_plugin_metadata_name) || !json[_plugin_metadata_name].isString()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _pluginName = json[_plugin_metadata_name].asString();

            if(!json.isMember(_plugin_metadata_version) || !json[_plugin_metadata_version].isString()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _version = json[_plugin_metadata_version].asString();

            if(!json.isMember(_plugin_metadata_description) || !json[_plugin_metadata_description].isString()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _description = json[_plugin_metadata_description].asString();

            if(!json.isMember(_plugin_metadata_url) || !json[_plugin_metadata_url].isString()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _url = json[_plugin_metadata_url].asString();

            if(!json.isMember(_plugin_metadata_enabled) || !json[_plugin_metadata_enabled].isBool()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _enabled = json[_plugin_metadata_enabled].asBool();

            if(!json.isMember(_plugin_metadata_dependency) || !json[_plugin_metadata_dependency].isArray()){
                LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                return false;
            }
            for(Json::Value::const_iterator iter = json[_plugin_metadata_dependency].begin();
                iter != json[_plugin_metadata_dependency].end();
                ++iter)
            {
                if(!iter->isObject()){
                    LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                    return false;
                }
                if(!iter->isMember("Name") || !iter->isMember("Type")){
                    LOG_ERR(extension.core.plugin) << "Invalid metadata: " << json;
                    return false;
                }
                PluginDependency dependency;
                dependency.name = (*iter)["Name"].asString();
                dependency.type = PluginDependency::fromString((*iter)["Type"].asString());
                _dependencys.push_back(dependency);
            }
            return true;
        }

        bool PluginSpecPrivate::loadLibrary()
        {
            if(_hasError){
                LOG_ERR(extension.core.plugin) << "Invalid plugin: " << _location;
                return false;
            }
            if(PluginSpec::Resolved != _state){
                if(PluginSpec::Loaded == _state){
                    return true;
                }
                LOG_ERR(extension.core.plugin) << "Invalid plugin state: " << _state;
                return false;
            }

            try{
                _plugin = boost::dll::import<Plugin*>(_location,"Plugin");
            }catch(const boost::system::system_error& e){
                LOG_ERR(extension.core.plugin) << "Load library failed: " << _location << " " << e.what();
                return false;
            }
            return false;
        }

        std::string PluginSpecPrivate::GetCustomSectionContent(const std::string &libraryPath, const std::string &sectionName)
        {
            const char* section_start = DECL_EQ_NULLPTR;
            size_t section_size = 0;
            std::string result;
#if defined(__unix__)
            int fd;
            struct stat sb;
            char *mapped, *section_str = NULL;
            Elf64_Ehdr *header;
            Elf64_Shdr *sections, *sh_strtab;
            if ((fd = open(libraryPath.c_str(), O_RDONLY)) < 0){
                LOG_ERR(extension.core.plugin) << "open failed: " << libraryPath;
                return result;
            }
            if (fstat(fd, &sb) == -1){
                LOG_ERR(extension.core.plugin) << "fstat failed: " << libraryPath;
                close(fd);
                return result;
            }
            mapped = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
            if (mapped == MAP_FAILED){
                LOG_ERR(extension.core.plugin) << "mmap failed: " << libraryPath;
                close(fd);
                return result;
            }
            header = (Elf64_Ehdr *)mapped;
            sections = (Elf64_Shdr *)(mapped + header->e_shoff);
            sh_strtab = &sections[header->e_shstrndx];
            const char *const sh_strtab_p = mapped + sh_strtab->sh_offset;
             for (int i = 0; i < header->e_shnum; ++i){
                if (strcmp(sh_strtab_p + sections[i].sh_name, sectionName.c_str()) == 0){
                    section_str = (char*)malloc(sections[i].sh_size + 1);
                    if(!section_str){
                        LOG_ERR(extension.core.plugin) << "malloc failed: " << libraryPath;
                        break;
                    }
                    memcpy(section_str, mapped + sections[i].sh_offset, sections[i].sh_size);
                    section_str[sections[i].sh_size] = '\0';
                    break;
                }
             }
             munmap(mapped, sb.st_size);
             close(fd);
             result = std::string(section_str);
#elif defined(__APPLE__)
            unsigned long size;
            const char* section_start = getsectdatafromFramework(libraryPath.c_str(), "__TEXT", sectionName.c_str(), &size);
            size_t section_size = static_cast<size_t>(size);
            result = std::string(section_start,section_size);
#elif defined(_WIN32)
            HMODULE hModule = LoadLibraryEx(libraryPath.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
            if(!hModule){
                LOG_ERR(extension.core.plugin) << "LoadLibraryEx failed: " << libraryPath;
                return result;
            }
            HRSRC hResInfo = FindResource(hModule, MAKEINTRESOURCE(sectionName.c_str()), RT_RCDATA);
            HGLOBAL hResData = LoadResource(hModule, hResInfo);
            section_start = static_cast<const char*>(LockResource(hResData));
            section_size = SizeofResource(hModule, hResInfo);
            FreeLibrary(hModule);
            result = std::string(section_start,section_size);
#else
#error "Unsupported platform"
#endif
            return result;
        }

        
        PluginDependency::Type PluginDependency::fromString(const std::string &str)
        {
            if(str == "Required"){
                return PluginDependency::Required;
            }else if(str == "Optional"){
                return PluginDependency::Optional;
            }else if(str == "Test"){
                return PluginDependency::Test;
            }
            return PluginDependency::Required;
        }

    }
}

