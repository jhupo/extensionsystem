#include <iostream>

#include <logger/logger.h>
#include <event/eventdispatch.h>
#include <basicvariant/basicvariant.h>

#include <boost/bind.hpp>

bool test(const extension::core::Event& var)
{
    std::cout<<var.eventId()<<"--"<<var.basicVariant().which()<<"--"<<var.basicVariant().variant_value<char*>()<<std::endl;
}

int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();

    LOG_INF(logger_main) << " test";

    extension::core::EventDispatch::EventCallback callback = boost::bind(&test,_1);

    extension::core::EventDispatch::inst()->registerEvent(1,callback);

    {
        char* asasd = "1233123";
        extension::core::Event as(1,extension::core::BasicVariant(asasd));
        extension::core::EventDispatch::inst()->publishEvent(1,as);
    }



    return 0;
}