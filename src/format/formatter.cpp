#include <format/formatter.h>
#include <logger/logger.h>

#include <boost/thread/mutex.hpp>

namespace extension{

    namespace core{

        class FormatterPrivate
        {
            DECLARE_PUBLIC(Formatter)
        public:
            FormatterPrivate(Formatter* q);
            ~FormatterPrivate();

            Formatter*const                                                             q_ptr;
            mutable boost::mutex                                                        _mutex;
            std::list<BasicVariant>                                                     _args;
            std::list<std::pair<BasicVariant,BasicVariant> >                            _kargs;
            std::list<Formatter::ptr >                                                  _children;
        };

        FormatterPrivate::FormatterPrivate(Formatter *q)
            : q_ptr(q)
        {
            
        }

        FormatterPrivate::~FormatterPrivate()
        {

        }

        Formatter::Formatter(const BasicVariant &var)
            : basic_variant_format_t(var)
            , d_ptr(new FormatterPrivate(this))
        {
            
        }

        Formatter::~Formatter()
        {

        }

        void Formatter::clear()
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_args.clear();
            d->_kargs.clear();
            d->_children.clear();
        }

        void Formatter::addChildren(const ptr &children)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_children.push_back(children);
        }

        void Formatter::addVariant(const BasicVariant &var)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_args.push_back(var);
        }

        void Formatter::addVariant(const BasicVariant &first, const BasicVariant &second)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_kargs.push_back(std::make_pair(first,second));
        }

        void Formatter::addVariant(const BasicVariant &first, const std::list<std::pair<BasicVariant, BasicVariant> > &second)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            Formatter::ptr formater(new Formatter(first));
            formater->reset(second);
            d->_children.push_back(formater);
        }

        void Formatter::reset(const std::list<ptr>& children)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_children = children;
        }

        void Formatter::reset(const std::list<BasicVariant>& args)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_args = args;
        }

        void Formatter::reset(const std::list<std::pair<BasicVariant,BasicVariant> >& kargs)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_kargs = kargs;
        }

        std::list<Formatter::ptr> Formatter::children() const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_children;
        }

        std::list<BasicVariant> Formatter::args() const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_args;
        }

        std::list<std::pair<BasicVariant,BasicVariant> > Formatter::kargs() const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_kargs;
        }

        Formatter::ptr Formatter::findChildren(const BasicVariant &var) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<ptr>::const_iterator iter = d->_children.begin();
                iter != d->_children.end();
                ++iter)
            {
                if((*iter)->variant() == var){
                    return *iter;
                }
            }
            return ptr();
        }

        BasicVariant Formatter::findArgs(const BasicVariant &var) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<BasicVariant>::const_iterator iter = d->_args.begin();
                iter != d->_args.end();
                ++iter)
            {
                if(*iter == var){
                    return *iter;
                }
            }
            return basic_variant_null;
        }

        BasicVariant Formatter::findKargs(const BasicVariant &var) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<std::pair<BasicVariant,BasicVariant> >::const_iterator iter = d->_kargs.begin();
                iter != d->_kargs.end();
                ++iter)
            {
                if(iter->first == var){
                    return iter->second;
                }
            }
            return basic_variant_null;
        }

        std::list<BasicVariant> Formatter::kargsMember() const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            std::list<BasicVariant> result;
            for(std::list<std::pair<BasicVariant,BasicVariant> >::const_iterator iter = d->_kargs.begin();
                iter != d->_kargs.end();
                ++iter)
            {
                result.push_back(iter->first);
            }
            return result;
        }

        bool Formatter::isArgsMember(const BasicVariant &var) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<BasicVariant>::const_iterator iter = d->_args.begin();
                iter != d->_args.end();
                ++iter)
            {
                if(*iter == var){
                    return true;
                }
            }
            return false;
        }

        bool Formatter::isKargsMember(const BasicVariant &var) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<std::pair<BasicVariant,BasicVariant> >::const_iterator iter = d->_kargs.begin();
                iter != d->_kargs.end();
                ++iter)
            {
                if(iter->first == var){
                    return true;
                }
            }
            return false;
        }

        bool Formatter::isChildrenMember(const BasicVariant &children) const
        {
            D_P(const Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<ptr>::const_iterator iter = d->_children.begin();
                iter != d->_children.end();
                ++iter)
            {
                if((*iter)->variant() == children){
                    return true;
                }
            }
            return false;
        }

        void Formatter::eraseArgs(const BasicVariant &var)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<BasicVariant>::iterator iter = d->_args.begin();
                iter != d->_args.end();
                ++iter)
            {
                if(*iter == var){
                    d->_args.erase(iter);
                    break;
                }
            }
        }

        void Formatter::eraseKargs(const BasicVariant &var)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<std::pair<BasicVariant,BasicVariant> >::iterator iter = d->_kargs.begin();
                iter != d->_kargs.end();
                ++iter)
            {
                if(iter->first == var){
                    d->_kargs.erase(iter);
                    break;
                }
            }
        }

        void Formatter::eraseChildren(const BasicVariant &children)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            for(std::list<ptr>::iterator iter = d->_children.begin();
                iter != d->_children.end();
                ++iter)
            {
                if((*iter)->variant() == children){
                    d->_children.erase(iter);
                    break;
                }
            }
        }

        void Formatter::addChildren(const Formatter& children)
        {
            D_P(Formatter);
            boost::mutex::scoped_lock lock(d->_mutex);
            Formatter::ptr formatter(new Formatter(children.variant()));
            formatter->reset(children.args());
            formatter->reset(children.kargs());
            formatter->reset(children.children());
            d->_children.push_back(formatter);
        }

    }
}