#ifndef __EXTENSION_PLUGIN_H__
#define __EXTENSION_PLUGIN_H__

#include <extension_global.h>


namespace extension{
    namespace core{

        struct ObjectSmartDeleter;
        class PluginSpec;
        class PluginPrivate;

        class EXTENSION_EXPORT Plugin
        {
            DISABLE_COPY(Plugin)
            DECLARE_PRIVATE(Plugin)
        public:

            enum ShutdownFlag{SynchronousShutdown, AsynchronousShutdown};

            Plugin();
            virtual~Plugin();

            virtual void initialize() = 0;

            virtual void extensionsInitialized() = 0;

            virtual bool delayedInitialize() { return false; }

            virtual ShutdownFlag aboutToShutdown() { return SynchronousShutdown; }

            template<typename T = ObjectSmartDeleter>
            void addObject(const std::string& name, void *obj, T& smart = ObjectSmartDeleter());
            template<typename T = ObjectSmartDeleter>
            void addAutoReleasedObject(const std::string& name, void *obj, T& smart);

            void removeObject(void *obj);
            void removeObject(const std::string& name);

            PluginSpec *pluginSpec() const;

        private:
            const std::shared_ptr<PluginPrivate> d_ptr;
        };


    }
}

#endif