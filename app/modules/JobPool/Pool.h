#pragma once
// Include standard library
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <atomic>

using namespace std;

namespace JobPool {
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
                        [](const thread::native_handle_type& _handle) { pthread_cancel(_handle); }
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
            vector<thread::native_handle_type> m_thread_handles;
            atomic<bool> m_lock;
            WORKER m_worker;
    };
}
