typedef和 using都是给类型起别名,在使用的过程中有什么需要注意的地方？

- typedef和 using都是给类型起别名,在使用的过程中有什么需要注意的地方？
- typedef int pInt;和using pInt = int;时会是什么样

```
typedef int* pInt;
using pInt = int*;

int main()
{
	int a = 10;
	int b = 10;
	//1、const int * pA = &a; 
	//2、int * const pA = &a;
	//不外乎有上面两种情况，
	//如果是第一种，那么不可以通过pA修改变量b的值，并且pA可以指向&b
	//如果是第二种，那么可以通过pA去修改变量a的值，并且pA不能指向&b
	const pInt pA = &a; //实测可以通过编译，
	*pA = 100;
	cout << a << endl;

	pA = &b;//实测不能通过编译

	return 0;
}
```

通过测试，我们可以看到当有*****存在时const修饰typedef或者using重命名的类型时，const只修饰的其实是一个常量指针。

注意：很多人在这个地方可能会认为是上面所说的第一种，做简单的文本替换，这种理解是错误的，因为const 修饰的对象是pA，相当于pInt const pA 这样就很好理解了



**指针函数**

下面两种定义是等价的

```
using DockFuncPtr = void (FinalReturn::*)();
DockFuncPtr func_ptr = nullptr;
```

```
typedef void (FinalReturn::*DockFuncPtr )();
DockFuncPtr func_ptr = nullptr;
```

