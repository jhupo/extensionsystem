#ifndef __EVENT_H__
#define __EVENT_H__

#include <extension_global.h>
#include <basicvariant/basicvariant.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace extension{

    namespace core{

        class EventPrivate;

        class EXTENSION_EXPORT Event : public boost::enable_shared_from_this<Event>
        {
            DECLARE_PRIVATE(Event)
            DECLARE_SHARED_PTR(Event)
        public:
            Event(const int64_t& id, const BasicVariant& data);
            virtual~Event();

            void setBasicVariant(const BasicVariant& data);
            BasicVariant basicVariant()const;

            void setProperty(const std::string& key, const BasicVariant& value);
            BasicVariant property(const std::string& key)const;

            void setEventId(const int64_t& id);
            int64_t eventId()const;

            boost::shared_ptr<Event const> clone()const;

        private:
            const boost::shared_ptr<EventPrivate> d_ptr;
        };

    }


}


#endif