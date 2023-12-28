#ifndef __EXTENSION_PLUGIN_H__
#define __EXTENSION_PLUGIN_H__

#include <extension_global.h>

#include <boost/shared_ptr.hpp>

namespace extension{
    namespace core{

        class PluginSpec;
        class PluginPrivate;

        class Plugin
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

            void addObject(const std::string& name, const void *obj);
            void addAutoReleasedObject(const std::string& name, const void *obj);

            void removeObject(void *obj);
            void removeObject(const std::string& name);

            PluginSpec *pluginSpec() const;

        private:
            const boost::shared_ptr<PluginPrivate> d_ptr;
        };


    }
}

#endif