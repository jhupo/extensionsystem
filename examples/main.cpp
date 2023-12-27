#include <iostream>

#include <logger/logger.h>


int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();

    LOG_INF(logger_main) << " test";

    return 0;
}