#include <event/eventdispatch.h>
#include <logger/logger.h>

#include <map>
#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace extension{

    namespace core{

        class EventDispatchPrivate
        {
            DECLARE_PUBLIC(EventDispatch)
        public:
            EventDispatchPrivate(EventDispatch* q);
            ~EventDispatchPrivate();

            std::vector<EventDispatch::EventCallback> find_id_call_back(const int64_t& id);

            EventDispatch*const                             q_ptr;
            boost::mutex                                    _mutex;
            std::map<int64_t,
            std::map<EventDispatch::EventPriority,
            std::vector<EventDispatch::EventCallback> > >   _listeners;
        };

        EventDispatchPrivate::EventDispatchPrivate(EventDispatch *q)
            : q_ptr(q)
        {
            
        }

        EventDispatchPrivate::~EventDispatchPrivate()
        {

        }

        std::vector<EventDispatch::EventCallback> EventDispatchPrivate::find_id_call_back(const int64_t &id)
        {
            std::vector<EventDispatch::EventCallback> result;
            result.insert(result.end(),_listeners[id][EventDispatch::HighEventPriority].begin(),_listeners[id][EventDispatch::HighEventPriority].end());
            result.insert(result.end(),_listeners[id][EventDispatch::NormalEventPriority].begin(),_listeners[id][EventDispatch::NormalEventPriority].end());
            result.insert(result.end(),_listeners[id][EventDispatch::LowEventPriority].begin(),_listeners[id][EventDispatch::LowEventPriority].end());
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

        void EventDispatch::registerEvent(const int64_t& id, const EventCallback& call, const EventPriority& priority)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_listeners[id][priority].push_back(call);
            LOG_DBG(extension.core.event) << "register event id: " << id << " priority: " << priority;
        }

        void EventDispatch::unRegisterEvent(const int64_t& id)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_listeners.erase(id);
            LOG_DBG(extension.core.event) << "unregister event id: " << id;
        }

        void EventDispatch::publishEvent(const int64_t& id, const Event& var, const ConnectionType& priority)
        {
            D_P(EventDispatch);
            boost::mutex::scoped_lock lock(d->_mutex);
            std::vector<EventDispatch::EventCallback> callbacks = d->find_id_call_back(id);
            for(std::vector<EventDispatch::EventCallback>::const_iterator iter = callbacks.begin();
                iter != callbacks.end();
                ++iter)
            {
                if(EventDispatch::UniqueConnection == priority){
                    boost::thread t(*iter,var);
                    t.detach();
                }else if(EventDispatch::QueuedConnection == priority){
                    if((*iter)(var)){
                       break; 
                    }
                }
            }
        }

        void EventDispatch::sendPostedEvents(const int64_t& id, const Event& var)
        {
            EventDispatch::inst()->publishEvent(id,var,UniqueConnection);
        }




    }
}