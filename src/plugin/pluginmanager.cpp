#include <plugin/pluginmanager.h>


namespace extension{

    namespace core{

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

        void PluginManager::addObject(const std::string &name, const void *obj)
        {

        }

        void PluginManager::removeObject(const std::string &name)
        {

        }

        void PluginManager::removeObject(const void *obj)
        {

        }

    }
}