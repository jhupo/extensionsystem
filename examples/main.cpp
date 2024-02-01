#include <iostream>

#include <functional>

#include <logger/logger.h>
#include <timer/timermanager.h>

#include <thread/threadpool.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <thread/thread.h>
#include <boost/random.hpp>
#include <vector>
#include <thread>
#include <ctime> 

int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();

    getchar();
    return 0;
}