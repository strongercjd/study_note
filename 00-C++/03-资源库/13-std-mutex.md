# 简介

std::mutex是多线程中常用到的基本功能，mutex用来确保同一时间内只有一个线程能够存取这段代码区块，避免发生不预期的意外状况

std::mutex通常不单独使用而是搭配使用，例如：std::lock_guard、std::unique_lock、std::scoped_lock(C++17)，其中最常搭配 std::lock_guard 一起使用

# 无mutex锁

main 建立了两个线程并且会同时存取 print 函式的资源

print 会将带入的参数 c 字符打印 n 次，且每次印完会将 g_count 全局变量的次数加1，print 函数最后再将 g_count 全局变量打印出来

第一个线程为 t1 线程，会输出 10 个 A

第二个为 t2 线程，输出 5 个 B

如果想让 print 某一时间只能某个线程来执行存取的话

先来看看如果没有 mutex 的保护，这个代码的输出会是怎样
``` cpp
#include <iostream>
#include <thread>

using namespace std;

int g_count = 0;

void  print(int n, char c)
{
    for (int i = 0; i < n; ++i)
    {
        std::cout << c;
        g_count++;
    }
    std::cout << '\n';

    std::cout << "count=" << g_count << std::endl;
}

int main()
{
    std::thread t1(print, 10, 'A');
    std::thread t2(print, 5, 'B');
    t1.join();
    t2.join();
    return 0;
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
AAAAAAAAAA
Bcount=BBBB
count=1510
```
如果没上锁的话，可能造成不预期的输出，如下count=5A所示，t2 的 g_count 还没来得及打印完，另一个线程 t1 已经开始抢着印了

另外补充，t1 与 t2 谁先执行并没有一定谁先谁后，每次执行的结果都有可能不同

# 有mutex锁


两个线程都共同存取 g_count 这个全局变量，如果要让执行结果符合预期的话，需上锁

已确保同一时间内只有一个线程能够存取 g_count，当有线程占用，其它线程要存取该资源时，就会被阻塞

直到该资源被线程释放后，才能被其它线程执行

这里在 print 函数里使用 g_mutex.lock() 手动上锁

并且在 print 函式最后使用 g_mutex.unlock() 手动解锁
``` cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex g_mutex;
int g_count = 0;

void print(int n, char c)
{
    g_mutex.lock();
    for (int i = 0; i < n; ++i)
    {
        std::cout << c;
        g_count++;
    }
    std::cout << '\n';

    std::cout << "count=" << g_count << std::endl;
    g_mutex.unlock();
}

int main()
{
    std::thread t1(print, 10, 'A');
    std::thread t2(print, 5, 'B');
    t1.join();
    t2.join();

    return 0;
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
AAAAAAAAAA
count=10
BBBBB
count=15
```

# lock_guard

直接使用 std::mutex 的成员函数 lock/unlock 来上锁是可以的，只是要注意 lock 要有对应的 unlock ，一旦没有解锁到程序很可能发生死锁

那有没有比较便捷的写法来避免这种忘记解锁而导致死锁的问题发生呢？

有的！答案就是配合 std::lock_guard 使用，学会用 std::lock_guard 就可以避免手动上锁解锁，进而减少在写程序上出现死锁的机会

根据前一个范例进行修改，将原本使用 g_mutex 上锁与解锁的动作，换成了 lock_guard，如下范例所示
``` cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex g_mutex;
int g_count = 0;

void print(int n, char c)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    for (int i = 0; i < n; ++i)
    {
        std::cout << c;
        g_count++;
    }
    std::cout << '\n';

    std::cout << "count=" << g_count << std::endl;
}

int main()
{
    std::thread t1(print, 10, 'A');
    std::thread t2(print, 5, 'B');
    t1.join();
    t2.join();

    return 0;
}
```
运行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
AAAAAAAAAA
count=10
BBBBB
count=15
```
在 lock_guard 建构时带入一个 mutex，就会自动将其 mutex 上锁，而在 lock_guard 析构时 mutex 解锁

简单说就是「lock_guard 构建时对 mutex 上锁，析构时对 mutex 解锁」

lock_guard 利用生命周期这概念来进行上锁与解锁，lock_guard 本身并不管理 mutex 的生命周期，也就是 lock_guard 生命周期结束不代表 mutex 生命周期也结束

在该例中，进入 print 后将 g_mutex 带入 lock_guard 建构时上锁，之后离开 print 函数时，lock_guard 生命周期也随之结束，lock_guard 进行析构时对 g_mutex 解锁


