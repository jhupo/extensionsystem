#ifndef __BASIC_VARIANT_H__
#define __BASIC_VARIANT_H__

#include <extension_global.h>

#include <boost/variant.hpp>

namespace extension{

    namespace core{

        class basic_variant_format_t;

        typedef boost::variant<void*,char,unsigned char,char*,unsigned char*,
                                short,unsigned short,int,unsigned int,long,
                                unsigned long,float,double,std::string,bool> variant_t;

        class EXTENSION_EXPORT BasicVariant : public variant_t
        {
        public:
            enum VariantType{variant_null,variant_char,variant_uchar,variant_char_ptr,variant_uchar_ptr,
                    variant_short,variant_ushort,variant_int,variant_uint,variant_long,variant_ulong,
                    variant_float,variant_double,variant_string,variant_bool,variant_format};
            BasicVariant();
            BasicVariant(void* var);
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
            BasicVariant(const variant_t& var);
            virtual~BasicVariant();

        public:

            bool variant_value_null()const;

            template<typename T>
            inline T variant_value(){
                return variant_value<T>(*this);
            }

            VariantType variant_type()const;

        public:

            static bool variant_equal_apply_visitor(const BasicVariant& _1, const BasicVariant& _2);
            static BasicVariant variant_assign_apply_visitor(const BasicVariant& var);
            static void variant_smart_deleter_apply_visitor(const BasicVariant& var);
            static bool variant_value_null(const BasicVariant& var);
            template<typename T>
            static T variant_value(const BasicVariant& var){
                throw std::runtime_error("Not implemented");
            }
        public:
            bool operator<(const BasicVariant& var)const;
            bool operator==(const BasicVariant& var)const;
        };

#define basic_variant_null variant_t((void*)NULL)

        template<>   
        void* BasicVariant::variant_value<void*>(const BasicVariant &var);
        template<>
        char BasicVariant::variant_value<char>(const BasicVariant &var);
        template<>
        unsigned char BasicVariant::variant_value<unsigned char>(const BasicVariant &var);
        template<>
        char* BasicVariant::variant_value<char*>(const BasicVariant &var);
        template<>
        unsigned char* BasicVariant::variant_value<unsigned char*>(const BasicVariant &var);
        template<>
        short BasicVariant::variant_value<short>(const BasicVariant &var);
        template<>
        unsigned short BasicVariant::variant_value<unsigned short>(const BasicVariant &var);
        template<>
        int BasicVariant::variant_value<int>(const BasicVariant &var);
        template<>
        unsigned int BasicVariant::variant_value<unsigned int>(const BasicVariant &var);
        template<>
        long BasicVariant::variant_value<long>(const BasicVariant &var);
        template<>
        float BasicVariant::variant_value<float>(const BasicVariant& var);
        template<>
        double BasicVariant::variant_value<double>(const BasicVariant& var);
        template<>
        std::string BasicVariant::variant_value<std::string>(const BasicVariant& var);
        template<>
        bool BasicVariant::variant_value<bool>(const BasicVariant& var);

        class EXTENSION_EXPORT basic_variant_format_t
        {
        public:
            basic_variant_format_t(const BasicVariant& var);
            virtual~basic_variant_format_t();
            void setVariant(const BasicVariant& var);
            BasicVariant variant()const;
            template<unsigned short,typename T, typename U>
            class EXTENSION_EXPORT Formatter
            { 
            public:
                int deserialize(const T&, U& )
                {throw std::runtime_error("Not implemented");}
                int serialize(T&, const U&)
                {throw std::runtime_error("Not implemented");}
            };
        private:
            BasicVariant        _var;
        };
    }

}
typedef extension::core::basic_variant_format_t VariantFormatter;

#endif