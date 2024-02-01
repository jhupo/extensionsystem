#include <plugin/pluginmanager.h>
#include <plugin/private/plugin_p.h>

namespace extension{

    class PluginManagerPrivate
    {
        DECLARE_PUBLIC(PluginManager)
    public:
        PluginManagerPrivate(PluginManager* q);
        ~PluginManagerPrivate();


        PluginManager*const q_ptr;
    };

    PluginManagerPrivate::PluginManagerPrivate(PluginManager *q)
        : q_ptr(q)
    {

    }

    PluginManagerPrivate::~PluginManagerPrivate()
    {

    }

    PluginManager::PluginManager()
        : d_ptr(new PluginManagerPrivate(this))
    {

    }

    PluginManager::~PluginManager()
    {

    }

    PluginManager* PluginManager::inst()
    {
        static PluginManager _inst_;
        return &_inst_;
    }

    void PluginManager::addObject(void *obj)
    {

    }

    void PluginManager::removeObject(void *obj)
    {

    }

    std::string PluginManager::interfaceIdentifier() const
    {
        return std::string();
    }

}