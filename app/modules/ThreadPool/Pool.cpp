// Include standard library
#include <iostream>
// Include local library header
#include "Pool.h"
// Declare namespace
using namespace std;
using namespace ThreadPool;

// Constructor
Pool::Pool() {

}

// Destructor

// Set thread worker, default is null.
void Pool::setWorker(ThreadPool::Worker* _worker=nullptr) {
}

// Set maximum thread number, default 1.
void Pool::setMax(int _maxThreadNum = 1) {
}

// Set minimum thread number, default 1.
void Pool::setMin(int _minThreadNum = 1) {
}

// Set queue threshold, if queue have data more then queue threshold, add running thread.
void Pool::setQueueThreshold(int _queueThreshold = 1) {
}

// Set alert threshold, if queue hvae data more then alert threshold, add alert information in log. Alert threshold must big then queue threshold.
void Pool::setAlertThreshold(int _alertThreadhold = 1) {
}

// Operate action, play running thread pool.
void Pool::play() {
}

// Operate action, pause running thread pool.
void Pool::pause() {
}

// Operate action, stop running thread pool.
void Pool::stop() {
}

// Addition thread data in queue, and wait for worker process.
void Pool::add(ThreadPool::Data* _threadData) {
}
