#include <plugin/pluginloader.h>
#include <logger/logger.h>

#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>

#include <json/json.h>

#if defined(__unix__)
#include <dlfcn.h>
#elif defined(WIN32)
#include <windows.h>
#endif

namespace extension{

    namespace core{

        namespace{
            const char _plugin_metadata_section[] = ".metadata";
            const char _plugin_metadata_section_pattern[] = "METADATA ";
        }

        namespace{
            typedef void (*FunctionPointer)();
            long plugin_find_pattern(const char* s, ulong s_len, const char* pattern, ulong p_len)
            {
                if (! s || ! pattern || p_len > s_len) return -1;
                ulong i, hs = 0, hp = 0, delta = s_len - p_len;
                for (i = 0; i < p_len; ++i) {
                    hs += s[delta + i];
                    hp += pattern[i];
                }
                i = delta;
                for (;;) {
                    if (hs == hp && strncmp(s + i, pattern, p_len) == 0)
                        return i;
                    if (i == 0)
                        break;
                    --i;
                    hs -= s[i + p_len];
                    hs += s[i];
                }
                return -1;
            }
            bool findPatternUnloaded(const std::string& library,Json::Value& json)
            {
                FILE* fp = fopen(library.c_str(),"rb");
                if(!fp){
                    LOG_ERR(extension.pluginloader) << "Failed to open file: " << library;
                    return false;
                }
                fseek(fp,0,SEEK_END);
                long size = ftell(fp);
                fseek(fp,0,SEEK_SET);
                char* buffer = (char*)malloc(size);
                if(!buffer){
                    LOG_ERR(extension.pluginloader) << "Failed to allocate memory for file: " << library;
                    fclose(fp);
                    return false;
                }
                if(fread(buffer,1,size,fp) != size){
                    LOG_ERR(extension.pluginloader) << "Failed to read file: " << library;
                    fclose(fp);
                    free(buffer);
                    return false;
                }
                fclose(fp);
                long pos = plugin_find_pattern(buffer,size,_plugin_metadata_section_pattern,sizeof(_plugin_metadata_section_pattern) - 1);
                if(pos < 0){
                    LOG_ERR(extension.pluginloader) << "Failed to find pattern: " << _plugin_metadata_section_pattern << " in file: " << library;
                    free(buffer);
                    return false;
                }
                std::string json_str = std::string(buffer + pos + sizeof(_plugin_metadata_section_pattern) - 1);
                free(buffer);
                Json::Reader reader;
                if(!reader.parse(json_str,json)){
                    LOG_ERR(extension.pluginloader) << "Failed to parse json string: " << json_str;
                    return false;
                }
                return true;
            }
        }


        class PluginLoaderPrivate
        {
            DECLARE_PUBLIC(PluginLoader)
        public:
            PluginLoaderPrivate(PluginLoader* q);
            ~PluginLoaderPrivate();

            bool updateMetaData();

            void* load_sys();
            bool unload_sys();

            FunctionPointer resolve(const char* symbol);

            PluginLoader*const                  q_ptr;
            mutable boost::mutex                _mutext;
            bool                                _hasError;
            std::string                         _errorString;
            std::string                         _fileName;
            void*                               _instance;
            Json::Value                         _metaData;
        };

        PluginLoaderPrivate::PluginLoaderPrivate(PluginLoader *q)
            : q_ptr(q)
            , _hasError(false)
            , _instance(nullptr)
        {

        }

        PluginLoaderPrivate::~PluginLoaderPrivate()
        {
            
        }

        bool PluginLoaderPrivate::updateMetaData()
        {
            return findPatternUnloaded(_fileName,_metaData);
        }

        void *PluginLoaderPrivate::load_sys()
        {
            void* result = nullptr;
#if defined(__unix__)
            result = dlopen(_fileName.c_str(),RTLD_NOW);
#elif defined(_WIN32)
#endif
            return result;
        }

        bool PluginLoaderPrivate::unload_sys()
        {
#if defined(__unix__)
            return dlclose(_instance);
#elif defined(_WIN32)
#endif
            return false;
        }

        PluginLoader::PluginLoader()
            : d_ptr(new PluginLoaderPrivate(this))
        {

        }

        PluginLoader::PluginLoader(const std::string &fileName)
            : d_ptr(new PluginLoaderPrivate(this))
        {
            setFileName(fileName);
        }

        PluginLoader::~PluginLoader()
        {

        }

        void PluginLoader::setFileName(const std::string &fileName)
        {
            D_P(PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            d->_fileName = boost::filesystem::absolute(fileName).string();
            if(d->_fileName.empty()){
                d->_hasError = true;
                d->_errorString = "Invalid file name: " + fileName;
                LOG_ERR(extension.pluginloader) << "Invalid file name: " << fileName;
                return;
            }
            if(!boost::filesystem::exists(d->_fileName) && !boost::filesystem::is_regular_file(d->_fileName)){
                d->_hasError = true;
                d->_errorString = "File not found: " + fileName;
                LOG_ERR(extension.pluginloader) << "File not found: " << fileName;
                return;
            }
            d->updateMetaData();
        }

        std::string PluginLoader::fileName() const
        {
            D_P(const PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            return d->_fileName;
        }

        bool PluginLoader::load()
        {
            D_P(PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            if(d->_instance){
                d->_hasError = true;
                d->_errorString = "Plugin already loaded: " + d->_fileName;
                LOG_ERR(extension.pluginloader) << "Plugin already loaded: " << d->_fileName;
                return false;
            }
            if(d->_fileName.empty()){
                d->_hasError = true;
                d->_errorString = "Invalid file name: " + d->_fileName;
                LOG_ERR(extension.pluginloader) << "Invalid file name: " << d->_fileName;
                return false;
            }
            if(!boost::filesystem::exists(d->_fileName) && !boost::filesystem::is_regular_file(d->_fileName)){
                d->_hasError = true;
                d->_errorString = "File not found: " + d->_fileName;
                LOG_ERR(extension.pluginloader) << "File not found: " << d->_fileName;
                return false;
            }
            d->_instance = d->load_sys();
            if(!d->_instance){
                d->_hasError = true;
                d->_errorString = "Failed to load plugin: " + d->_fileName;
                LOG_ERR(extension.pluginloader) << "Failed to load plugin: " << d->_fileName;
                return false;
            }
            return true;
        }

        bool PluginLoader::unload()
        {
            D_P(PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            if(!d->_instance){
                d->_hasError = true;
                d->_errorString = "Plugin not loaded: " + d->_fileName;
                LOG_ERR(extension.pluginloader) << "Plugin not loaded: " << d->_fileName;
                return false;
            }
            return d->unload_sys();
        }

        bool PluginLoader::isLoad() const
        {
            D_P(const PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            return d->_instance != nullptr;
        }

        bool PluginLoader::hasError() const
        {
            D_P(const PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            return d->_hasError;
        }

        std::string PluginLoader::errorString() const
        {
            D_P(const PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            return d->_errorString;
        }

        Json::Value PluginLoader::metaData() const
        {
            D_P(const PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            return d->_metaData;
        }

        FunctionPointer PluginLoaderPrivate::resolve(const char *symbol)
        {
#if defined(__unix__)
            FunctionPointer address = FunctionPointer(dlsym(_instance,symbol));
#elif defined(_WIN32)
            FARPROC address = GetProcAddress(pHnd, symbol);
#endif
            if(!address){
                _hasError = true;
                _errorString = "Failed to resolve symbol: " + std::string(symbol);
                LOG_ERR(extension.pluginloader) << "Failed to resolve symbol: " << symbol;
                return nullptr;
            }
#if defined(_WIN32)
            return FunctionPointer(address);
#endif
            return address;
        }


    }

}