#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <atomic>

namespace JobPool {
    //
    class Worker {
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
    // WorkerController is a controller to control Worker object work in thread and control Worker loop, Worker thread action.
    // How to terminate a C++ std::thread?, ref : http://www.bo-yang.net/2017/11/19/cpp-kill-detached-thread
    template <class WORKER, class DATA>
    class WorkerController {
    public:
        WorkerController( DATA* _data ) : m_lock(false), m_worker(_data), m_thread(nullptr) {
            this->m_worker.setLock(&this->m_lock);
        }
        ~WorkerController() {
            if ( this->m_thread != nullptr ) {
                pthread_cancel(this->m_thread_handle);
            }
        }
        void play() {
            if ( this->m_thread == nullptr ) {
                std::thread t = std::thread(this->m_worker);
                this->m_thread = &t;
                this->m_thread_handle = this->m_thread->native_handle();
                this->m_thread->detach();
            }
            this->m_lock.store(false);
        }
        void stop() {
            this->m_lock.store(true);
        }
    protected:
        //
        std::thread* m_thread;
        std::thread::native_handle_type m_thread_handle;
        std::atomic<bool> m_lock;
        WORKER m_worker;
    };
    // Pool is a multiple thread controller use Worker object, it action is like WorkerController but it is only run one thread.
    template <class WORKER, class DATA>
    class Pool {
        public:
            // Constructor
            Pool( DATA* _data, int _number = 1 ) : m_lock(false), m_worker(_data), m_number(_number) {
                this->m_worker.setLock(&this->m_lock);
            }
            ~Pool() {
                if (this->m_thread_handles.size() > 0) {
                    for_each(
                        this->m_thread_handles.begin(),
                        this->m_thread_handles.end(),
                        [](const std::thread::native_handle_type& _handle) { pthread_cancel(_handle); }
                    );
                }
            }
            // Operate action, play running thread pool.
            void play() {
                if ( this->m_thread_handles.size() != this->m_number ) {
                    for( int i = 0 ; i < this->m_number ; i++ ) {
                        std::thread t = std::thread(this->m_worker);
                        this->m_thread_handles.push_back(t.native_handle());
                        t.detach();
                    }
                }
                this->m_lock.store(false);
            };
            // Operate action, stop running thread pool.
            void stop() {
                this->m_lock.store(true);
            }
        protected:
            //
            int m_number;
            std::vector<std::thread::native_handle_type> m_thread_handles;
            std::atomic<bool> m_lock;
            WORKER m_worker;
    };
}
