// 定義測試模組名稱
#include <thread>
#include "../app/modules/ThreadPool.h"

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( ThreadPool_Test_Suite )

// 類別宣告
class CountWorker: public ThreadPool::Worker  {
public:
    CountWorker(long* _count) : m_count(_count) {}
    void exec() {
        for ( int i = 0 ; i < 100 ; i++ ) {
            *this->m_count += 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
private:
    long* m_count;
};

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_ThreadGuard_Case_Count )
{
    // Declare atomic count variable
    long count = 0;
    CountWorker w(&count);
    // Declare thread controller & worker
    std::thread t(w);
    ThreadPool::ThreadSet* ts = new ThreadPool::ThreadSet(t);
    t.detach();
    // Test script
    // Step 1
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 1 : count number 100ms, value = " << count);
    BOOST_CHECK(count >= 9);
    // Step 2
    BOOST_TEST_MESSAGE("Step 2 : deleete thread set");
    delete ts;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // Step 3
    long tmp = count;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 3 : after delete 100ms, value = " << count);
    BOOST_CHECK(tmp == count);
}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
