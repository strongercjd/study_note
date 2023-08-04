# 简介

使用 std::condition_variable 的 wait 会把目前的线程 thread 停下来并且等候事件通知，而在另一个线程中可以使用 std::condition_variable 的 notify_one 或 notify_all 发送通知那些正在等待的事件，在多线程中经常使用

condition_variable 常用成员函数:
- wait：阻塞当前线程直到条件变量被唤醒
- notify_one：通知一个正在等待的线程
- notify_all：通知所有正在等待的线程

使用 wait **必须搭配 std::unique_lock<std::mutex> 一起使用**

# notify_one

用 notify_one 通知一个正在 wait 的线程

先梳理流程，该例先开一个新的线程 worker_thread 然后使用 wait 等待

此时 worker_thread 会阻塞(block)直到事件通知才会被唤醒

之后 main 主程序延迟个 5 ms 再使用 notify_one 发送

之后 worker_thread 收到 来自主线程的事件通知就离开 wait 继续往下 cout 完就结束该线程

主程序延迟 5ms 是避免一开始线程还没建立好来不及 wait 等待通知，主程序就先发送 notify_one 事件通知
``` CPP
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cond_var;

void worker_thread()
{
    std::unique_lock<std::mutex> lock(m);
    std::cout << "worker_thread() wait\n";
    cond_var.wait(lock);

    // after the wait, we own the lock.
    std::cout << "worker_thread() is processing data\n";
}

int main()
{
    std::thread worker(worker_thread);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    std::cout << "main() notify_one\n";
    cond_var.notify_one();

    worker.join();
    std::cout << "main() end\n";
}
```
运行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
worker_thread() wait
main() notify_one
worker_thread() is processing data
main() end
```

# notify_all

用 notify_all 通知全部多个 wait 等待的线程

该例主要目的是建立5个线程并等待通知,之后主程序执行go函数里的cond_var.notify_all()去通知所有正在等待的线程

这5个线程分别收到通知后从wait函数离开，之后检查ready为true则离开循环,接着打印thread id然后结束该线程
``` cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cond_var;
bool ready = false;

void print_id(int id)
{
    std::unique_lock<std::mutex> lock(m);
    while (!ready)
    {
        cond_var.wait(lock);
    }
    std::cout << "thread " << id << '\n';
}

void go()
{
    std::unique_lock<std::mutex> lock(m);
    ready = true;
    cond_var.notify_all();
}

int main()
{
    std::thread threads[5];
    // spawn 5 threads:
    for (int i = 0; i < 5; ++i)
        threads[i] = std::thread(print_id, i);

    std::cout << "5 threads ready to race...\n";
    go();

    for (auto &th : threads)
        th.join();

    return 0;
}
```
运行结果
``` cpp
$ g++ -pthread demo.cp
$ ./a.out 
5 threads ready to race...
thread 0
thread 1
thread 3
thread 2
thread 4
```
该例中多使用了一个额外的 ready 变量来辅助判断，也间接介绍了cond_var.wait的另一种用法

使用一个 while 循环来不断检查 ready 变量，条件不成立的话就cond_var.wait继续等待

等到下次cond_var.wait被唤醒又会再度检查这个 ready 值，一直循环检查下去

该技巧在某些情形下可以避免假唤醒这个问题,简单说就是「cond_var.wait被唤醒后还要多判断一个 bool 变量，一定要条件成立才会结束等待，否则继续等待

注意，其中while写法
``` cpp
while (!ready) {
    cond_var.wait(lock);
}
```
可以简化写成如下，亦即 wait 的另一种用法，多带一个关键词在第二个参数
``` cpp
cond_var.wait(lock, []{return ready;});
```
因为 wait 内部的实现方法如下，等价于上面这种写法
``` cpp
template <typename _Predicate>
void wait(unique_lock<mutex> &__lock, _Predicate __p)
{
    while (!__p())
        wait(__lock);
}
```

# wait 等待通知且有条件的结束等待

上文简单提及cond_var.wait带入第二个参数的用法，该例中，worker_thread里的cond_var.wait

第一参数传入一个 unique_lock 锁

第二个参数传入一个有返回的函数，来判断是否要停止等待，回传一个 bool 变量

如果回传 true ，condition_variable 停止等待、继续往下执行，如果回传 false ，则重新开始等待下一个通知

因此等价于
``` cpp
while (!pred()) { wait(lock); }
```
注意 main 里是有一个 lock_guard 与 unique_lock，worker_thread 里有一个 unique_lock
``` cpp
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cond_var;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
    // Wait until main() sends data
    std::unique_lock<std::mutex> lock(m);
    std::cout << "worker_thread() wait\n";
    cond_var.wait(lock, []
                  { return ready; });

    // after the wait, we own the lock.
    std::cout << "worker_thread() is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "worker_thread() signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cond_var.notify_one();
}

int main()
{
    std::thread worker(worker_thread);

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cond_var.notify_one();

    // wait for the worker
    {
        std::unique_lock<std::mutex> lock(m);
        cond_var.wait(lock, []
                      { return processed; });
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}
```
执行结果
``` cpp
$ g++ -pthread demo.cpp 
$ ./a.out 
main() signals data ready for processing
worker_thread() wait
worker_thread() is processing data
worker_thread() signals data processing completed
Back in main(), data = Example data after processing
```

# 典型的生产者与消费者案例

在设计模式(design pattern)中，这是一个典型的生产者与消费者(producer-consumer)的例子

范例里有一位生产者每1秒生产了1个东西放到 condvarQueue 里，这个 condvarQueue 会在去通知消费者，消费者收到通知后从 queue 里拿出这个东西来做事情
``` cpp
#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>

/* conditional variable queue  条件变量队列
条件变量是一种线程间的同步机制，用于满足特定条件之前使线程等待
它通常与互斥锁(mutex)一起使用，在多线程编成中实现线程间同步与协调*/
class condvarQueue
{
    std::queue<int> produced_nums;
    std::mutex m;
    std::condition_variable cond_var;
    bool done = false; // 是否结束
    bool notified = false;

public:
    void push(int i)
    {
        std::unique_lock<std::mutex> lock(m);
        produced_nums.push(i);
        notified = true;
        cond_var.notify_one();
    }
    // 消费者
    template <typename Consumer>
    void consume(Consumer consumer)
    {
        std::unique_lock<std::mutex> lock(m);
        while (!done)
        {
            while (!notified) // 使用notified条件循环避免假的唤醒
            {
                cond_var.wait(lock);
            }
            while (!produced_nums.empty())
            {
                consumer(produced_nums.front());
                produced_nums.pop();
            }
            notified = false;
        }
    }

    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m);
            done = true;
            notified = true;
        }
        cond_var.notify_one();
    }
};

int main()
{
    condvarQueue queue;
    // 生产者线程
    std::thread producer([&]()
                         {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "producing " << i << '\n';
            queue.push(i);
        }
        queue.close(); });

    // 消费者线程
    std::thread consumer([&]()
                         { queue.consume([](int input)
                                         { std::cout << "consuming " << input << '\n'; }); });

    producer.join();
    consumer.join();
}
```
运行结果
``` cpp
$ g++ -pthread demo.cpp
$ ./a.out 
producing 0
consuming 0
producing 1
consuming 1
producing 2
consuming 2
producing 3
consuming 3
producing 4
consuming 4
```

# 总结

等待的线程应有下列几个步骤：
1. 获得 std::unique_lock 锁，并用该锁来保护共享变量
2. 检查有没有满足结束等待的条件，以预防数据早已经被更新与被通知了
3. 执行 wait 等待，wait 操作会自动释放该 mutex 并且暂停该线程
4. 当 condition variable 通知时，该线程被唤醒，且该mutex自动被重新获得，该线程应该检查一些条件决定要不要继续等待

通知的线程应有下列几个步骤：

1. 获取一个 std::mutex (通常通过std::lock_guard)
2. 在上锁的范围内完成变量的修改
3. 执行 std::condition_variable 的notify_one/notify_all








