#include <event/event.h>

#include <map>

#include <boost/thread/mutex.hpp>

namespace extension{

    namespace core{

        class EventPrivate
        {
            DECLARE_PUBLIC(Event)
        public:
            EventPrivate(Event* ptr, const int64_t& id, const BasicVariant& data);
            ~EventPrivate();

            Event*const                         q_ptr;
            int64_t                             _event_id;
            BasicVariant                        _data;
            std::map<std::string,BasicVariant>  _properties;
            mutable boost::mutex                _mutex;
        };

        EventPrivate::EventPrivate(Event *ptr, const int64_t &id, const BasicVariant &data)
            : q_ptr(ptr)
            , _event_id(id)
            , _data(data)
        {
            
        }

        EventPrivate::~EventPrivate()
        {

        }

    
        Event::Event(const int64_t &id, const BasicVariant &data)
            : basic_variant_format_t(id)
            , d_ptr(new EventPrivate(this,id,data))
        {

        }

        Event::~Event()
        {

        }

        void Event::setBasicVariant(const BasicVariant &data)
        {
            D_P(Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_data = data;
        }

        BasicVariant Event::basicVariant() const
        {
            D_P(const Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_data;
        }

        void Event::setProperty(const std::string &key, const BasicVariant &value)
        {
            D_P(Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_properties[key] = value;
        }

        BasicVariant Event::property(const std::string &key) const
        {
            D_P(const Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            std::map<std::string,BasicVariant>::const_iterator iter = d->_properties.find(key);
            if(iter != d->_properties.end()){
                return iter->second;
            }
            return basic_variant_null;
        }

        void Event::setEventId(const int64_t &id)
        {
            D_P(Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_event_id = id;
        }

        int64_t Event::eventId() const
        {
            D_P(const Event);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_event_id;
        }

    }
}

