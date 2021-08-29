#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <atomic>
#include "ThreadPool.h"

namespace JobPool {
    // JobPool Worker is base at ThreadPool Worker, and override operator().
    // JobPool is running at infinite loop, it could use lock to control exec method run or not.
    // This Worker need run at ThreadPool::ThreadAction::detach mode.
    class Worker : public ThreadPool::Worker {
    public:
        Worker(std::atomic<bool>* _lock = nullptr) : m_lock(_lock) {}
        void operator() () {
            do {
                if ( this->m_lock != nullptr && ! *this->m_lock ) {
                    this->exec();
                }
            } while(1);
        }
        void setLock(std::atomic<bool>* _lock = nullptr) {
            this->m_lock = _lock;
        }
    protected:
        virtual void exec() {}
    private:
        std::atomic<bool>* m_lock;
    };
    // JobPool Pool is a JobPool Worker controller to control lock, and inherit ThreadPool Pool.
    // How to terminate a C++ std::thread?, ref : http://www.bo-yang.net/2017/11/19/cpp-kill-detached-thread
    template <class WORKER, class DATA>
    class Pool : public ThreadPool::Pool<WORKER, DATA> {
        public:
            // Constructor
            Pool( DATA* _data, int _number = 1 ) : m_lock(false), ThreadPool::Pool<WORKER, DATA>(_data, _number, ThreadPool::ThreadAction::detach) {
                WORKER* w = this->getWorker();
                w->setLock(&this->m_lock);
            }
            // Operate action, play running thread pool.
            void play() {
                ThreadPool::Pool<WORKER, DATA>::play();
                this->m_lock.store(false);
            };
            // Operate action, stop running thread pool.
            void stop() {
                this->m_lock.store(true);
            }
        protected:
            std::atomic<bool> m_lock;
    };
}
