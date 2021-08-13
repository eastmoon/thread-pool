// 定義測試模組名稱
// #define BOOST_TEST_MODULE Example_1

// 测试套件宣告
BOOST_AUTO_TEST_SUITE( E1_S1 )

// 測試案例 1
BOOST_AUTO_TEST_CASE( E1_S1_Case_1 )
{
    BOOST_TEST( true );
}

// 測試案例 2
BOOST_AUTO_TEST_CASE( E1_S1_Case_2 )
{
    int a = 1;
    BOOST_CHECK_EQUAL(a, 1);
}

// 測試套件結束
BOOST_AUTO_TEST_SUITE_END()
