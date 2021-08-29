// 定義測試模組名稱
#include "../app/modules/JobPool.h"

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( JobPool_Test_Suite )

// 類別宣告
class AtomicCountWorker: public JobPool::Worker {
public:
    AtomicCountWorker(std::atomic<long>* _count) : m_count(_count) {}
protected:
    void exec() {
        *this->m_count += 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
private:
    std::atomic<long>* m_count;
};

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Pool_Case_Atomic_Count_Default_Number )
{
    // Declare atomic count variable
    std::atomic<long> count(0);

    // Declare thread controller & worker
    JobPool::Pool<AtomicCountWorker, std::atomic<long>> p(&count);
    p.play();

    // Test script
    // Step 1
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 1 : count number 1s, value = " << count);
    BOOST_CHECK(count >= 98);

    // Step 2
    p.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    long tmp = count;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 2 : lock count 1s, value = " << count);
    BOOST_CHECK_EQUAL(count, tmp);

    // Step 3
    p.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 3 : unlock count 1s, value = " << count);
    BOOST_CHECK(count >= 196);
}

// 測試案例
BOOST_AUTO_TEST_CASE( JobPool_Pool_Case_Atomic_Count_Assign_Number )
{
    // Declare atomic count variable
    std::atomic<long> count(0);

    // Declare thread controller & worker
    JobPool::Pool<AtomicCountWorker, std::atomic<long>> p(&count, 2);
    p.play();

    // Test script
    // Step 1
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 1 : count number 1s, value = " << count);
    BOOST_CHECK(count >= 196);

    // Step 2
    p.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    long tmp = count;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 2 : lock count 1s, value = " << count);
    BOOST_CHECK_EQUAL(count, tmp);

    // Step 3
    p.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_TEST_MESSAGE("Step 3 : unlock count 1s, value = " << count);
    BOOST_CHECK(count >= 392);
}


// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
