#ifndef __EXTENSION_PLUGIN_MANAGER_H__
#define __EXTENSION_PLUGIN_MANAGER_H__

#include <extension_global.h>

#include <list>
#include <vector>
#include <string>

namespace extension{

    namespace core{

        struct ObjectSmartDeleter;
        class Plugin;
        class PluginSpec;
        class PluginManagerPrivate;

        class EXTENSION_EXPORT PluginManager final
        {
            DISABLE_COPY(PluginManager)
            DECLARE_PRIVATE(PluginManager)
        public:

            static PluginManager* inst();

            template<typename T = ObjectSmartDeleter>
            void addObject(const std::string& name, void* object, T& smart = ObjectSmartDeleter());
            void removeObject(const std::string& name);
            void removeObject(void* object);

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
            const std::shared_ptr<PluginManagerPrivate>       d_ptr;
        };



    }

}


#endif