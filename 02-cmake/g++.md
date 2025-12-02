

g++命令选项字

> 选项字和后面内容之间可以有空格，也可以没有空格。-L/include和-L /include是一样的效果

-L 用于指定库文件的搜索路径，在链接时，编译器需要在文件系统中搜索库文件，默认情况下会在系统预定义的库路径中搜索，比如/usr/lib等，但是有些库不在默认路径中，这时使用-L制定库路径

-l 用于指定链接时需要的库文件，后面根库名，例如需要链接的是libexample.a，则是-l libexamole  注意：库名不需要包含前缀"lib"和文件扩展名

> -l dl dl是一个库的名字。dl是linux系统中动态加载库的缩写，这个库提供了在运行时的动态加载和卸载共享库的功能，这个库的全名通常时libdl.so

-I 用于指定编译时搜索头文件的路径，实际使用中可以制定输入的源文件

-c 用于表示只进行编译，不进行链接，当存在这个选项时，编译器会将C++源码文件编译成目标文件(.o文件)，但不会生成可执行文件


makefile

``` cpp
TARGET:=demo

OBJECTS:=app_main.o

CXXFLAGS:=-I$(HOME)/.tbox/include
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic
LIBS:=\
	-ltbox_main \

# $@ 表示目标文件的名称，也就是(TARGET)的值。
# $^ 表示所有依赖文件的列表，也就是(OBJECTS)的值

$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)
```
由于目标文件依赖app_main.o，所以需要编译app_main.o，展开后如下
``` cpp
g++ -I/home/chenjd/.tbox/include   -c -o app_main.o app_main.cpp
```

