#include <plugin/pluginspec.h>


namespace extension{

    namespace core{

        class PluginSpecPrivate
        {
            DECLARE_PUBLIC(PluginSpec)
        public:

            PluginSpecPrivate(PluginSpec* q);
            ~PluginSpecPrivate();


            PluginSpec*const                q_ptr;
            bool                            _hasError;
            std::string                     _errorString;
            std::string                     _pluginName;
            std::string                     _version;
            std::string                     _description;
            std::string                     _url;
            bool                            _enabled;
            PluginSpec::State               _state;
            std::vector<PluginDependency>   _dependencys;
            Plugin*                         _plugin;
        };


    }

}