#include <iostream>

#include <logger/logger.h>
#include <event/eventdispatch.h>
#include <basicvariant/basicvariant.h>
#include <event/event.h>

#include <functional>

void test(const extension::core::Event& var)
{
    std::cout<<var.eventId()<<"--"<<var.basicVariant().which()<<"--"<<var.basicVariant().variant_value<char*>()<<std::endl;
}

int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();

    LOG_INF(logger_main) << " test";

    extension::core::EventDispatch::EventCallback callback = std::bind(test,std::placeholders::_1);

    extension::core::EventDispatch::inst()->registerEvent(1,NULL,callback);

    {
        extension::core::EventDispatch::inst()->publishEvent(1,"test");
    }

    getchar();
    return 0;
}