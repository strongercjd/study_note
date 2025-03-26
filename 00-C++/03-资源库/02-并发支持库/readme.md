# 多线程简述

多线程是多任务处理的一种特殊形式，多任务处理允许让电脑同时运行两个或两个以上的程序。一般情况下，两种类型的多任务处理：**基于进程和基于线程**。

1. 基于进程的多任务处理是程序的并发执行。
2. 基于线程的多任务处理是同一程序的片段的并发执行。

多线程程序包含可以同时运行的两个或多个部分。这样的程序中的每个部分称为一个线程，每个线程定义了一个单独的执行路径。


# pthread 和 std::thread

#include <pthread.h> 和 #include <thread> 是两种不同的线程API，分别对应于POSIX线程（POSIX Threads）和C++11标准库中的线程支持。

pthread（POSIX Threads）：

pthread.h 是POSIX标准的一部分，用于在类Unix操作系统（如Linux、macOS等）中创建和管理线程。
使用C语言接口，函数命名风格遵循POSIX规范，例如 pthread_create() 创建线程，pthread_join() 等待线程结束，以及使用各种mutex、条件变量等同步原语。
需要手动管理线程资源的生命周期，比如显式地调用 pthread_create() 创建线程，然后在适当的时候调用 pthread_detach() 或 pthread_join() 来释放线程资源。
不是C++的标准部分，因此不是跨平台的。Windows系统下需要依赖第三方库来实现类似功能。

释放cpu延时
``` cpp
#include <unistd.h>
usleep(1000000); // 延迟1秒
```

C++11 std::thread：

C++11标准引入了 <thread> 头文件，提供了 std::thread 类来创建和管理线程。
使用C++面向对象的方式，可以直接从可调用对象（函数、lambda表达式或具有operator()的对象）创建线程，并且支持RAII（Resource Acquisition Is Initialization）机制，当 std::thread 对象超出作用域时会自动尝试终止关联的线程。
同步原语（如互斥量 std::mutex，条件变量 std::condition_variable）设计为与C++标准库中的其他组件更加兼容和易于使用，如通过 std::lock_guard 实现RAII式的锁管理。
标准化的C++ API，旨在提供跨平台的多线程编程能力，只要编译器和运行环境支持C++11标准即可使用。
总结一下，pthread 更底层、更接近操作系统API，而 std::thread 提供了一种更现代、面向对象、符合C++习惯且通常更易于使用的多线程编程模型。

释放cpu延时
``` cpp
#include <thread>
#include <chrono>
std::this_thread::sleep_for(std::chrono::microseconds(500));
```

# 链接库的问题

* 在ubuntu20.04编译需要添加"-lpthread"(pthread)或"-pthread"(std::thread)。g++的版本是gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2)

* 在ubuntu22.04编译时不需要添加"-lpthread"(pthread)选项 或 "-pthread"(std::thread)选项,g++的版本是g++ (Ubuntu 11.2.0-19ubuntu1) 11.2.0

**根本原因**

Ubuntu 22.04 ​系统对 GCC 的打包配置进行了调整​（如默认启用 -pthread 或隐式依赖线程库），而非 GCC 编译器版本本身的功能变更

> 对于生产环境，为确保跨平台兼容性，​始终显式添加-pthread选项是最稳妥的做法。

一开始我认为是gcc版本的问题，后来我在ubuntu20.04安装了gcc11.2.0，但是还是报了链接库的问题
```bash
e$ cat /etc/os-release
NAME="Ubuntu"
VERSION="20.04.6 LTS (Focal Fossa)"
ID=ubuntu
ID_LIKE=debian
PRETTY_NAME="Ubuntu 20.04.6 LTS"
VERSION_ID="20.04"
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
VERSION_CODENAME=focal
UBUNTU_CODENAME=focal

$ g++ --version
g++ (Ubuntu 11.4.0-2ubuntu1~20.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ g++ main.cpp 
/usr/bin/ld: /tmp/cc7NTpx2.o: in function `std::thread::thread<void (&)(), , void>(void (&)())':
main.cpp:(.text._ZNSt6threadC2IRFvvEJEvEEOT_DpOT0_[_ZNSt6threadC5IRFvvEJEvEEOT_DpOT0_]+0x33): undefined reference to `pthread_create'
collect2: error: ld returned 1 exit status
```

