#include <plugin/pluginloader.h>
#include <logger/logger.h>

#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>

#include <json/json.h>

namespace extension{

    namespace core{

        namespace{
            const char _plugin_metadata_section[] = ".metadata";
        }

        class PluginLoaderPrivate
        {
            DECLARE_PUBLIC(PluginLoader)
        public:
            PluginLoaderPrivate(PluginLoader* q);
            ~PluginLoaderPrivate();

            bool loadPlugin();
            bool unloadPlugin();
            bool updateMetaData();

            void* resolve(const char* symbol);

            PluginLoader*const                  q_ptr;
            mutable boost::mutex                _mutext;
            bool                                _hasError;
            std::string                         _errorString;
            std::string                         _fileName;
            void*                               _instance;
            Json::Value                         _metaData;
        };

        PluginLoaderPrivate::PluginLoaderPrivate(PluginLoader *q)
            : q_ptr(q)
            , _hasError(false)
            , _instance(nullptr)
        {

        }

        PluginLoaderPrivate::~PluginLoaderPrivate()
        {
            
        }

        bool PluginLoaderPrivate::updateMetaData()
        {
            
            return false;
        }

        PluginLoader::PluginLoader()
            : d_ptr(new PluginLoaderPrivate(this))
        {

        }

        PluginLoader::PluginLoader(const std::string &fileName)
            : d_ptr(new PluginLoaderPrivate(this))
        {
            setFileName(fileName);
        }

        PluginLoader::~PluginLoader()
        {

        }

        void PluginLoader::setFileName(const std::string &fileName)
        {
            D_P(PluginLoader);
            boost::mutex::scoped_lock lock(d->_mutext);
            d->_fileName = boost::filesystem::absolute(fileName).string();
            if(d->_fileName.empty()){
                LOG_ERR(extension.pluginloader) << "Invalid file name: " << fileName;
                return;
            }
            if(!boost::filesystem::exists(d->_fileName) && !boost::filesystem::is_regular_file(d->_fileName)){
                LOG_ERR(extension.pluginloader) << "File not found: " << fileName;
                return;
            }
            d->updateMetaData();
        }



    }

}