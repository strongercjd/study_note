# 1、综述

CMake文件**指令**不区分大小写的，**参数和变量**是大小写相关的

# 2、单个文件

新建code文件夹，作为根目录

在根目录新建CMakeLists.txt和main.cpp

CMakeLists.txt文件内容如下

```cpp
# 声明要求的 cmake 最低版本 
cmake_minimum_required( VERSION 2.8 ) 
# 声明一个 cmake 工程 
project( HelloWorld ) 
# 设置编译模式 
set( CMAKE_BUILD_TYPE "Debug" ) 
# 添加一个可执行程序 
# 语法：add_executable( 程序名 源代码文件 ） 
add_executable( HelloWorld main.cpp )
```

main.cpp文件内容如下

```cpp
#include <iostream>
using namespace std;

int main()
{
    printf("Hello World\r\n");
    return 0;
}
```

新建build目录，因为执行cmake时，会在执行目录产生中间文件，所以在build中执行cmake操作，这样中间文件就会产生在build文件夹中

```cpp
$ tree
.
├── build
├── CMakeLists.txt
└── main.cpp
```

在build中执行cmake，会产生Makefile文件，注意执行的是cmake ../或cmake .. 指令，表示工程在当前目录(build)的上一级。

```cpp
cmake ../
```

然后执行make命令，就会产生HelloWorld文件

```cpp
$ ./HelloWorld
Hello World
```

# 3、编译相关

## 3.1 编译选项

有时编译程序时想添加一些编译选项，如-Wall，-std=c++11等，就可以使用add_compile_options来进行操作。

```cpp
add_compile_options(-std=c++11 -Wall) 
```

## 3.2 宏定义

有时我们需要在cmake中在增加宏定义控制，去控制源文件中的一些宏定义

CMakeLists.txt文件

```cpp
option(WWW1 "print one message" OFF)
option(WWW2 "print another message" OFF)

if (WWW1)
    add_definitions(-DWWW1)
endif()

if (WWW2)
    add_definitions(-DWWW2)
endif()
```

那么代码源文件

```cpp
#include <stdio.h>

int main(int argc, char** argv)
{
#ifdef WWW1
    printf("hello world1\n");
#endif    

#ifdef WWW2     
    printf("hello world2\n");
#endif

    return 0;
}
```

这样cmake就可以通过OFF或ON决定源代码中宏定义

```cpp
option(WWW1 "print one message" OFF)
option(WWW2 "print another message" OFF)
```

如果使用cmake ../编译，默认是WWW1和WWW2都是OFF的

也可以编译是直接指定数值，WWW1为ON，WWW2为OFF，如下

```cpp
cmake ../ -DWWW1=ON -DWWW2=OFF
```

## 3.3 设置参数

有时候我们需要在cmake透传给代码一些参数，比如版本号控制等等，下面我们距离以版本号控制为例

```cpp
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)
```

分别指定当前的项目的主版本号和副版本号。

之后，为了在代码中获取版本信息，我们可以修改 config.h文件，添加两个预定义变量：

```cpp
#define Demo_VERSION_MAJOR @Demo_VERSION_MAJOR@
#define Demo_VERSION_MINOR @Demo_VERSION_MINOR@
```



**编写 config.h.in 文件**
上面的程序引用了一个 config.h 文件，如果在这个文件预定义了 USE_MYMATH 的值。但我们并不直接编写这个文件，为了方便从 CMakeLists.txt 中导入配置，我们编写一个 config.h.in 文件，内容如下：

```cpp
#cmakedefine USE_MYMATH
```

这样 CMake 会自动根据 CMakeLists 配置文件中的设置自动生成 config.h 文件。



# 3、多个文件

本章节牵扯到的源文件代码如下

main.cpp如下

```cpp
#include <iostream>
#include "hello.h"
using namespace std;

int main()
{
    helloworld();
    return 0;
}
```

hello.cpp如下

```cpp
#include <iostream>
using namespace std;

int helloworld()
{
    printf("Hello World\r\n");
    return 0;
}
```

hello.h如下

```cpp
#ifndef HELLO_H
#define HELLO_H
int helloworld();
#endif 
```



## 3.1 同级目录

当所有文件都在统计目录中

```cpp
$ tree
.
├── build
├── CMakeLists.txt
├── hello.cpp
├── hello.h
└── main.cpp

1 directory, 4 files
```

在增加一个hello.cpp文件，并在main.cpp调用hello.cpp中函数

CMakeLists.txt文件add_executable修改如下

```cpp
add_executable( HelloWorld main.cpp hello.cpp)
```

省事的方法是使用 aux_source_directory 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：

```cpp
aux_source_directory (<dir> <variable>)
```

CMakeLists.txt文件修改如下

```cpp
aux_source_directory (. DIR_SRCS)
add_executable( HelloWorld ${DIR_SRCS})
```

会将当前目录所有源文件的文件名赋值给变量 DIR_SRCS ，再指示变量 DIR_SRCS 中的源文件需要编译成一个名称为 HelloWorld 的可执行文件。

aux_source_directory()也是有弊端的，它会把指定目录中所有的文件全部添加进入，有些时候不需要添加一些文件，此时我们可以使用set命令去新建变量来存放需要的源文件

```cpp
set (DIR_SRCS ./main.cpp ./hello.cpp)
add_executable( HelloWorld ${DIR_SRCS})
```

## 3.2 多级目录

当某些文件在子目录时

```cpp
$ tree
.
├── build
├── CMakeLists.txt
├── hello
│   ├── hello.cpp
│   └── hello.h
└── main.cpp

2 directories, 4 files
```

CMakeLists.txt文件内容如下

```cpp
# 声明要求的 cmake 最低版本 
cmake_minimum_required( VERSION 2.8 ) 
# 声明一个 cmake 工程 
project( HelloWorld ) 
# 设置编译模式 
set( CMAKE_BUILD_TYPE "Debug" ) 
# 向工程添加多个指定头文件的搜索路径
include_directories (hello)

aux_source_directory (hello SRC_LIST)
aux_source_directory (. DIR_SRCS)
# 添加一个可执行程序 
# 语法：add_executable( 程序名 源代码文件 ） 
add_executable( HelloWorld ${DIR_SRCS} ${SRC_LIST})
```

include_directories。该命令是用来向工程添加多个指定头文件的搜索路径，路径之间用空格分隔，添加多个目录如下

```cpp
#增加hello和test目录作为头文件索引目录
include_directories (hello test)
```

# 4、规范组织结构

一般会把源文件放到src目录下，把头文件放入到include文件下，生成的对象文件放入到build目录下，最终输出的elf文件会放到bin目录下，这样整个结构更加清晰

```cpp
$ tree
.
├── bin
├── build
├── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    ├── hello
    │   ├── hello.cpp
    │   └── hello.h
    └── main.cpp

4 directories, 5 files
```

根目录中的CMakeLists.txt如下

```cpp
# 声明要求的 cmake 最低版本 
cmake_minimum_required( VERSION 2.8 ) 
# 声明一个 cmake 工程 
project( HelloWorld ) 
# 设置编译模式 
set( CMAKE_BUILD_TYPE "Debug" ) 
# 向工程添加存放源文件的子目录
add_subdirectory (src)
```

add_subdirectory()，这个命令可以向当前工程添加存放源文件的子目录，这里要求子目录中也建立一个CMakeLists.txt，如下

```cpp
# 向工程添加多个指定头文件的搜索路径
include_directories (hello)

aux_source_directory (hello SRC_LIST)
aux_source_directory (. DIR_SRCS)
# 添加一个可执行程序 
# 语法：add_executable( 程序名 源代码文件 ） 
add_executable( HelloWorld ${DIR_SRCS} ${SRC_LIST})

# 生成的文件放到根目录的bin文件夹中
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
```

这里又出现一个新的命令set，是用于定义变量的，EXECUTABLE_OUT_PATH和PROJECT_SOURCE_DIR是CMake自带的预定义变量，其意义如下，

EXECUTABLE_OUTPUT_PATH ：目标二进制可执行文件的存放位置
PROJECT_SOURCE_DIR：工程的根目录
所以，这里set的意思是把存放elf文件的位置设置为工程根目录下的bin目录中。

**注意**：EXECUTABLE_OUTPUT_PATH 和PROJECT_SOURCE_DIR必须是大写的




# 5、库文件

编译成动态库或静态库方式调用，新建hello.cpp和hello.h文件，并在main.cpp调用hello.cpp中函数

```cpp
$ tree
.
├── build
├── CMakeLists.txt
├── hello.cpp
├── hello.h
└── main.cpp
```

hello.cpp文件如下

```cpp
#include <iostream>
using namespace std;

int helloworld()
{
    printf("Hello World\r\n");
    return 0;
}
```

hello.h文件如下

```cpp
#ifndef HELLO_H
#define HELLO_H
int helloworld();
#endif 
```

main.cpp文件如下

```cpp
#include <iostream>
#include "hello.h"
using namespace std;

int main()
{
    helloworld();
    return 0;
}
```

CMakeLists.txt文件内容如下

```cpp
# 声明要求的 cmake 最低版本 
cmake_minimum_required( VERSION 2.8 ) 
# 声明一个 cmake 工程 
project( HelloWorld ) 
# 设置编译模式 
set( CMAKE_BUILD_TYPE "Debug" ) 
# 添加一个库 静态库
add_library( hello hello.cpp ) 
# 共享库 动态库
add_library( hello_shared SHARED hello.cpp ) 

# 添加一个可执行程序 
# 语法：add_executable( 程序名 源代码文件 ） 
add_executable( HelloWorld main.cpp )
# 将库文件链接到可执行程序上 
target_link_libraries( HelloWorld hello_shared )
```

上文我们将库生成在默认位置，有些情况我们需要将库生成在指定目录，使用以下命令

```cpp
# 设置库文件的生成位置
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
```

在其他代码调用这个库文件时，可能将库文件放在指定位置，找到对应库命令如下

```cpp
find_library(HELLOFUNC_LIB libhello_shared.so HINTS ${PROJECT_SOURCE_DIR}/lib)
```

find_library: 在指定目录下查找指定库，并把库的绝对路径存放到变量里，其第一个参数是变量名称，第二个参数是库名称，第三个参数是HINTS，第4个参数是路径。

使用find_library的好处是在执行cmake …时就会去查找库是否存在，这样可以提前发现错误，不用等到链接时。



# 6、命令详解

## 6.1 add_subdirectory

**命令格式**

```
add_subdirectory (source_dir [binary_dir] [EXCLUDE_FROM_ALL])
```

添加一个子目录并构建该子目录。

添加一个子目录并构建该子目录。

**命令解析**

1. source_dir

   必选参数。该参数指定一个子目录，子目录下应该包含CMakeLists.txt文件和代码文件。子目录可以是相对路径也可以是绝对路径，如果是相对路径，则是相对当前目录的一个相对路径。

2. binary_dir

   可选参数。该参数指定一个目录，用于存放输出文件。可以是相对路径也可以是绝对路径，如果是相对路径，则是相对当前输出目录的一个相对路径。如果该参数没有指定，则默认的输出目录使用source_dir。

3. EXCLUDE_FROM_ALL

   可选参数。当指定了该参数，则子目录下的目标不会被父目录下的目标文件包含进去，父目录的CMakeLists.txt不会构建子目录的目标文件，必须在子目录下显式去构建。例外情况：当父目录的目标依赖于子目录的目标，则子目录的目标仍然会被构建出来以满足依赖关系（例如使用了target_link_libraries）。



