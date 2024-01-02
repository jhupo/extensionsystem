#ifndef __EXTENSION_PLUGIN_SPEC_H__
#define __EXTENSION_PLUGIN_SPEC_H__

#include <extension_global.h>

#include <string>
#include <vector>

namespace extension{

    namespace core{

        class Plugin;
        class PluginSpecPrivate;

        struct EXTENSION_EXPORT PluginDependency
        {
            enum Type{Required, Optional, Test};
            PluginDependency():type(Required){}

            bool operator==(const PluginDependency &other) const{return name == other.name;}

            static Type fromString(const std::string& str);

            Type            type;
            std::string     name;
        };

        class EXTENSION_EXPORT PluginSpec final
        {
            DISABLE_COPY(PluginSpec)
            DECLARE_PRIVATE(PluginSpec)
        public:

            enum State{ Invalid, Read, Resolved, Loaded, Initialized, Running, Stopped, Deleted };

            ~PluginSpec();

            bool hasError()const;
            std::string errorString()const;

            std::string pluginName()const;
            std::string version()const;
            std::string description()const;
            std::string url()const;
            bool enabled()const;

            State state()const;

            std::vector<PluginDependency> dependencys()const;

            Plugin* plugin()const;

        private:
            const std::shared_ptr<PluginSpecPrivate>          d_ptr;
        };


    }

}

#endif