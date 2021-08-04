/*
Boost Pool：基於記憶體池 ( Memory Pool ) 技術，用於快速分配相同大小的記憶體區塊，並有效的反覆分配、釋放其記憶體區塊。

boost::object_pool 為用於物件的記憶體池技術，其操作與運用技術與 pool 相同。
Ref : http://cplusplus.wikidot.com/cn:mempool-example-boost-pool
*/

#include <iostream>
#include <boost/pool/object_pool.hpp>

// 宣告類別
class Worker {
public:
    Worker(int _id, int _count) : m_id(_id), m_count(_count) {};
    ~Worker() {
        this->m_id = 0;
        this->m_count = 0;
    }
    virtual void increase() {
        this->m_count ++;
    }
    virtual void decrease() {
        this->m_count --;
    }
    virtual void show() {
        std::cout << "ID : " << this->m_id << ", Count : " << this->m_count << std::endl;
    }
protected:
    int m_id;
    int m_count;
};


int main()
{

    // 設定 Pool 物件
    boost::object_pool<Worker> alloc;

    // 執行分配與釋放
    std::cout << "-----" << std::endl;
    for (int i = 0; i < 10000; ++i)
    {
        // 藉由記憶體池宣告建構函數。
        Worker* w = alloc.construct(1, 100);
        for ( int j = i ; j > 0 ; --j ) {
            w->increase();
        }
        // do somthing
        w->show();
        // 解構目標物件，可無須呼叫，交由 Pool 管理
        alloc.destroy(w);
    }

    // 取回 Pool 管理的物件區塊，並以 placement new 方式宣告此物件的建構函數
    // placement new ref : https://www.geeksforgeeks.org/placement-new-operator-cpp/
    // placement new 是 C++ 用於做記憶體池的宣告技術，使用此方式可避免 new、delete 行為的 heap 記憶體區的處理 ( 降低宣告時間、記憶體碎片化 )，且物件在程式啟動時宣告令其存於 stack 記憶體區。
    // 其問題，因 new 指向相同的記憶體區塊，多次生成也只為操作同個記憶區塊，並且需透過執行解構函數來做到記憶體釋放。
    std::cout << "-----" << std::endl;
    void* mem = alloc.malloc();
    Worker* w1 = new(mem) Worker(1, 100);
    Worker* w2 = new(mem) Worker(2, 100);
    std::cout << "alloc memory address : " << mem << std::endl;
    std::cout << "W1 memory address : " << w1 << std::endl;
    std::cout << "W2 memory address : " << w2 << std::endl;
    w1->increase();
    w2->increase();
    w1->decrease();
    w1->show();
    w2->show();
    w1->~Worker();
    w2->show();
    w2->~Worker();

    // 使用 Pool 的建構函數，其內部為透過宣告完成的記憶區塊，配合 placement new 的方式呼叫建構；可預想其內將 new 的行為分為兩段 ( 宣告記憶體、執行建構函數 )，以此提高宣告的效率。
    std::cout << "-----" << std::endl;
    Worker* wc1 = alloc.construct(1, 100);
    Worker* wc2 = alloc.construct(2, 100);
    wc1->increase();
    wc2->increase();
    wc1->decrease();
    wc1->show();
    wc2->show();
    wc1->~Worker();
    wc2->show();
    wc2->~Worker();

    return 0;
}
