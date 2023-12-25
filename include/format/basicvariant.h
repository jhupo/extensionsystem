#ifndef __BASICVARIANT_H__
#define __BASICVARIANT_H__

#include "extension_global.h"

#include <boost/variant.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vector>
#include <list>
#include <set>
#include <map>

#include <stdexcept>

namespace extension{

    namespace core{

        struct EXTENSION_EXPORT basic_format_t
        {

        };

        class BasicVariantPrivate;

        class EXTENSION_EXPORT BasicVariant : public boost::enable_shared_from_this<BasicVariant>
        {
            DISABLE_COPY(BasicVariant)
            DECLARE_PRIVATE(BasicVariant)
            DECLARE_SHARED_PTR(BasicVariant)
        public:

            typedef boost::variant<void*,unsigned short, unsigned int, unsigned long, unsigned long long,unsigned char*,
                                   short,int,long,long long,char*,std::string,bool> basic_variant_t;
            typedef boost::variant<std::vector<basic_variant_t>,std::list<basic_variant_t>,std::set<basic_variant_t>,std::map<basic_variant_t,basic_variant_t> > basic_variant_container_t;
            typedef boost::variant<basic_variant_t,basic_variant_container_t,basic_format_t> basic_variant_container_or_single_t;
            enum{variant_null,variant_u_short,variant_u_int,variant_u_long,variant_u_long_long,variant_u_char_ptr,
                 variant_short,variant_int,variant_long,variant_long_long,variant_char_ptr,variant_string,variant_bool,
                 variant_vector,variant_list,variant_set,variant_map,variant_format};

            template<unsigned short, typename S, typename D>
            class EXTENSION_EXPORT BasicVariantParser
            {
            public:
                int deserialize(D, S)
                {throw std::runtime_error("Not implemented");}
                int serialize(D, S)
                {throw std::runtime_error("Not implemented");}
            };

            struct EXTENSION_EXPORT BasicVariantVisitor : public boost::static_visitor<bool>
            {
                template<typename T>
                bool operator()(const T& _t1, const T& _t2)
                {return _t1 == _t2;}
                template<typename T, typename U>
                bool operator()(const T& _t1, const U& _u2)
                {return false;}
                bool operator()(const char* _1, const char* _2)
                {return strcmp(_1,_2) == 0;}
                bool operator()(const char* _1, const std::string& _2)
                {return strcmp(_1,_2.c_str()) == 0;}
                bool operator()(const std::string& _1, const char* _2)
                {return strcmp(_1.c_str(),_2) == 0;}
                bool operator()(const std::string& _1, const std::string& _2)
                {return _1 == _2;}
                bool operator()(const basic_variant_container_or_single_t& _1, const basic_variant_container_or_single_t& _2)
                {return false;}
            };

            BasicVariant(const basic_variant_container_or_single_t& variant = DECL_EQ_NULLPTR);
            virtual~BasicVariant();

            template<typename T>
            inline T value()const
            {throw std::runtime_error("Not implemented");}

            void set_value(const basic_variant_container_or_single_t& variant);

            bool value_null()const;

            bool operator==(const BasicVariant& _other)const;

        private:
            const boost::scoped_ptr<BasicVariantPrivate>  d_ptr;
        };

    }

#define basic_variant_null extension::core::BasicVariant()

}



#endif