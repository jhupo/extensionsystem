#ifndef __EXTENSION_PLUGIN_P_H__
#define __EXTENSION_PLUGIN_P_H__

#include <extension_global.h>

#include <list>
#include <string>

#include <boost/thread/mutex.hpp>

namespace extension{

    class Plugin;
    class PluginSpec;

    class PluginPrivate
    {
        DECLARE_PUBLIC(Plugin)
    public:
        PluginPrivate(Plugin* q);
        ~PluginPrivate();
        
        Plugin*const                  q_ptr;
        boost::mutex                  _mutex;
        PluginSpec*                   _spec;
        std::list<void*>              _addedObjectsInReverseOrder;
    };

}


#endif