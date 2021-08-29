// 定義測試模組名稱
#include <thread>
#include "../app/modules/ThreadPool.h"

// 類別宣告
class Worker: public ThreadPool::Worker  {
public:
    Worker(long* _count) : m_count(_count), m_number(0) {}
    void exec() {
        for ( int i = 0 ; i < 10 ; i++ ) {
            *this->m_count += 1;
            this->m_number += 1;
        }
        BOOST_TEST_MESSAGE("Step 0 : number value in worker = " << this->m_number);
    }
    long m_number;
private:
    long* m_count;
};

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( JobPool_Test_Suite )

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Worker_Case_Count )
{
    // Declare atomic count variable
    long count = 0;
    Worker w(&count);

    // Declare thread controller & worker
    std::thread t1(w);
    std::thread t2(w);
    t1.join();
    t2.join();

    // Test script
    // Step 1
    BOOST_TEST_MESSAGE("Step 1 : count value = " << count);
    BOOST_CHECK(count == 20);
    BOOST_TEST_MESSAGE("Step 1 : number value = " << w.m_number);
    BOOST_CHECK(w.m_number == 0);

}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
