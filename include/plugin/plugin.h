#ifndef __EXTENSION_PLUGIN_H__
#define __EXTENSION_PLUGIN_H__

#include <extension_global.h>


namespace extension{

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

        void addObject(void *obj);

        void addAutoReleasedObject(void *obj);

        void removeObject(void *obj);

        PluginSpec *pluginSpec() const;

    private:
        const std::shared_ptr<PluginPrivate> d_ptr;
    };

}

#endif