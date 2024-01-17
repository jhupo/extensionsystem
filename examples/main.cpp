#include <iostream>

#include <functional>

#include <logger/logger.h>


int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();

    LOG_INFO(logger) << "test";

    getchar();
    return 0;
}