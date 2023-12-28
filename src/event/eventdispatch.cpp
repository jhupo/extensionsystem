#include <event/eventdispatch.h>
#include <logger/logger.h>
#include <event/event.h>

#include <map>
#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/unordered_map.hpp>

namespace extension{

    namespace core{

        class EventDispatchPrivate
        {
            DECLARE_PUBLIC(EventDispatch)
        public:
            EventDispatchPrivate(EventDispatch* q);
            ~EventDispatchPrivate();

            std::vector<EventDispatch::EventCallback> find_id_call_back(int64_t id);

            EventDispatch*const                             q_ptr;
            boost::mutex                                    _mutex;
            boost::unordered_map<int64_t,
            boost::unordered_map<EventDispatch::EventPriority,
            boost::unordered_map<const void*,
            std::vector<EventDispatch::EventCallback> > > > _listeners;
        };

        EventDispatchPrivate::EventDispatchPrivate(EventDispatch *q)
            : q_ptr(q)
        {
            
        }

        EventDispatchPrivate::~EventDispatchPrivate()
        {

        }

        std::vector<EventDispatch::EventCallback> EventDispatchPrivate::find_id_call_back(int64_t id)
        {
            std::vector<EventDispatch::EventCallback> result;
            boost::unordered_map<const void*,std::vector<EventDispatch::EventCallback> > map = _listeners[id][EventDispatch::HighEventPriority];
            map.insert(_listeners[id][EventDispatch::NormalEventPriority].begin(),_listeners[id][EventDispatch::NormalEventPriority].end());
            map.insert(_listeners[id][EventDispatch::LowEventPriority].begin(),_listeners[id][EventDispatch::LowEventPriority].end());
            for(boost::unordered_map<const void*,std::vector<EventDispatch::EventCallback> >::const_iterator iter = map.begin();
                iter != map.end();
                ++iter)
            {
                result.insert(result.end(),iter->second.begin(),iter->second.end());
            }
            return result;
        }

        EventDispatch::EventDispatch()
            : d_ptr(new EventDispatchPrivate(this))
        {

        }

        EventDispatch::~EventDispatch()
        {

        }

        EventDispatch* EventDispatch::inst()
        {
            static EventDispatch _inst_;
            return &_inst_;
        }

        void EventDispatch::registerEvent(int64_t id, const void* object, const EventCallback& call, const EventPriority& priority)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_listeners[id][priority][object].push_back(call);
            LOG_DBG(extension.core.event) << "register event id: " << id << " priority: " << priority << " object: " << object;
        }

        void EventDispatch::unRegisterEvent(int64_t id, const void* object)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_listeners[id][EventDispatch::HighEventPriority].erase(object);
            d->_listeners[id][EventDispatch::NormalEventPriority].erase(object);
            d->_listeners[id][EventDispatch::LowEventPriority].erase(object);
            LOG_DBG(extension.core.event) << "unregister event id: " << id << " object: " << object;
        }

        void EventDispatch::publishEvent(int64_t id, const BasicVariant& var, const ConnectionType& priority)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            std::vector<EventDispatch::EventCallback> callbacks = d->find_id_call_back(id);
            for(std::vector<EventDispatch::EventCallback>::const_iterator iter = callbacks.begin();
                iter != callbacks.end();
                ++iter)
            {
                if(EventDispatch::UniqueConnection == priority){
                    boost::thread t(*iter,Event(id,var));
                    t.detach();
                }else if(EventDispatch::QueuedConnection == priority){
                    (*iter)(Event(id,var));
                }
            }
        }


    }
}