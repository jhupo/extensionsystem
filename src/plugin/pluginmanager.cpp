#include <plugin/pluginmanager.h>
#include <plugin/private/plugin_p.h>

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

        template<>
        void PluginManager::addObject<ObjectSmartDeleter>(const std::string &name, void *obj, ObjectSmartDeleter& smart)
        {

        }

        void PluginManager::removeObject(const std::string &name)
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
}