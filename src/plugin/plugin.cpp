#include <plugin/plugin.h>  
#include <plugin/private/plugin_p.h>
#include <plugin/pluginmanager.h>


namespace extension{
    
    namespace core{

        PluginPrivate::PluginPrivate(Plugin *q)
            : q_ptr(q)
            , _spec(0)
        {

        }

        PluginPrivate::~PluginPrivate()
        {

        }

        Plugin::Plugin()
            : d_ptr(new PluginPrivate(this))
        {

        }

        Plugin::~Plugin()
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<std::pair<std::string,const void*> >::const_iterator iter = d->_addedObjectsInReverseOrder.begin();
                iter != d->_addedObjectsInReverseOrder.end();
                ++iter)
            {
                PluginManager::inst()->removeObject(iter->first);
                delete iter->second;
            }
            d->_addedObjectsInReverseOrder.clear();
        }

        void Plugin::addObject(const std::string &name, const void *obj)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            PluginManager::inst()->addObject(name,obj);
        }

        void Plugin::addAutoReleasedObject(const std::string &name, const void *obj)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_addedObjectsInReverseOrder.push_front(std::make_pair(name,obj));
            PluginManager::inst()->addObject(name,obj);
        }

        void Plugin::removeObject(void *obj)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            PluginManager::inst()->removeObject(obj);
        }

        void Plugin::removeObject(const std::string &name)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            PluginManager::inst()->removeObject(name);
        }

        PluginSpec *Plugin::pluginSpec() const
        {
            D_P(const Plugin);
            return d->_spec;
        }

        PLUGIN_METADATA_SECTION
        static const unsigned char _plugin_metadata[] = 
        {
            'P','L','U','G','I','N','_','M','E','T','A','D','A','T','A','_','S','E','C','T','I','O','N','\0'
        };

        
    }

}