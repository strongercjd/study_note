
auto_ptr 同样是 STL 智能指针家族的成员之一，由 C++98 引入，定义在头文件<memory>。其功能和用法类似于 unique_ptr，由 new expression 获得对象，在 auto_ptr 对象销毁时，他所管理的对象也会自动被 delete 掉。

auto_ptr 从 C++98 使用至今，为何从 C++11 开始，引入 unique_ptr 来替代 auto_ptr 呢？原因主要有如下几点：

（1）基于安全考虑。
先来看下面的赋值语句:

``` cpp
auto_ptr< string> ps (new string ("I reigned lonely as a cloud.”）;
auto_ptr<string> vocation; 
vocaticn = ps;
```

上述赋值语句将完成什么工作呢？如果 ps 和 vocation 是常规指针，则两个指针将指向同一个 string 对象。这是不能接受的，因为程序将试图删除同一个对象两次，一次是 ps 过期时，另一次是 vocation 过期时。要避免这种问题，方法有多种：

- 定义陚值运算符，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本，缺点是浪费空间，所以智能指针都未采用此方案。
- 建立所有权（ownership）概念。对于特定的对象，只能有一个智能指针可拥有，这样只有拥有对象的智能指针的析构函数会删除该对象。然后让赋值操作转让所有权。这就是用于 auto_ptr 和 unique_ptr 的策略，但 unique_ptr 的策略更严格。
- 创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为引用计数。例如，赋值时，计数将加 1，而指针过期时，计数将减 1,。当减为 0 时才调用 delete。这是 shared_ptr 采用的策略。

当然，同样的策略也适用于复制构造函数，即auto_ptr<string> vocation(ps)时也需要上面的策略。每种方法都有其用途，但为何要摒弃 auto_ptr 呢？

下面举个例子来说明。

``` cpp
#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main() {
	auto_ptr<string> films[5] ={
		auto_ptr<string> (new string("Fowl Balls")),
		auto_ptr<string> (new string("Duck Walks")),
		auto_ptr<string> (new string("Chicken Runs")),
		auto_ptr<string> (new string("Turkey Errors")),
		auto_ptr<string> (new string("Goose Eggs"))
	};
    auto_ptr<string> pwin;
    pwin = films[2]; // films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针

	cout << "The nominees for best avian baseballl film are\n";
	for(int i = 0; i < 5; ++i) {
		cout << *films[i] << endl;
	}
 	cout << "The winner is " << *pwin << endl;
	return 0;
}

```

运行下发现程序崩溃了，原因在上面注释已经说的很清楚，films[2] 已经是空指针了，下面输出访问空指针当然会崩溃了。但这里如果把 auto_ptr 换成 shared_ptr 或 unique_ptr 后，程序就不会崩溃，原因如下：

使用 shared_ptr 时运行正常，因为 shared_ptr 采用引用计数，pwin 和 films[2] 都指向同一块内存，在释放空间时因为事先要判断引用计数值的大小因此不会出现多次删除一个对象的错误。

使用 unique_ptr 时编译出错，与 auto_ptr 一样，unique_ptr 也采用所有权模型，但在使用 unique_ptr 时，程序不会等到运行阶段崩溃，而在编译期因下述代码行出现错误：

``` cpp
unique_ptr<string> pwin;
pwin = films[2]; 					// films[2] loses ownership

```

指导你发现潜在的内存错误。这就是为何要摒弃 auto_ptr 的原因，一句话总结就是：避免因潜在的内存问题导致程序崩溃。

从上面可见，unique_ptr 比 auto_ptr 更加安全，因为 auto_ptr 有拷贝语义，拷贝后原对象变得无效，再次访问原对象时会导致程序崩溃；unique_ptr 则禁止了拷贝语义，但提供了移动语义，即可以使用 std::move() 进行控制权限的转移，如下代码所示：

``` cpp
unique_ptr<string> upt(new string("lvlv"));
unique_ptr<string> upt1(upt);	//编译出错，已禁止拷贝
unique_ptr<string> upt1=upt;	//编译出错，已禁止拷贝
unique_ptr<string> upt1=std::move(upt);  //控制权限转移

auto_ptr<string> apt(new string("lvlv"));
auto_ptr<string> apt1(apt);	//编译通过
auto_ptr<string> apt1=apt;	//编译通过

```

这里要注意，在使用 std::move 将 unique_ptr 的控制权限转移后，不能够再通过 unique_ptr 来访问和控制资源了，否则同样会出现程序崩溃。我们可以在使用 unique_ptr 访问资源前，使用成员函数 get() 进行判空操作。

``` cpp
unique_ptr<string> upt1=std::move(upt);	// 控制权限转移
if(upt.get()!=nullptr) {				// 判空操作更安全
	// do something
}

```

2）unique_ptr 不仅安全，而且灵活。
如果 unique_ptr 是个临时右值，编译器允许拷贝语义。参考如下代码：

``` cpp
unique_ptr<string> demo(const char* s) {
    unique_ptr<string> temp (new string(s))； 
    return temp；
}
// 假设编写了如下代码
unique_ptr<string> ps;
ps = demo('Uniquely special")；
```

demo() 返回一个临时 unique_ptr，然后 ps 接管了临时对象 unique_ptr 所管理的资源，而返回时临时的 unique_ptr 被销毁，也就是说没有机会使用 unique_ptr 来访问无效的数据，换句话来说，这种赋值是不会出现任何问题的，即没有理由禁止这种赋值。实际上，编译器确实允许这种赋值。相对于 auto_ptr 任何情况下都允许拷贝语义，这正是 unique_ptr 更加灵活聪明的地方。

（3）扩展 auto_ptr 不能完成的功能。

unique_ptr 可放在容器中，弥补了 auto_ptr 不能作为容器元素的缺点。

``` cpp
// 方式一
vector<unique_ptr<string>> vs { new string{“Doug”}, new string{“Adams”} };  

// 方式二
vector<unique_ptr<string>>v;  
unique_ptr<string> p1(new string("abc"));  
```

管理动态数组，因为 unique_ptr 有 unique_ptr<X[]> 重载版本，销毁动态对象时调用 delete[]。

``` cpp
unique_ptr<int[]> p (new int[3]{1,2,3});  
p[0] = 0;// 重载了operator[]
```

自定义资源删除操作（Deleter）。unique_ptr 默认的资源删除操作是 delete/delete[]，若需要可以进行自定义。

``` cpp
// 资源清理函数
void end_connection(connection *p) { disconnect(*p); }

// 资源清理器的类型
unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);// 传入函数名，会自动转换为函数指针
```

综上所述，基于 unique_ptr 的安全性和扩充的功能，unique_ptr 成功的将 auto_ptr 取而代之。