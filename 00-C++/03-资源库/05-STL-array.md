array<T,N> 模板定义了一种相当于标准数组的容器类型。它是一个有 N 个 T 类型元素的固定序列。除了需要指定元素的类型和个数之外，它和常规数组没有太大的差别。显然，不能增加或删除元素。

模板实例的元素被内部存储在标准数组中。和标准数组相比，array 容器的额外幵销很小，但提供了两个优点：如果使用 at()，当用一个非法的索引访问数组元素时，能够被检测到，因为容器知道它有多少个元素，这也就意味着数组容器可以作为参数传给函数，而不再需要单独去指定数组元素的个数。

# 1、模板类声明

array模板类声明如下

```
template<class T, size_t N> class array;
```

# 2、迭代器遍历

begin：返回容器中的第一个元素的迭代器

end：返回容器中的最后一个元素的迭代器

rbegin：返回指向最后一个元素的迭代器

rend：返回指向第一个元素之前的迭代器

```
#include<iostream>
#include<array>

int main()
{
	std::array<int, 4> arr = { 1, 3, 2, 4 };
	std::cout << "arr values:" << std::endl;
	for (auto it = arr.begin(); it != arr.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cin.get();
	return 0;
}
```

运行结果

arr values:
1 3 2 4

# 2、获取array的大小

size：返回容器当前元素的数量

max_size：返回容易可容纳的最大数据

empty：判断容器是否为空

```
#include<iostream>
#include<array>

int main()
{
	std::array<int, 4> arr = { 1, 3, 2, 4 };
	std::cout << "sizeof(arr) = " << sizeof(arr) << std::endl;
	std::cout << "size of arr = " << arr.size() << std::endl;
	std::cout << "max size arr = " << arr.max_size() << std::endl;
	std::cout << "empty = " << (arr.empty() ? "yes" : "no") << std::endl;
	std::cin.get();
	return 0;
}
```

运行结果

sizeof(arr) = 16
size of arr = 4
max size arr = 4
empty = no

# 3、array元素操作

at：返回容器在n处的引用，有错误检测

front：返回容器的第一个元素的引用

back：返回容器的最后一个元素的引用

data：返回指向首个元素的指针

fill：填充元素

swap：交换容器的元素

```
#include<iostream>
#include<array>

int main()
{
	std::array<int, 4> arr = {};
	for (int i = 0; i < arr.size(); i++)
	{
		arr.at(i) = i;
	}
	for (auto it = arr.begin(); it != arr.end(); it++)
	{
		std::cout << *it << " " ;
	}
	std::cout << std::endl;
	std::cout << arr.data() << std::endl;
	std::array<int, 4> arr1 = { 4, 5, 6,7 };
	arr1.swap(arr);
	for (auto it = arr.begin(); it != arr.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	arr.fill(1);
	for (auto it = arr.begin(); it != arr.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cin.get();
	return 0;
}
```

运行结果

0 1 2 3
0x7fff93bfda20
4 5 6 7
1 1 1 1

# 4、用array构建多维数组

构建多维数组

```
#include<iostream>
#include<array>

int main()
{
	std::array <std::array <int, 4>, 5> arr = {};
	arr.fill({ 1, 2,3,4 });
	std::cout << arr.size() << std::endl;
	std::cout << arr[0].size() << std::endl;
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < arr[0].size(); j++) {
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cin.get();
	return 0;
}
```

运行结果

5
4
1 2 3 4
1 2 3 4
1 2 3 4
1 2 3 4
1 2 3 4