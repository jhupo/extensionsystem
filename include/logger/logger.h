#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <extension_global.h>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>


namespace extension{

    namespace logger{

        bool EXTENSION_EXPORT init_logger_environment();

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

extern EXTENSION_EXPORT boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level,std::string> _logger_channel_severity;

#define LOG_DEBUG(Module) BOOST_LOG_SEV(extension::logger::_logger_channel_severity, boost::log::trivial::debug)      <<"["<< #Module << "][" << __FUNCTION__ << "][" << __FILENAME__ << ":" << __LINE__ << "]"

#define LOG_INFO(Module)  BOOST_LOG_SEV(extension::logger::_logger_channel_severity, boost::log::trivial::info)       <<"["<< #Module << "][" << __FUNCTION__ << "][" << __FILENAME__ << ":" << __LINE__ << "]"

#define LOG_WARN(Module)  BOOST_LOG_SEV(extension::logger::_logger_channel_severity, boost::log::trivial::warning)    <<"["<< #Module << "][" << __FUNCTION__ << "][" << __FILENAME__ << ":" << __LINE__ << "]"

#define LOG_ERROR(Module) BOOST_LOG_SEV(extension::logger::_logger_channel_severity, boost::log::trivial::error)      <<"["<< #Module << "][" << __FUNCTION__ << "][" << __FILENAME__ << ":" << __LINE__ << "]"

#define LOG_FATAL(Module) BOOST_LOG_SEV(extension::logger::_logger_channel_severity, boost::log::trivial::fatal)      <<"["<< #Module << "][" << __FUNCTION__ << "][" << __FILENAME__ << ":" << __LINE__ << "]"

    }


}


#endif