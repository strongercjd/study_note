Linux 操作系统，我们要使用 POSIX 编写多线程 C++ 程序。POSIX Threads 或 Pthreads 提供的 API 可在多种类 Unix POSIX 系统上可用，比如 FreeBSD、NetBSD、GNU/Linux、Mac OS X 和 Solaris。

# 创建线程

下面的程序，我们可以用它来创建一个 POSIX 线程：

``` cpp
#include <pthread.h>
pthread_create (thread, attr, start_routine, arg) 
```

在这里，**pthread_create** 创建一个新的线程，并让它可执行。下面是关于参数的说明：

| 参数          | 描述                                                         |
| :------------ | :----------------------------------------------------------- |
| thread        | 指向线程标识符指针。                                         |
| attr          | 一个不透明的属性对象，可以被用来设置线程属性。您可以指定线程属性对象，也可以使用默认值 NULL。 |
| start_routine | 线程运行函数起始地址，一旦线程被创建就会执行。               |
| arg           | 运行函数的参数。它必须通过把引用作为指针强制转换为 void 类型进行传递。如果没有传递参数，则使用 NULL。 |

创建线程成功时，函数返回 0，若返回值不为 0 则说明创建线程失败。

# 终止线程

使用下面的程序，我们可以用它来终止一个 POSIX 线程：

``` cpp
#include <pthread.h>
pthread_exit (status) 
```

在这里，**pthread_exit** 用于显式地退出一个线程。通常情况下，pthread_exit() 函数是在线程完成工作后无需继续存在时被调用。

如果 main() 是在它所创建的线程之前结束，并通过 pthread_exit() 退出，那么其他线程将继续执行。否则，它们将在 main() 结束时自动被终止。

# 示例代码

以下简单的实例代码使用 pthread_create() 函数创建了 5 个线程，每个线程输出"Hello Runoob！":

``` cpp
#include <iostream>
// 必须的头文件
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS 5
 
// 线程的运行函数
void* say_hello(void* args)
{
    cout << "Hello Runoob！" << endl;
    return 0;
}
 
int main()
{
    // 定义线程的 id 变量，多个变量使用数组
    pthread_t tids[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);
        if (ret != 0)
        {
           cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    pthread_exit(NULL);
}
```
直接编译会报错
``` cpp
$ g++ test.cpp 
/usr/bin/ld: /tmp/ccitX2hv.o: in function `main':
test.cpp:(.text+0x8d): undefined reference to `pthread_create'
collect2: error: ld returned 1 exit status
```

使用 -lpthread 库编译下面的程序：

``` cpp
$ g++ test.cpp -lpthread -o test.o
```

>以上是在ubuntu20.04编译的结果。ubuntu22.04就不需要需要添加"-lpthread"选项，具体看readme.md

> **为确保跨平台兼容性，​始终显式添加-pthread选项是最稳妥的做法**


运行结果

``` cpp
$ ./test.o
Hello Runoob！
Hello Runoob！
Hello Runoob！
Hello Runoob！
Hello Runoob！
```

以下简单的实例代码使用 pthread_create() 函数创建了 5 个线程，并接收传入的参数。每个线程打印一个 "Hello Runoob!" 消息，并输出接收的参数，然后调用 pthread_exit() 终止线程。

``` cpp
//文件名：test.cpp
 
#include <iostream>
#include <cstdlib>
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS     5
 
void *PrintHello(void *threadid)
{  
   // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
   int tid = *((int*)threadid);
   if(tid == 4)
   {
      while(1)
      {
         cout << "special thread" << endl;
         cout << "Hello Runoob! 线程 ID, " << tid << endl;
      }
   }else{
      cout << "Hello Runoob! 线程 ID, " << tid << endl;
   }
   pthread_exit(NULL);
}
 
int main ()
{
   pthread_t threads[NUM_THREADS];
   int indexes[NUM_THREADS];// 用数组来保存i的值
   int rc;
   int i;
   for( i=0; i < NUM_THREADS; i++ ){      
      // cout << "main() : 创建线程, " << i << endl;
      indexes[i] = i; //先保存i的值
      // 传入的时候必须强制转换为void* 类型，即无类型指针        
      rc = pthread_create(&threads[i], NULL, 
                          PrintHello, (void *)&(indexes[i]));
      if (rc){
         cout << "Error:无法创建线程," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
```

运行结果

``` cpp
$ g++ test.cpp -lpthread -o test.o
$ ./test.o
main() : 创建线程, 0
main() : 创建线程, 1
Hello Runoob! 线程 ID, 0
main() : 创建线程, Hello Runoob! 线程 ID, 21

main() : 创建线程, 3
Hello Runoob! 线程 ID, 2
main() : 创建线程, 4
Hello Runoob! 线程 ID, 3
Hello Runoob! 线程 ID, 4
```

# 向线程传递参数

这个实例演示了如何通过结构传递多个参数。您可以在线程回调中传递任意的数据类型，因为它指向 void，如下面的实例所示：

``` cpp
#include <iostream>
#include <cstdlib>
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS     5
 
struct thread_data{
   int  thread_id;
   char *message;
};
 
void *PrintHello(void *threadarg)
{
   struct thread_data *my_data;
 
   my_data = (struct thread_data *) threadarg;
 
   cout << "Thread ID : " << my_data->thread_id ;
   cout << " Message : " << my_data->message << endl;
 
   pthread_exit(NULL);
}
 
int main ()
{
   pthread_t threads[NUM_THREADS];
   struct thread_data td[NUM_THREADS];
   int rc;
   int i;
 
   for( i=0; i < NUM_THREADS; i++ ){
      cout <<"main() : creating thread, " << i << endl;
      td[i].thread_id = i;
      td[i].message = (char*)"This is message";
      rc = pthread_create(&threads[i], NULL,
                          PrintHello, (void *)&td[i]);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
```

运行结果

``` cpp
$ g++ -Wno-write-strings test.cpp -lpthread -o test.o
$ ./test.o
main() : creating thread, 0
main() : creating thread, 1
Thread ID : 0 Message : This is message
main() : creating thread, Thread ID : 21
 Message : This is message
main() : creating thread, 3
Thread ID : 2 Message : This is message
main() : creating thread, 4
Thread ID : 3 Message : This is message
Thread ID : 4 Message : This is message

```