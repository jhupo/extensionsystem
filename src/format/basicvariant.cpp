#include <format/basicvariant.h>

#include <boost/thread/mutex.hpp>

namespace extension{

    namespace core{

        class BasicVariantPrivate
        {
            DECLARE_PUBLIC(BasicVariant)
        public:
            BasicVariantPrivate(BasicVariant* q, const BasicVariant::basic_variant_container_or_single_t& _v);
            ~BasicVariantPrivate();

            BasicVariant*const                                  q_ptr;
            mutable boost::mutex                                _mutex; 
            BasicVariant::basic_variant_container_or_single_t   _variant;
        };

        BasicVariantPrivate::BasicVariantPrivate(BasicVariant *q, const BasicVariant::basic_variant_container_or_single_t &_v)
            : q_ptr(q)
            , _variant(_v)
        {

        }

        BasicVariantPrivate::~BasicVariantPrivate()
        {

        }

        BasicVariant::BasicVariant(const basic_variant_container_or_single_t &variant)
            : d_ptr(new BasicVariantPrivate(this, variant))
        {

        }

        BasicVariant::~BasicVariant()
        {

        }

        void BasicVariant::set_value(const basic_variant_container_or_single_t &variant)
        {
            D_P(BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            d->_variant = variant;
        }

        bool BasicVariant::value_null() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_variant.which() == variant_null && boost::get<void*>(d->_variant) == 0;
        }

        bool BasicVariant::operator==(const BasicVariant &_other) const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            return boost::apply_visitor(BasicVariantVisitor(), d->_variant, _other.value<basic_variant_container_or_single_t>());
        }

        template<>
        inline void* BasicVariant::value<void*>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() != variant_null){
                return DECL_EQ_NULLPTR;
            }
            return boost::get<void*>(d->_variant);
        }

        template<>
        inline unsigned short BasicVariant::value<unsigned short>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            unsigned short result = 0;
            if(d->_variant.which() == variant_u_short){
                result = boost::get<unsigned short>(d->_variant);
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<unsigned short>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<unsigned short>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<unsigned short>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<unsigned short>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<unsigned short>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<unsigned short>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<unsigned short>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline unsigned int BasicVariant::value<unsigned int>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            unsigned int result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<unsigned int>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = boost::get<unsigned int>(d->_variant);
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<unsigned int>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<unsigned int>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<unsigned int>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<unsigned int>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<unsigned int>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<unsigned int>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline unsigned long BasicVariant::value<unsigned long>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            unsigned long result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<unsigned long>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<unsigned long>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = boost::get<unsigned long>(d->_variant);
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<unsigned long>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<unsigned long>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<unsigned long>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<unsigned long>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<unsigned long>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline unsigned long long BasicVariant::value<unsigned long long>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            unsigned long long result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<unsigned long long>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<unsigned long long>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<unsigned long long>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = boost::get<unsigned long long>(d->_variant);
            }else if(d->_variant.which() == variant_short){
                result = static_cast<unsigned long long>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<unsigned long long>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<unsigned long long>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<unsigned long long>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline unsigned char* BasicVariant::value<unsigned char*>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_u_char_ptr){
                return boost::get<unsigned char*>(d->_variant);
            }else if(d->_variant.which() == variant_char_ptr){
                return reinterpret_cast<unsigned char*>(boost::get<char*>(d->_variant));
            }else if(d->_variant.which() == variant_string){
                return reinterpret_cast<unsigned char*>(const_cast<char*>(boost::get<std::string>(d->_variant).c_str()));
            }else if(d->_variant.which() == variant_null){
                return reinterpret_cast<unsigned char*>(boost::get<void*>(d->_variant));
            }
            return DECL_EQ_NULLPTR;
        }

        template<>
        inline short BasicVariant::value<short>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            short result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<short>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<short>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<short>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<short>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = boost::get<short>(d->_variant);
            }else if(d->_variant.which() == variant_int){
                result = static_cast<short>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<short>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<short>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline int BasicVariant::value<int>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            int result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<int>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<int>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<int>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<int>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<int>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = boost::get<int>(d->_variant);
            }else if(d->_variant.which() == variant_long){
                result = static_cast<int>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<int>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline long BasicVariant::value<long>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            long result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<long>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<long>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<long>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<long>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<long>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<long>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = boost::get<long>(d->_variant);
            }else if(d->_variant.which() == variant_long_long){
                result = static_cast<long>(boost::get<long long>(d->_variant));
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline long long BasicVariant::value<long long>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            long long result = 0;
            if(d->_variant.which() == variant_u_short){
                result = static_cast<long long>(boost::get<unsigned short>(d->_variant));
            }else if(d->_variant.which() == variant_u_int){
                result = static_cast<long long>(boost::get<unsigned int>(d->_variant));
            }else if(d->_variant.which() == variant_u_long){
                result = static_cast<long long>(boost::get<unsigned long>(d->_variant));
            }else if(d->_variant.which() == variant_u_long_long){
                result = static_cast<long long>(boost::get<unsigned long long>(d->_variant));
            }else if(d->_variant.which() == variant_short){
                result = static_cast<long long>(boost::get<short>(d->_variant));
            }else if(d->_variant.which() == variant_int){
                result = static_cast<long long>(boost::get<int>(d->_variant));
            }else if(d->_variant.which() == variant_long){
                result = static_cast<long long>(boost::get<long>(d->_variant));
            }else if(d->_variant.which() == variant_long_long){
                result = boost::get<long long>(d->_variant);
            }else{
                throw std::runtime_error("Not implemented");
            }
            return result;
        }

        template<>
        inline char* BasicVariant::value<char*>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_u_char_ptr){
                return reinterpret_cast<char*>(boost::get<unsigned char*>(d->_variant));
            }else if(d->_variant.which() == variant_char_ptr){
                return boost::get<char*>(d->_variant);
            }else if(d->_variant.which() == variant_string){
                return const_cast<char*>(boost::get<std::string>(d->_variant).c_str());
            }else if(d->_variant.which() == variant_null){
                return reinterpret_cast<char*>(boost::get<void*>(d->_variant));
            }
            return DECL_EQ_NULLPTR;
        }

        template<>
        inline std::string BasicVariant::value<std::string>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_u_char_ptr){
                return std::string(reinterpret_cast<char*>(boost::get<unsigned char*>(d->_variant)));
            }else if(d->_variant.which() == variant_char_ptr){
                return std::string(boost::get<char*>(d->_variant));
            }else if(d->_variant.which() == variant_string){
                return boost::get<std::string>(d->_variant);
            }else if(d->_variant.which() == variant_null){
                return std::string(reinterpret_cast<char*>(boost::get<void*>(d->_variant)));
            }
            return std::string();
        }

        template<>
        inline bool BasicVariant::value<bool>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_bool){
                return boost::get<bool>(d->_variant);
            }
            return false;
        }

        template<>
        inline std::vector<BasicVariant::basic_variant_t> BasicVariant::value<std::vector<BasicVariant::basic_variant_t> >() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_vector){
                return boost::get<std::vector<basic_variant_t> >(d->_variant);
            }
            return std::vector<basic_variant_t>();
        }

        template<>
        inline std::list<BasicVariant::basic_variant_t> BasicVariant::value<std::list<BasicVariant::basic_variant_t> >() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_list){
                return boost::get<std::list<basic_variant_t> >(d->_variant);
            }
            return std::list<basic_variant_t>();
        }

        template<>
        inline std::set<BasicVariant::basic_variant_t> BasicVariant::value<std::set<BasicVariant::basic_variant_t> >() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_set){
                return boost::get<std::set<basic_variant_t> >(d->_variant);
            }
            return std::set<basic_variant_t>();
        }

        template<>
        inline std::map<BasicVariant::basic_variant_t, BasicVariant::basic_variant_t> BasicVariant::value<std::map<BasicVariant::basic_variant_t, BasicVariant::basic_variant_t> >() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_map){
                return boost::get<std::map<basic_variant_t, basic_variant_t> >(d->_variant);
            }
            return std::map<basic_variant_t, basic_variant_t>();
        }

        template<>
        inline basic_format_t BasicVariant::value<basic_format_t>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            if(d->_variant.which() == variant_format){
                return boost::get<basic_format_t>(d->_variant);
            }
            return basic_format_t();
        }

        template<>
        inline BasicVariant::basic_variant_container_or_single_t BasicVariant::value<BasicVariant::basic_variant_container_or_single_t>() const
        {
            D_P(const BasicVariant);
            boost::mutex::scoped_lock lock(d->_mutex);
            return d->_variant;
        }

    }
}