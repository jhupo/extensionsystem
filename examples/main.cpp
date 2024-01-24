#include <iostream>

#include <functional>

#include <logger/logger.h>

#include <thread/threadpool.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <thread/thread.h>
#include <boost/random.hpp>
#include <vector>
#include <thread>
#include <ctime> 

void test(void*){
    LOG_DEBUG(logger)<<"thread pool test";
    boost::random::mt19937 rng(static_cast<unsigned int>(std::time(0)));
    boost::random::uniform_int_distribution<> dist(500, 10000);
    int randomValue = dist(rng);
    LOG_DEBUG(add_task)<<"msleep = "<<randomValue;
}

void random_add_task()
{
    for(;;){
        extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
        extension::Thread::msleep(1000);
    }
}

int main(int argc, char** argv)
{
    extension::logger::init_logger_environment();


    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    extension::ThreadPoolManager::inst()->addTask(boost::uuids::to_string(boost::uuids::random_generator()()),test);
    LOG_DEBUG(logger)<<"action thread count = "<<extension::ThreadPoolManager::inst()->activeThreadCount();

    getchar();
    LOG_DEBUG(logger)<<"action thread count = "<<extension::ThreadPoolManager::inst()->activeThreadCount();
    return 0;
}