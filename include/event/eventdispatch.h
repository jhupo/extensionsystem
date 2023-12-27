#ifndef __EVENT_DISPATCH_H__
#define __EVENT_DISPATCH_H__


#include <extension_global.h>
#include <event/event.h>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace extension{

    namespace core{

        class EventDispatchPrivate;

        class EXTENSION_EXPORT EventDispatch DECL_EQ_FINAL
        {
            DECLARE_PRIVATE(EventDispatch)
            DECLARE_SHARED_PTR(EventDispatch)
        public:

            enum EventPriority{HighEventPriority,NormalEventPriority,LowEventPriority};
            enum ConnectionType{UniqueConnection,QueuedConnection};

            typedef boost::function<bool(const Event& evnt)> EventCallback;

            static EventDispatch* inst();

            void publishEvent(const int64_t& id, const Event& var, const ConnectionType& priority = QueuedConnection);

            void registerEvent(const int64_t& id, const EventCallback& call, const EventPriority& priority = NormalEventPriority);
            void unRegisterEvent(const int64_t& id);

            static void sendPostedEvents(const int64_t& id, const Event& var);

        private:
            EventDispatch();
            virtual~EventDispatch();
            const boost::shared_ptr<EventDispatchPrivate>       d_ptr;
        };


    }


}



#endif