#include <logger/logger.h>

#include <boost/filesystem.hpp>

#include <fstream>

namespace extension{

    namespace logger{

        namespace{
            const char logger_cfg[] = "./logger.ini";
        }

        boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level,std::string> _logger_channel_severity(boost::log::keywords::channel = "logger");

        bool init_logger_environment()
        {
            boost::log::add_common_attributes();
            boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level,char>("Severity");
            boost::log::register_simple_filter_factory<boost::log::trivial::severity_level,char>("Severity");
            std::ifstream file(logger_cfg);
            try{
                boost::log::init_from_stream(file);
            }catch(const std::exception& e){
                BOOST_LOG_TRIVIAL(fatal) << "init_logger failed, read log config file fail. curse: " << e.what() << std::endl;
                return false;
            }
            return true;
        }
        
    }

}
