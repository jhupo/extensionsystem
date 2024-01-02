#ifndef __EVENT_DISPATCH_H__
#define __EVENT_DISPATCH_H__

#include <extension_global.h>
#include <basicvariant/basicvariant.h>

#include <functional>

namespace extension{

    namespace core{

        class Event;
        class EventDispatchPrivate;

        class EXTENSION_EXPORT EventDispatch final
        {
            DISABLE_COPY(EventDispatch)
            DECLARE_PRIVATE(EventDispatch)
            DECLARE_SHARED_PTR(EventDispatch)
        public:

            enum EventPriority{HighEventPriority,NormalEventPriority,LowEventPriority};
            enum ConnectionType{UniqueConnection,QueuedConnection};

            typedef std::function<void(const Event& evnt)> EventCallback;

            static EventDispatch* inst();

            void publishEvent(int64_t id, const BasicVariant& var, const ConnectionType& priority = QueuedConnection);

            void registerEvent(int64_t id, const void* object, const EventCallback& call, const EventPriority& priority = NormalEventPriority);
            void unRegisterEvent(int64_t id, const void* object);

        private:
            EventDispatch();
            virtual~EventDispatch();
            const std::shared_ptr<EventDispatchPrivate>       d_ptr;
        };


    }


}



#endif