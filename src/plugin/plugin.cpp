#include <plugin/plugin.h>  
#include <plugin/private/plugin_p.h>
#include <plugin/pluginmanager.h>


namespace extension{

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
        for(std::list<void*>::iterator iter = d->_addedObjectsInReverseOrder.begin();
            iter != d->_addedObjectsInReverseOrder.end();
            ++iter)
        {
            PluginManager::inst()->removeObject(*iter);
        }
        d->_addedObjectsInReverseOrder.clear();
    }

    void Plugin::addObject(void *obj)
    {
        D_P(Plugin);
        boost::mutex::scoped_lock lock(d->_mutex);
        d->_addedObjectsInReverseOrder.push_back(obj);
        PluginManager::inst()->addObject(obj);
    }

    void Plugin::addAutoReleasedObject(void *obj)
    {
        D_P(Plugin);
        boost::mutex::scoped_lock lock(d->_mutex);
        d->_addedObjectsInReverseOrder.push_front(obj);
        PluginManager::inst()->addObject(obj);
    }

    void Plugin::removeObject(void *obj)
    {
        D_P(Plugin);
        boost::mutex::scoped_lock lock(d->_mutex);
        PluginManager::inst()->removeObject(obj);
    }

    PluginSpec *Plugin::pluginSpec() const
    {
        D_P(const Plugin);
        return d->_spec;
    }


}