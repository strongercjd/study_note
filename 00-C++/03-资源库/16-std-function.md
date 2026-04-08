# std::function 介绍

`std::function` 是 C++11 标准库 `<functional>` 中提供的**通用可调用对象包装器**。它可以存储、复制和调用任何**可调用对象**——包括普通函数、函数指针、lambda 表达式、`std::bind` 表达式、仿函数（重载了 `operator()` 的类），甚至是成员函数指针（经过适配后）。

简单说：`std::function` 给所有“可以像函数一样调用”的东西提供了一个统一的类型。

## 基本语法

```cpp
#include <functional>

std::function<返回类型(参数类型列表)>  变量名;
```

例如：

```cpp
std::function<int(int, int)> func;  // 可以存储任何接受两个int、返回int的可调用对象
```

## 核心特性

- **类型擦除**：虽然底层可调用对象类型不同，但通过 `std::function` 包装后，它们都呈现为同一个类型。
- **可作为函数参数、返回值、容器元素**：便于实现回调、策略模式、延迟执行等。
- **可空**：未包装任何可调用对象时，`operator bool()` 返回 `false`；调用会抛出 `std::bad_function_call`。

## 基本用法示例

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) { return a + b; }

struct Multiply {
    int operator()(int a, int b) const { return a * b; }
};

int main() {
    // 1. 包装普通函数
    std::function<int(int,int)> f1 = add;
    std::cout << f1(3, 4) << std::endl;   // 7

    // 2. 包装 lambda
    std::function<int(int,int)> f2 = [](int a, int b) { return a - b; };
    std::cout << f2(10, 3) << std::endl;  // 7

    // 3. 包装仿函数对象
    Multiply mul;
    std::function<int(int,int)> f3 = mul;
    std::cout << f3(5, 6) << std::endl;   // 30

    // 4. 包装成员函数（需要 std::bind 或 lambda 配合）
    struct Calculator {
        int divide(int a, int b) const { return a / b; }
    };
    Calculator calc;
    std::function<int(int,int)> f4 = std::bind(&Calculator::divide, &calc, _1, _2);
    // 或者用 lambda（更现代）
    std::function<int(int,int)> f5 = [&calc](int a, int b) { return calc.divide(a, b); };
    std::cout << f4(10, 2) << std::endl;   // 5

    // 5. 可空判断
    std::function<void()> f6;
    if (f6) {
        f6();  // 不会执行，因为 f6 为空
    } else {
        std::cout << "f6 is empty" << std::endl;
    }
    return 0;
}
```

## std::function 与 std::bind 的关系

这两者经常一起出现，但角色不同：

- **`std::bind` 是一个生成器**：它接受一个可调用对象和若干参数（实参或占位符），**产生一个新的可调用对象**（具体类型是编译器生成的、未指定的绑定表达式类型）。
- **`std::function` 是一个容器**：它能够存储任何符合签名的可调用对象，包括 `std::bind` 返回的那个“未指定类型”的对象。

## 为什么常搭配使用？

`std::bind` 返回的类型非常复杂（依赖于绑定的函数和参数），难以直接声明变量或作为函数参数。但 `std::function` 可以轻松包装它，从而获得统一的、易于传递的类型。

```cpp
#include <functional>
using namespace std::placeholders;

void greet(const std::string& name, const std::string& title) {
    std::cout << title << " " << name << std::endl;
}

int main() {
    // bind 返回的类型很长（通常难以写出）
    auto bound = std::bind(greet, "Alice", _1);   // auto 推导
    bound("Ms.");  // 输出: Ms. Alice

    // 用 std::function 包装后，可以明确类型
    std::function<void(const std::string&)> func = bound;
    func("Ms.");   // 输出: Ms. Alice

    // 也可以直接赋值，省略中间 auto
    std::function<void(const std::string&)> func2 = std::bind(greet, "Bob", _1);
    func2("Dr.");  // 输出: Dr. Bob

    // 将 bind 结果存入容器
    std::vector<std::function<void()>> tasks;
    tasks.push_back(std::bind(greet, "Alice", "Ms."));   // 占位符全部固定
    tasks.push_back(std::bind(greet, "Bob",   "Mr."));
    for (auto& task : tasks) task();
}
```

## 结合成员函数的典型模式

成员函数不能直接赋值给 `std::function`，因为隐含的 `this` 参数需要显式处理。`std::bind` 可以把对象实例绑定进来，然后把结果放入 `std::function`：

```cpp
struct Handler {
    void process(int x) { std::cout << x << std::endl; }
};

Handler h;
std::function<void(int)> f = std::bind(&Handler::process, &h, _1);
f(42);
```

也可以用 lambda 代替 bind：

```cpp
std::function<void(int)> f = [&h](int x) { h.process(x); };
```

## function作为回调参数

假设有一个 Downloader 类，下载完成后需要通知调用方。不同调用方可能想用不同的处理方式：普通函数、lambda、类的成员函数等。用 std::function 统一类型即可。

```cpp
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <chrono>

class Downloader
{
public:
  // 回调类型：接受一个表示下载结果的 string
  using Callback = std::function<void(const std::string &)>;

  void downloadAsync(const std::string &url, Callback onComplete)
  {
    // 模拟异步下载
    std::thread([url, onComplete]()
                {
                  std::this_thread::sleep_for(std::chrono::seconds(1));
                  std::string result = "Downloaded: " + url;
                  onComplete(result); // 下载完成后调用回调
                })
        .detach();
  }
};

// 普通函数作为回调
void printResult(const std::string &msg)
{
  std::cout << "[Function] " << msg << std::endl;
}

int main()
{
  Downloader dl;

  using namespace std::placeholders;

  // 1. 使用普通函数
  dl.downloadAsync("http://example.com/file1", printResult);

  // 2. 使用 lambda
  dl.downloadAsync("http://example.com/file2", [](const std::string &res)
                   { std::cout << "[Lambda] " << res << std::endl; });

  // 3. 使用函数对象（带状态）
  struct Logger
  {
    void log(const std::string &s) const
    {
      std::cout << "[Logger] " << s << std::endl;
    }
  };
  Logger logger;
  dl.downloadAsync("http://example.com/file3",
                   std::bind(&Logger::log, logger, _1));

  std::this_thread::sleep_for(std::chrono::seconds(2)); // 等异步完成
  return 0;
}
```

## function放入容器

std::function 可以放入 std::vector 等容器，实现一个任务队列或事件处理器列表，里面可以混合各种可调用对象。

```cpp
#include <iostream>
#include <vector>
#include <functional>

void globalHandler(int x)
{
  std::cout << "Global handler: " << x << std::endl;
}

int main()
{
  // 存储多种 "void(int)" 类型的可调用对象
  std::vector<std::function<void(int)>> handlers;
  using namespace std::placeholders;

  // 添加普通函数
  handlers.push_back(globalHandler);

  // 添加 lambda
  handlers.push_back([](int n)
                     { std::cout << "Lambda: " << n * 2 << std::endl; });

  // 添加函数对象
  struct Square
  {
    void operator()(int n) const
    {
      std::cout << "Square: " << n * n << std::endl;
    }
  };
  handlers.push_back(Square{});

  // 添加 std::bind 的结果
  struct Printer
  {
    void print(int x, const std::string &prefix) const
    {
      std::cout << prefix << x << std::endl;
    }
  };
  Printer p;
  handlers.push_back(std::bind(&Printer::print, p, _1, "[Bound] "));

  // 统一调用所有处理器
  int value = 5;
  for (auto &handler : handlers)
  {
    handler(value);
  }
}
```

## 任务分发器

任务分发器中常用

```cpp
using DispatchMessageFunc = std::function<void(std::function<void()> &&)>;
```

DispatchMessageFunc 就是一个可执行对象，传入的入参就是也是一个"函数"。

### 分解分析

1. **最内层的 `std::function<void()>`**  
   表示一个**无参数、返回 `void`** 的可调用对象包装器。它可以存储 lambda、普通函数、`std::bind` 结果等，只要调用形式是 `void()`。

2. **`std::function<void()> &&`**  
   这是一个**右值引用**，指向 `std::function<void()>` 类型的临时对象。也就是说，参数要求传入的是一个**右值**（例如临时对象，或使用 `std::move` 转换的左值）。

3. **外层的 `std::function<void( ... )>`**  
   表示一个**接受一个参数（即上述右值引用的任务函数）、返回 `void`** 的可调用对象包装器。

4. **`using DispatchMessageFunc = ...`**  
   给这个类型起一个别名，以后可以用 `DispatchMessageFunc` 代替这个长长的类型。

> 右值的问题，看文章<08-左值和右值>

### 通俗解释

`DispatchMessageFunc` 是一个**函数对象类型**，它的实例可以像一个函数一样被调用，调用时你需要给它传递一个“任务”（类型为 `std::function<void()>`），而且这个任务必须通过**移动语义**传递（通常是临时对象或显式 `std::move` 的结果）。

这个模式常见于**任务分发器**或**消息循环**中。例如：

- 你有一个后台线程池，主线程产生任务（如 lambda 表达式），然后调用 `DispatchMessageFunc` 类型的对象，将任务“投递”到工作线程中执行。
- 参数使用右值引用是为了**避免不必要的拷贝**，因为 `std::function` 可能内部存储了较大的状态（如捕获了很多变量的 lambda），移动比拷贝高效。

### 用法示例

假设我们有一个消息分发器

```cpp
#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class MessageQueue
{
  std::queue<std::function<void()>> tasks;
  std::mutex mtx;
  std::condition_variable cv;//条件变量，用于等待任务队列不为空
  bool stop = false;//停止标志

public:
  // 这正是 DispatchMessageFunc 类型所描述的函数
  void dispatch(std::function<void()> &&task)
  {
    {
      std::lock_guard<std::mutex> lock(mtx);
      tasks.push(std::move(task)); // 移动进队列，避免拷贝
    }
    cv.notify_one();//通知一个等待的线程
  }

  void worker()
  {
    while (!stop)
    {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this]
              { return !tasks.empty() || stop; });
      if (stop && tasks.empty())
        break;
      auto task = std::move(tasks.front());
      tasks.pop();
      lock.unlock();
      task(); // 执行任务
    }
  }
};

int main()
{
  MessageQueue mq;

  using DispatchMessageFunc = std::function<void(std::function<void()> &&)>;

  // DispatchMessageFunc 可以指向 MessageQueue::dispatch 函数
  DispatchMessageFunc dispatcher = std::bind(&MessageQueue::dispatch, &mq, std::placeholders::_1);

  // 投递任务
  dispatcher([]
             { std::cout << "Hello from task 1\n"; });
  dispatcher([]
             { std::cout << "Hello from task 2\n"; });

  // 启动工作线程（略，实际需要管理生命周期）
  std::thread worker(&MessageQueue::worker, &mq);
  worker.join(); // 示例简化，实际需停止标志
  return 0;
}
```

### 总结

- **`DispatchMessageFunc`** 是一个**可调用对象**，用于**接收并处理一个任务**。
- 任务本身也是一个可调用对象（`std::function<void()>`），且通过**右值引用**传递，表示对任务的所有权转移（移动语义），提高性能。
- 这种设计在异步编程、线程池、事件循环中非常普遍。
