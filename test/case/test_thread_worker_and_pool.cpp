// 定義測試模組名稱
#include <thread>
#include "../app/modules/ThreadPool.h"

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( ThreadPool_Test_Suite )

// 類別宣告
bool lock = false;
class LockWorker: public ThreadPool::Worker  {
public:
    void exec() {
        lock = !lock;
    }
};

class AtomicCountWorker: public ThreadPool::Worker {
public:
    AtomicCountWorker(std::atomic<long>* _count) : m_count(_count) {}
protected:
    void exec() {
        for ( int i = 0 ; i < 100 ; i++ ) {
            *this->m_count += 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
private:
    std::atomic<long>* m_count;
};

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_Pool_Case_Non_assignment_worker )
{
    // Declare thread controller & worker
    BOOST_TEST_MESSAGE("Step 0 : Declare Pool 1 worker and default action (join), worker didn't have assignment.");
    ThreadPool::Pool<LockWorker, void> p(1);
    p.play();
    BOOST_TEST_MESSAGE("Step 1 : lock is on");
    BOOST_CHECK(lock);
}

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_Pool_Case_Count_2 )
{
    // Declare atomic count variable
    std::atomic<long> count(0);

    // Declare thread controller & worker
    BOOST_TEST_MESSAGE("Step 0 : Declare Pool 2 worker and join action ");
    ThreadPool::Pool<AtomicCountWorker, std::atomic<long>> p(&count, 2, ThreadPool::ThreadAction::join);
    p.play();
    // Test script
    // Step 1
    BOOST_TEST_MESSAGE("Step 1 : action join will run over all count, value = " << count);
    BOOST_CHECK(count >= 200);
}

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_Pool_Case_Count_1 )
{
    // Declare atomic count variable
    std::atomic<long> count(0);

    // Declare thread controller & worker
    BOOST_TEST_MESSAGE("Step 0 : Declare Pool 2 worker and detach action ");
    ThreadPool::Pool<AtomicCountWorker, std::atomic<long>> p(&count, 2, ThreadPool::ThreadAction::detach);
    p.play();
    // Test script
    // Step 1
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 1 : count number 100ms, value = " << count);
    BOOST_CHECK(count >= 20);
    // Step 2
    p.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 2 : stop pool and wait 100ms, value = " << count);
    BOOST_CHECK(count >= 20);
    // Step 3
    p.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 3 : re-play Pool and count number 100ms, value = " << count);
    BOOST_CHECK(count >= 40);
}

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_Pool_Case_reszie )
{
    // Declare atomic count variable
    std::atomic<long> count(0);
    // Declare thread controller & worker
    BOOST_TEST_MESSAGE("Step 0 : Declare Pool 1 worker and detach action ");
    ThreadPool::Pool<AtomicCountWorker, std::atomic<long>> p(&count, 1, ThreadPool::ThreadAction::detach);
    p.play();
    // Test script
    // Step 1
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 1 : count number 100ms, value = " << count);
    BOOST_CHECK(count >= 10);
    // Step 2
    p.size(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 2 : call size(2) but no re-call play, wait 100ms, value = " << count);
    BOOST_CHECK(count >= 20);
    // Step 3
    p.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 3 : re-call play, wait 100ms, value = " << count);
    BOOST_CHECK(count >= 40);
    // Step 4
    p.size(1);
    p.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    BOOST_TEST_MESSAGE("Step 4 : call size(1) & re-play, wait 100ms, value = " << count);
    BOOST_CHECK(count >= 50);
}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
