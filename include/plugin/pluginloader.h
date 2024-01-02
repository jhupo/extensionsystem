#ifndef __EXTENSION_PLUGIN_LOADER_H__
#define __EXTENSION_PLUGIN_LOADER_H__

#include <extension_global.h>

#include <string>

namespace Json{
    class Value;
}

namespace extension{

    namespace core{

        class PluginLoaderPrivate;

        class EXTENSION_EXPORT PluginLoader final
        {
            DISABLE_COPY(PluginLoader)
            DECLARE_PRIVATE(PluginLoader)
            DECLARE_SHARED_PTR(PluginLoader)
        public:

            PluginLoader();
            PluginLoader(const std::string& fileName);
            ~PluginLoader();

            template<typename T>
            T* instance(){
                return static_cast<T>(_plugin_loader_instance());
            }

            Json::Value metaData()const;

            bool load();
            bool unload();
            bool isLoad()const;

            void setFileName(const std::string& fileName);
            std::string fileName()const;

            bool hasError()const;
            std::string errorString()const;

        private:
            void* _plugin_loader_instance()const;
            const std::shared_ptr<PluginLoaderPrivate>  d_ptr;
        };


    }

}


#endif