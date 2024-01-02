#ifndef __EVENT_H__
#define __EVENT_H__

#include <extension_global.h>
#include <basicvariant/basicvariant.h>

namespace extension{

    namespace core{

        class EventPrivate;

        class EXTENSION_EXPORT Event : public basic_variant_format_t
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

        private:
            const std::shared_ptr<EventPrivate> d_ptr;
        };

    }


}


#endif