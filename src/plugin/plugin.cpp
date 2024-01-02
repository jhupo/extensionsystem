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
            for(std::list<std::pair<std::string,std::pair<void*,ObjectSmartDeleter> > >::iterator iter = d->_addedObjectsInReverseOrder.begin();
                iter != d->_addedObjectsInReverseOrder.end();
                ++iter)
            {
                PluginManager::inst()->removeObject(iter->first);
                iter->second.second(iter->second.first);
            }
            d->_addedObjectsInReverseOrder.clear();
        }

        template<>
        void Plugin::addObject<ObjectSmartDeleter>(const std::string &name, void *obj, ObjectSmartDeleter& smart)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_addedObjectsInReverseOrder.push_back(std::make_pair(name,std::make_pair(obj,smart)));
            PluginManager::inst()->addObject(name,obj,smart);
        }

        template<>
        void Plugin::addAutoReleasedObject(const std::string &name, void *obj, ObjectSmartDeleter& smart)
        {
            D_P(Plugin);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_addedObjectsInReverseOrder.push_front(std::make_pair(name,std::make_pair(obj,smart)));
            PluginManager::inst()->addObject(name,obj,smart);
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

    }

}