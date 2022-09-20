可以看到，原来使用for_each算法需要定义一个函数来作为参数进行计算。用了lambda之后，代码结构更加清晰和简单。不过，针对这个例子使用C++11中的for循环更加方便，但是如果循环中的算法更加复杂，可能用lambda+for_each会比较好

# 1、C过渡到C++

C中的定义struct

```
struct A
{
	int a;
	int b;
	//成员列表
};
```

> 注意：因为struct是一种数据类型，那么就肯定不能定义函数，所以在面向c的过程中，struct不能包含任何函数。否则编译器会报错

面向过程的编程认为，数据和数据操作是分开的。然而当struct进入面向对象的c++时，其特性也有了新发展，就拿上面的错误函数来说，在c++中就能运行，因为在c++中认为数据和数据对象是一个整体，不应该分开，这就是struct在c和c++两个时代的差别。

在C++中struct得到了很大的扩充：

1.struct可以包括成员函数
2.struct可以实现继承
3.struct可以实现多态

# 2、struct和class的区别

1.默认的继承访问权。class默认的是private,strcut默认的是public。

2.默认的继承访问权。class默认的是private,strcut默认的是public。







