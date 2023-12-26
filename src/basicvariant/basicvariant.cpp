#include <basicvariant/basicvariant.h>

#include <iostream>

namespace extension{

    namespace core{

        namespace{
            struct variant_equal_visitor : public boost::static_visitor<bool>
            {
                template<typename T>
                bool operator()(const T& _1, const T& _2)const{
                    return _1 == _2;
                }
                template<typename T, typename U>
                bool operator()(const T& _1, const U& _2)const{
                    return false;
                }
                bool operator()(const basic_variant_format_t& _1, const basic_variant_format_t& _2)const{
                    return _1 == _2;
                }
                bool operator()(const char* _1, const char* _2)const{
                    return strcmp(_1,_2) == 0;
                }
                bool operator()(const unsigned char* _1, const unsigned char* _2)const{
                    return strcmp((const char*)_1,(const char*)_2) == 0;
                }
                bool operator()(const char* _1, const std::string& _2)const{
                    return strcmp(_1,_2.c_str()) == 0;
                }
                bool operator()(const std::string& _1, const char* _2)const{
                    return strcmp(_1.c_str(),_2) == 0;
                }
                bool operator()(const unsigned char* _1, const std::string& _2)const{
                    return strcmp((const char*)_1,_2.c_str()) == 0;
                }
                bool operator()(const std::string& _1, const unsigned char* _2)const{
                    return strcmp(_1.c_str(),(const char*)_2) == 0;
                }
                bool operator()(const char* _1, const unsigned char* _2)const{
                    return strcmp(_1,(const char*)_2) == 0;
                }
                bool operator()(const unsigned char* _1, const char* _2)const{
                    return strcmp((const char*)_1,_2) == 0;
                }
            };
            struct variant_assign_visitor : public boost::static_visitor<variant_t>
            {
                variant_t operator()(const BasicVariant& var)const{
                    if(BasicVariant::variant_char_ptr == var.which()){
                        return variant_assign_visitor()(boost::get<char*>(var));
                    }else if(BasicVariant::variant_uchar_ptr == var.which()){
                        return variant_assign_visitor()(boost::get<unsigned char*>(var));
                    }else{
                        return variant_t((void*)NULL);
                    }
                }
                variant_t operator()(const char* var)const{
                    size_t length = strlen(var);
                    char* dest = new char[length + 1];
                    strcpy(dest,var);
                    dest[length] = '\0';
                    return variant_t(dest);
                }
                variant_t operator()(const unsigned char* var)const{
                    size_t length = strlen(reinterpret_cast<char*>(*var));
                    unsigned char* dest = new unsigned char[length + 1];
                    strcpy(reinterpret_cast<char*>(dest), reinterpret_cast<char*>(*var));
                    dest[length] = '\0';
                    return variant_t(dest);
                }
            };
            struct variant_smart_deleter_visitor : public boost::static_visitor<bool>
            {
                bool operator()(const variant_t& src)const{
                    if(BasicVariant::variant_char_ptr == src.which()){
                        delete[] boost::get<char*>(src);
                    }else if(BasicVariant::variant_uchar_ptr == src.which()){
                        delete[] boost::get<unsigned char*>(src);
                    }
                    return true;
                }
            };

        }

        BasicVariant::BasicVariant()
            : variant_t(basic_varinat_null)
        {

        }

        BasicVariant::BasicVariant(const char& var)
            : variant_t(static_cast<char>(var))
        {

        }

        BasicVariant::BasicVariant(const unsigned char& var)
            : variant_t(static_cast<unsigned char>(var))
        {

        }

        BasicVariant::BasicVariant(const char* var)
            : variant_t(variant_assign_visitor()(var))
        {
            std::cout<<var<<std::endl;
        }

        BasicVariant::BasicVariant(const unsigned char* var)
            : variant_t(variant_assign_visitor()(var))
        {

        }

        BasicVariant::BasicVariant(const short& var)
            : variant_t(static_cast<short>(var))
        {

        }

        BasicVariant::BasicVariant(const unsigned short& var)
            : variant_t(static_cast<unsigned short>(var))
        {

        }

        BasicVariant::BasicVariant(const int& var)
            : variant_t(static_cast<int>(var))
        {

        }

        BasicVariant::BasicVariant(const unsigned int& var)
            : variant_t(static_cast<unsigned int>(var))
        {

        }

        BasicVariant::BasicVariant(const long& var)
            : variant_t(static_cast<long>(var))
        {

        }

        BasicVariant::BasicVariant(const unsigned long& var)
            : variant_t(static_cast<unsigned long>(var))
        {

        }

        BasicVariant::BasicVariant(const float& var)
            : variant_t(static_cast<float>(var))
        {

        }

        BasicVariant::BasicVariant(const double& var)
            : variant_t(static_cast<double>(var))
        {

        }

        BasicVariant::BasicVariant(const std::string& var)
            : variant_t(var)
        {

        }

        BasicVariant::BasicVariant(const bool& var)
            : variant_t(static_cast<bool>(var))
        {
            std::cout<<"--"<<var<<std::endl;
        }

        BasicVariant::BasicVariant(const basic_variant_format_t& var)
            : variant_t(var)
        {

        }

        BasicVariant::BasicVariant(const variant_t &var)
            : variant_t(var)
        {

        }

        BasicVariant::~BasicVariant()
        {
            boost::apply_visitor(variant_smart_deleter_visitor(), *this);
        }

        bool BasicVariant::variant_value_null() const
        {
            return variant_null == this->which() && NULL == boost::get<void*>(this);
        }

        bool BasicVariant::variant_equal_apply_visitor(const BasicVariant &_1, const BasicVariant &_2)
        {
            return boost::apply_visitor(variant_equal_visitor(), _1, _2);
        }

        BasicVariant BasicVariant::variant_assign_apply_visitor(const BasicVariant &var)
        {
            return boost::apply_visitor(variant_assign_visitor(), var);
        }

        void BasicVariant::variant_smart_deleter_apply_visitor(const BasicVariant &var)
        {
            boost::apply_visitor(variant_smart_deleter_visitor(), var);
        }

        bool BasicVariant::variant_value_null(const BasicVariant &var)
        {
            return variant_null == var.which() && NULL == boost::get<void*>(var);
        }

        template<>
        void* BasicVariant::variant_value<void*>(const BasicVariant &var)
        {
            switch (var.which())
            {
            case variant_char:{return (void*)&boost::get<char>(var);}
            case variant_uchar:{return (void*)&boost::get<unsigned char>(var);}
            case variant_char_ptr:{return (void*)boost::get<char*>(var);}
            case variant_uchar_ptr:{return (void*)boost::get<unsigned char*>(var);}
            case variant_short:{return (void*)&boost::get<short>(var);}
            case variant_ushort:{return (void*)&boost::get<unsigned short>(var);}
            case variant_int:{return (void*)&boost::get<int>(var);}
            case variant_uint:{return (void*)&boost::get<unsigned int>(var);}
            case variant_long:{return (void*)&boost::get<long>(var);}
            case variant_ulong:{return (void*)&boost::get<unsigned long>(var);}
            case variant_float:{return (void*)&boost::get<float>(var);}
            case variant_double:{return (void*)&boost::get<double>(var);}
            case variant_string:{return (void*)&boost::get<std::string>(var);}
            case variant_bool:{return (void*)&boost::get<bool>(var);}
            case variant_format:{return (void*)&boost::get<basic_variant_format_t>(var);}
            default:
                break;
            }
            return boost::get<void*>(var);
        }

        template<>
        char BasicVariant::variant_value<char>(const BasicVariant &var)
        {
            if(variant_char == var.which()){
                return boost::get<char>(var);
            }else if(variant_uchar == var.which()){
                return static_cast<char>(boost::get<unsigned char>(var));
            }
            return '\0';
        }

        template<>
        unsigned char BasicVariant::variant_value<unsigned char>(const BasicVariant &var)
        {
            if(variant_char == var.which()){
                return static_cast<unsigned char>(boost::get<char>(var));
            }else if(variant_uchar == var.which()){
                return boost::get<unsigned char>(var);
            }
            return '\0';
        }

        template<>
        char* BasicVariant::variant_value<char*>(const BasicVariant &var)
        {
            if(variant_char_ptr == var.which()){
                return boost::get<char*>(var);
            }else if(variant_uchar_ptr == var.which()){
                return reinterpret_cast<char*>(boost::get<unsigned char*>(var));
            }else if(variant_string == var.which()){
                return const_cast<char*>(boost::get<std::string>(var).c_str());
            }
            return DECL_EQ_NULLPTR;
        }

        template<>
        unsigned char* BasicVariant::variant_value<unsigned char*>(const BasicVariant &var)
        {
            if(variant_char_ptr == var.which()){
                return reinterpret_cast<unsigned char*>(boost::get<char*>(var));
            }else if(variant_uchar_ptr == var.which()){
                return boost::get<unsigned char*>(var);
            }else if(variant_string == var.which()){
                return reinterpret_cast<unsigned char*>(const_cast<char*>(boost::get<std::string>(var).c_str()));
            }
            return DECL_EQ_NULLPTR;
        }

        template<>
        short BasicVariant::variant_value<short>(const BasicVariant &var)
        {
            if(variant_short == var.which()){
                return boost::get<short>(var);
            }else if(variant_ushort == var.which()){
                return static_cast<short>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return static_cast<short>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return static_cast<short>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return static_cast<short>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<short>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<short>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return static_cast<short>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        unsigned short BasicVariant::variant_value<unsigned short>(const BasicVariant &var)
        {
            if(variant_short == var.which()){
                return static_cast<unsigned short>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return boost::get<unsigned short>(var);
            }else if(variant_int == var.which()){
                return static_cast<unsigned short>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return static_cast<unsigned short>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return static_cast<unsigned short>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<unsigned short>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<unsigned short>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return static_cast<unsigned short>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        int BasicVariant::variant_value<int>(const BasicVariant &var)
        {
            if(variant_short == var.which()){
                return static_cast<int>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return static_cast<int>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return boost::get<int>(var);
            }else if(variant_uint == var.which()){
                return static_cast<int>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return static_cast<int>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<int>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<int>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return static_cast<int>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        unsigned int BasicVariant::variant_value<unsigned int>(const BasicVariant &var)
        {
            if(variant_short == var.which()){
                return static_cast<unsigned int>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return static_cast<unsigned int>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return static_cast<unsigned int>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return boost::get<unsigned int>(var);
            }else if(variant_long == var.which()){
                return static_cast<unsigned int>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<unsigned int>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<unsigned int>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return static_cast<unsigned int>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        long BasicVariant::variant_value<long>(const BasicVariant &var)
        {
            if(variant_short == var.which()){
                return static_cast<long>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return static_cast<long>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return static_cast<long>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return static_cast<long>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return boost::get<long>(var);
            }else if(variant_ulong == var.which()){
                return static_cast<long>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<long>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return static_cast<long>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        float BasicVariant::variant_value<float>(const BasicVariant& var)
        {
            if(variant_short == var.which()){
                return static_cast<float>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return static_cast<float>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return static_cast<float>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return static_cast<float>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return static_cast<float>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<float>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return boost::get<float>(var);
            }else if(variant_double == var.which()){
                return static_cast<float>(boost::get<double>(var));
            }
            return 0;
        }

        template<>
        double BasicVariant::variant_value<double>(const BasicVariant& var)
        {
            if(variant_short == var.which()){
                return static_cast<double>(boost::get<short>(var));
            }else if(variant_ushort == var.which()){
                return static_cast<double>(boost::get<unsigned short>(var));
            }else if(variant_int == var.which()){
                return static_cast<double>(boost::get<int>(var));
            }else if(variant_uint == var.which()){
                return static_cast<double>(boost::get<unsigned int>(var));
            }else if(variant_long == var.which()){
                return static_cast<double>(boost::get<long>(var));
            }else if(variant_ulong == var.which()){
                return static_cast<double>(boost::get<unsigned long>(var));
            }else if(variant_float == var.which()){
                return static_cast<double>(boost::get<float>(var));
            }else if(variant_double == var.which()){
                return boost::get<double>(var);
            }
            return 0;
        }

        template<>
        std::string BasicVariant::variant_value<std::string>(const BasicVariant& var)
        {
            if(variant_char_ptr == var.which()){
                return std::string(boost::get<char*>(var));
            }else if(variant_uchar_ptr == var.which()){
                return std::string(reinterpret_cast<char*>(boost::get<unsigned char*>(var)));
            }else if(variant_string == var.which()){
                return boost::get<std::string>(var);
            }
            return std::string();
        }

        template<>
        bool BasicVariant::variant_value<bool>(const BasicVariant& var)
        {
            switch (var.which())
            {
            case variant_null:{return NULL != boost::get<void*>(var);}
            case variant_char:{return '\0' != boost::get<char>(var);}
            case variant_uchar:{return '\0' != boost::get<unsigned char>(var);}
            case variant_char_ptr:{return NULL != boost::get<char*>(var);}
            case variant_uchar_ptr:{return NULL != boost::get<unsigned char*>(var);}
            case variant_short:{return 0 != boost::get<short>(var);}
            case variant_ushort:{return 0 != boost::get<unsigned short>(var);}
            case variant_int:{return 0 != boost::get<int>(var);}
            case variant_uint:{return 0 != boost::get<unsigned int>(var);}
            case variant_long:{return 0 != boost::get<long>(var);}
            case variant_ulong:{return 0 != boost::get<unsigned long>(var);}
            case variant_float:{return 0 != boost::get<float>(var);}
            case variant_double:{return 0 != boost::get<double>(var);}  
            case variant_string:{return !boost::get<std::string>(var).empty();}
            case variant_bool:{return boost::get<bool>(var);}
            case variant_format:{return sizeof(boost::get<basic_variant_format_t>(var)) > 1;}
            default:
                break;
            }
            return false;
        }

        template<>
        basic_variant_format_t BasicVariant::variant_value<basic_variant_format_t>(const BasicVariant& var)
        {
            if(variant_format == var.which()){
                return boost::get<basic_variant_format_t>(var);
            }
            return basic_variant_format_t();
        }

    }
}
