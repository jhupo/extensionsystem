#ifndef __BASIC_VARIANT_H__
#define __BASIC_VARIANT_H__

#include <extension_global.h>

#include <boost/variant.hpp>

namespace extension{

    namespace core{

        struct EXTENSION_EXPORT basic_variant_format_t
        {
            bool operator==(const basic_variant_format_t& other) const {
                return true;
            }
        };

        typedef boost::variant<void*,char,unsigned char,char*,unsigned char*,
                               short,unsigned short,int,unsigned int,long,
                               unsigned long,float,double,std::string,bool,basic_variant_format_t> variant_t;

        class EXTENSION_EXPORT BasicVariant : public variant_t
        {
        public:
            enum{variant_null,variant_char,variant_uchar,variant_char_ptr,variant_uchar_ptr,
                 variant_short,variant_ushort,variant_int,variant_uint,variant_long,variant_ulong,
                 variant_float,variant_double,variant_string,variant_bool,variant_format};
            BasicVariant();
            BasicVariant(const char& var);
            BasicVariant(const unsigned char& var);
            BasicVariant(const char* var);
            BasicVariant(const unsigned char* var);
            BasicVariant(const short& var);
            BasicVariant(const unsigned short& var);
            BasicVariant(const int& var);
            BasicVariant(const unsigned int& var);
            BasicVariant(const long& var);
            BasicVariant(const unsigned long& var);
            BasicVariant(const float& var);
            BasicVariant(const double& var);
            BasicVariant(const std::string& var);
            BasicVariant(const bool& var);
            BasicVariant(const basic_variant_format_t& var);
            BasicVariant(const variant_t& var);
            virtual~BasicVariant();

        public:

            bool variant_value_null()const;

            template<typename T>
            inline T variant_value(){
                variant_value<T>(*this);
            }

        public:

            static bool variant_equal_apply_visitor(const BasicVariant& _1, const BasicVariant& _2);
            static BasicVariant variant_assign_apply_visitor(const BasicVariant& var);
            static void variant_smart_deleter_apply_visitor(const BasicVariant& var);
            static bool variant_value_null(const BasicVariant& var);
            template<typename T>
            static T variant_value(const BasicVariant& var){
                throw std::runtime_error("Not implemented");
            }
        };

#define basic_varinat_null variant_t((void*)NULL)

    }

}


#endif