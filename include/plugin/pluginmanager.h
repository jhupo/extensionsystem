#ifndef __EXTENSION_PLUGIN_MANAGER_H__
#define __EXTENSION_PLUGIN_MANAGER_H__

#include <extension_global.h>

#include <boost/shared_ptr.hpp>

#include <list>
#include <vector>

namespace extension{

    namespace core{

        class Plugin;
        class PluginSpec;
        class PluginManagerPrivate;

        class EXTENSION_EXPORT PluginManager DECL_EQ_FINAL
        {
            DISABLE_COPY(PluginManager)
            DECLARE_PRIVATE(PluginManager)
        public:

            static PluginManager* inst();

            void addObject(const std::string& name, const void* object);
            void removeObject(const std::string& name);
            void removeObject(const void* object);

            std::list<std::pair<std::string,const void*> > allObjects()const;

            void setInterfaceIdentifier(const std::string& interfaceIdentifier);
            std::string interfaceIdentifier()const;

            void setPluginPaths(const std::list<std::string>& paths);
            std::list<std::string> pluginPaths()const;

            std::vector<Plugin*> plugins()const;
            std::vector<PluginSpec*> specs()const;

            Plugin* plugin(const std::string& name);
            PluginSpec* spec(const std::string& name);

            void loadPlugins();
            void loadPlugin(const std::string& name);
            void loadPlugin(PluginSpec* spec);

            void unloadPlugins();
            void unloadPlugin(const std::string& name);
            void unloadPlugin(PluginSpec* spec);
            
        private:
            PluginManager();
            virtual~PluginManager();
            const boost::shared_ptr<PluginManagerPrivate>       d_ptr;
        };



    }

}


#endif