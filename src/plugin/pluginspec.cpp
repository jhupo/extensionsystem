#include <plugin/pluginspec.h>
#include <plugin/plugin.h>
#include <plugin/pluginmanager.h>

#include <json/json.h>

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
            std::shared_ptr<Plugin>         _plugin;
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
            _plugin = nullptr;
            _state = PluginSpec::Invalid;
            _location = "";//boost::filesystem::absolute(path).string();
            if(_location.empty()){
                _hasError = true;
                _errorString = "Invalid path";
                // LOG_WRN(extension.plugin) << "Invalid path: " << path;
                return false;
            }
            _metaData = Json::Value(GetCustomSectionContent(_location,_plugin_metadata_section).c_str());
            if(!readMetaData(_metaData)){
                _hasError = true;
                _errorString = "Invalid metadata";
                // LOG_WRN(extension.plugin) << "Invalid metadata: " << _location;
                return false;
            }
            _state = PluginSpec::Read;
            // LOG_INF(extension.plugin) << _pluginName <<" Read successful";
            return true;
        }

        bool PluginSpecPrivate::readMetaData(const Json::Value &json)
        {
            // LOG_DBG(extension.plugin) << "Read metadata: " << json;
            if(!json.isObject()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(!json.isMember(_plugin_metadata_iid) || !json[_plugin_metadata_iid].isString()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(json[_plugin_metadata_iid].asString() != PluginManager::inst()->interfaceIdentifier()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            if(!json.isMember(_plugin_metadata_name) || !json[_plugin_metadata_name].isString()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _pluginName = json[_plugin_metadata_name].asString();

            if(!json.isMember(_plugin_metadata_version) || !json[_plugin_metadata_version].isString()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _version = json[_plugin_metadata_version].asString();

            if(!json.isMember(_plugin_metadata_description) || !json[_plugin_metadata_description].isString()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _description = json[_plugin_metadata_description].asString();

            if(!json.isMember(_plugin_metadata_url) || !json[_plugin_metadata_url].isString()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _url = json[_plugin_metadata_url].asString();

            if(!json.isMember(_plugin_metadata_enabled) || !json[_plugin_metadata_enabled].isBool()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            _enabled = json[_plugin_metadata_enabled].asBool();

            if(!json.isMember(_plugin_metadata_dependency) || !json[_plugin_metadata_dependency].isArray()){
                // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                return false;
            }
            for(Json::Value::const_iterator iter = json[_plugin_metadata_dependency].begin();
                iter != json[_plugin_metadata_dependency].end();
                ++iter)
            {
                if(!iter->isObject()){
                    // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
                    return false;
                }
                if(!iter->isMember("Name") || !iter->isMember("Type")){
                    // LOG_ERR(extension.plugin) << "Invalid metadata: " << json;
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
                // LOG_ERR(extension.core.plugin) << "Invalid plugin: " << _location;
                return false;
            }
            if(PluginSpec::Resolved != _state){
                if(PluginSpec::Loaded == _state){
                    return true;
                }
                // LOG_ERR(extension.plugin) << "Invalid plugin state: " << _state;
                return false;
            }

            try{
                // _plugin = boost::dll::import_symbol<Plugin*>(_location,"Plugin");
            }catch(...){
                // LOG_ERR(extension.plugin) << "Load library failed: " << _location << " " << e.what();
                return false;
            }
            return false;
        }

        std::string PluginSpecPrivate::GetCustomSectionContent(const std::string &libraryPath, const std::string &sectionName)
        {
            std::string result;
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

