#ifndef __EXTENSION_CONSTANTS_H__
#define __EXTENSION_CONSTANTS_H__


#include <iostream>


namespace extension{

    namespace core{

        enum EventConstants
        {
            PluginStartedEvent = 0x0001LL,
            pluginsChanged,
            initializationDone,
            AsynchronousShutdownFinishedEvent,
            PluginFinishedEvent,
        };



    }

}


#endif