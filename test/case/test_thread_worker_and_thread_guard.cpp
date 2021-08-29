// 定義測試模組名稱
#include <thread>
#include "../app/modules/ThreadPool.h"

// 類別宣告
class Worker: public ThreadPool::Worker  {
public:
    Worker(long* _count) : m_count(_count) {}
    void exec() {
        for ( int i = 0 ; i < 10 ; i++ ) {
            *this->m_count += 1;
        }
    }
private:
    long* m_count;
};

void runThread(long* _count) {
    BOOST_TEST_MESSAGE("Step 1.1 : Create thread, count : " << *_count);
    BOOST_CHECK(*_count == 0);
    Worker w(_count);
    std::thread t(w);
    ThreadPool::ThreadGuard tg(t);
    BOOST_TEST_MESSAGE("Step 1.2 : Before runThread close, count : " << *_count);
    BOOST_CHECK(*_count >= 0);
}

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( JobPool_Test_Suite )

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_and_ThreadGuard_Case_Count )
{
    // Declare atomic count variable
    long count = 0;
    // Declare thread controller & worker
    runThread(&count);
    // Test
    BOOST_TEST_MESSAGE("Step 2 : After runThread over, count : " << count);
    BOOST_CHECK(count == 10);
}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
