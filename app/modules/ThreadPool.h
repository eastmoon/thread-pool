#pragma once
#include <thread>
#include <vector>

namespace ThreadPool {
    /*
    // Worker，工作者
    //
    // Thread 可以採用多個方式啟用，然若欲將執行函數、共通變數、區域變數封裝，則可透過 Object 將要運用的工作者封裝，並透過操作函數 () 並在複製函數作為執行緒進入點。
    //
    // 設計此類別需注意：
    // 1. 執行緒間共享變數應使用指標在執行前傳入
    // 2. 執行緒內區域、物件變數則可於建構或函數執行時在產生
    // 3. 透過進入點開始執行的類別函數，皆屬於執行緒內的處理行為，不與外部共通
    // 4. 進入點應提供抽象設計，並採用樣板模式設計，讓開發者繼承後改寫實體物件的執行函數  ( exec() )
    */
    class Worker {
    public:
        Worker() {}
        void operator() () {
            this->exec();
        }
    protected:
        virtual void exec() {}
    };
    /*
    // ThreadSet，執行緒集
    //
    // Thread 依據其執行行為 ( join or detach ) 在系統解構時會觸發 std::terminate 以此結束執行緒運作；然若需中斷一個仍在運作中的執行緒，則會需要使用基礎的 pthread 功能進行移除。
    // How do I terminate a thread in C++11?, ref : https://stackoverflow.com/questions/12207684
    //
    // 設計執行緒集是一個封裝執行緒移除必要資訊的類別，藉由此類別可透過 new、delete 的結構來中斷執行緒，避免統一在系統結束時一次性移除。
    */
    class ThreadSet {
    public:
        ThreadSet(std::thread& _thread): m_thread(_thread), m_handle(_thread.native_handle()) {}
        ~ThreadSet() {
            pthread_cancel(this->m_handle);
        }
        std::thread& get() {
            return this->m_thread;
        }
    private:
        std::thread& m_thread;
        std::thread::native_handle_type m_handle;
    };
    /*
    // ThreadGuard，執行緒警衛
    //
    // Thread 在宣告後便會開始執行，然而若執行緒仍為 joinable 狀態時觸發解構，會倒置下述文獻的錯誤。
    //// C++11: What happens if you don't call join() for std::thread, ref : https://stackoverflow.com/questions/27392743
    //// Ensure Join or Detach Before Thread Destruction in C++, ref : https://leimao.github.io/blog/CPP-Ensure-Join-Detach-Before-Thread-Destruction/
    //
    // 而 Resource Acquisition Is Initialization ( RAII ) 法則則藉由物件生命週期結束來確保 Thread 必定執行一種行為 ( join or detach )，以此確保在解構發生前必定不會處於 joinable 狀態，相關內容參考夏墅文獻。
    //// RAII cppreference, ref : https://en.cppreference.com/w/cpp/language/raii
    //// Discussion about std::thread and RAII, ref : https://isocpp.org/files/papers/p0206r0.html
    //// C++ Threading : Thread Management RAII, ref : https://gist.github.com/anubhavrohatgi/74137561707121a7ac604002d6ef240f
    //// 使用 RAII 完成线程等待, ref : https://www.cnblogs.com/jingyg/p/5970003.html
    */
    enum ThreadAction {join, detach};
    class ThreadGuard {
    public:
        ThreadGuard(std::thread& _thread, ThreadAction _action = ThreadAction::join): m_thread(_thread), m_action(_action) {}
        ~ThreadGuard() {
            if (this->m_thread.joinable()) {
                if (this->m_action == ThreadAction::join) {
                    this->m_thread.join();
                } else {
                    this->m_thread.detach();
                }
            }
        }
        //delete the copy constructor and copy assignment operator as thread cannot be copied or assigned only can it be moved
        ThreadGuard(const ThreadGuard &)= delete;
        ThreadGuard& operator=(const ThreadGuard &)= delete ;
        //
        std::thread& get() {
            return this->m_thread;
        }
        ThreadPool::ThreadSet* set() {
            return new ThreadPool::ThreadSet(this->m_thread);
        }
    private:
        std::thread& m_thread;
        ThreadAction m_action;
    };

    /*
    // Pool，執行緒池
    //
    // Pool 會基於宣告的 Worker 與指定執行數量，控制啟動或關閉執行緒。
    */
    template <class WORKER, class DATA>
    class Pool {
    public:
        // Constructor
        Pool( int _number = 1, ThreadAction _action = ThreadAction::join ) : m_number(_number), m_action(_action) {
            this->m_threads.reserve(this->m_number);
        }
        Pool( DATA* _data, int _number = 1, ThreadAction _action = ThreadAction::join ) : m_worker(_data), m_number(_number), m_action(_action) {
            this->m_threads.reserve(this->m_number);
        }
        ~Pool() {
            this->stop();
        }
        //delete the copy constructor and copy assignment operator
        Pool(const Pool &)= delete;
        Pool& operator=(const Pool &)= delete ;
        // Play all thread, if thread set isn't exist
        void play() {
            if ( this->m_threads.size() != this->m_number ) {
                // Remove thread set vector have more than size number
                if ( this->m_threads.size() > this->m_number ) {
                    for( int i = this->m_threads.size() - 1 ; i >= this->m_number ; i-- ) {
                        if ( this->m_threads[i] != nullptr ) {
                            delete this->m_threads[i];
                            this->m_threads[i] = nullptr;
                        }
                    }
                }
                // Change vector container size
                this->m_threads.resize(this->m_number);
                // Add thread set, if cantainer is null
                for( int i = 0 ; i < this->m_number ; i++ ) {
                    if ( this->m_threads[i] == nullptr ) {
                        ThreadSet* t = createThreadSet();
                        this->m_threads[i] = t;
                    }
                }
            }
        }
        // Stop all thread, if thread set is exist
        void stop() {
            if (this->m_threads.size() > 0) {
                for_each(
                    this->m_threads.begin(),
                    this->m_threads.end(),
                    [](const ThreadSet* _set) {
                        if ( _set != nullptr ) {
                            delete _set;
                        }
                    }
                );
                this->m_threads.clear();
            }
        }
        // Size will add or remove thread set.
        void size(int _number) {
            if ( _number < 0 ) {
                _number = 0;
            }
            this->m_number = _number;
            this->m_threads.reserve(this->m_number);
        }
    private:
        ThreadSet* createThreadSet() {
            std::thread t = std::thread(this->m_worker);
            ThreadPool::ThreadGuard tg(t, this->m_action);
            return new ThreadPool::ThreadSet(t);
        }
    private:
        WORKER m_worker;
        ThreadAction m_action;
        int m_number;
        std::vector<ThreadSet*> m_threads;
    };
}
