#include <plugin/pluginspec.h>
#include <logger/logger.h>

#include <boost/filesystem.hpp>

namespace extension{

    namespace core{

        namespace{
            const char _plugin_metadata_section[] = "MetaData";
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
            bool readMetaData(const std::string& data);
            
            std::string read_section_as_string(const std::string& libraryPath, const std::string &sectionName);

            PluginSpec*const                q_ptr;
            bool                            _hasError;
            std::string                     _errorString;
            std::string                     _pluginName;
            std::string                     _version;
            std::string                     _description;
            std::string                     _url;
            std::string                     _metaData;
            std::string                     _location;
            bool                            _enabled;
            PluginSpec::State               _state;
            std::vector<PluginDependency>   _dependencys;
            Plugin*                         _plugin;
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
            _errorString = _metaData = _pluginName = _version = _description = _url = "";
            _plugin = DECL_EQ_NULLPTR;
            _state = PluginSpec::Invalid;
            _location = boost::filesystem::complete(path).string();
            if(_location.empty()){
                _hasError = true;
                _errorString = "Invalid path";
                LOG_WRN(extension.core.plugin) << "Invalid path: " << path;
                return false;
            }
            if(!readMetaData(read_section_as_string(_location,".metadata"))){
                _hasError = true;
                _errorString = "Invalid metadata";
                LOG_WRN(extension.core.plugin) << "Invalid metadata: " << _location;
                return false;
            }
            _state = PluginSpec::Read;
            LOG_INF(extension.core.plugin) << _pluginName <<" Read successful";
            return true;
        }

        bool PluginSpecPrivate::readMetaData(const std::string &data)
        {
            LOG_DBG(extension.core.plugin) << "Read metadata: " << data;
            return true;
        }
        
        std::string PluginSpecPrivate::read_section_as_string(const std::string &libraryPath, const std::string &sectionName)
        {
            return std::string();
        }
    }
}