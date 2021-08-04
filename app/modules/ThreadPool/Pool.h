#pragma once
// Include standard library
#include <iostream>
#include <vector>
// Include local library header
#include "Worker.h"
#include "Data.h"

namespace ThreadPool {
    class Pool {
        public:
            // Constructor
            Pool();
            // Destructor
            // Set thread worker, default is null.
            void setWorker(ThreadPool::Worker*);
            // Set maximum thread number, default 1.
            void setMax(int);
            // Set minimum thread number, default 1.
            void setMin(int);
            // Set queue threshold, if queue have data more then queue threshold, add running thread.
            void setQueueThreshold(int);
            // Set alert threshold, if queue hvae data more then alert threshold, add alert information in log. Alert threshold must big then queue threshold.
            void setAlertThreshold(int);
            // Operate action, play running thread pool.
            void play();
            // Operate action, pause running thread pool.
            void pause();
            // Operate action, stop running thread pool.
            void stop();
            // Addition thread data in queue, and wait for worker process.
            void add(ThreadPool::Data*);
        protected:
            ThreadPool::Worker* m_worker;
            std::vector<ThreadPool::Data*> m_dataQueue;
            int m_maxThreadNum;
            int m_minThreadNum;
            int m_queueThreshold;
            int m_alertThreshold;
    };
}
