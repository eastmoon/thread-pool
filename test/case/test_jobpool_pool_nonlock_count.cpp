// 定義測試模組名稱
#include "../app/modules/JobPool.h"

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( JobPool_Test_Suite )

// 類別宣告
class CountWorker: public JobPool::Worker {
public:
    CountWorker(long* _count) : m_count(_count) {}
protected:
    void exec() {
        *this->m_count += 1;
    }
private:
    long* m_count;
};

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Pool_Case_Nonlock_Count )
{
    // Declare atomic count variable
    long count = 0;

    // Declare thread controller & worker
    JobPool::Pool<CountWorker, long> p(&count, 2);
    p.play();

    // Test script
    // Step 1
    // When thread is work at unlock or sleep loop, it will run to fast, and crash with use variable which system remove at application close.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 1 : count number 1s, value = " << count);
    BOOST_CHECK(count > 2000);

    // Step 2
    // Follow step 1 issue, when thread stop loop, it still need a few time ( 1ms ) that atomic lock work.
    p.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    long tmp = count;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 2 : lock count 1s, value = " << count);
    BOOST_CHECK_EQUAL(count, tmp);
}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
