# 基础

const主要用于定义一个**只能读取，不能写入**的变量。
``` cpp
const int a =10; //定义const的int型变量a
int b = 0;
b = a; //const变量可以被访问
const int c = a; // 正确
a = 2； //错误！！！const变量不能被写入
const int d; // 错误！！！const变量必须被初始化
```
并且 const 和int书写没有前后之分

下面两种写法是一样的
``` cpp
const int a =10;
int const a =10;
```
const和指针
``` cpp
const int a = 10;
int b = 20;
/* const修饰的是*c，也就是指针c指向的int类型数据，所以c指向的int数据不能被修改，但是指针变量c本身的值是可以修改的，也就是c可以指向的变量 */
int const *c = &a;
c = &a; // 正确
c = &b; // 正确
// *c = 4; // 错误！！！

/* const修饰的是变量d，变量d是指针，所以指针变量是不能修改的，但是d指向的int类型的数据是可以修改的 */
int *const d = &b; //
// int *const d = &a; //错误，int *const d 定义的指针d不能修改，但是指向的int型数据可以修改的，也就是a可以修改，但是a又是const型的不能修改，所以冲突，编译器会报错
*d = 20;
// d = &b;//错误，const必须初始化时被赋值

const int *e = &a;// const 修饰的是*e,所以和int const *c = &a;是一样的，故认为 const 和 int 的前后顺序是可以换的

// const *int f = &b; // *在int前面是不合法的，所以会报错

const double pi =3.14;
const double *const pip = &pi;// pip是一个指向常量对象的常量指针
```

这里有常量指针和指针常量的区别
``` cpp
int const *c = &a;//常量指针
int *const d = &b;//指针常量
```
他们俩快速区别：谁在前谁不能被修改。
- 常量指针，常量不能被修改
- 指针常量，指针不能被修改

# const的引用

可以把引用绑定到const对象上，就像绑定到其他对象上一样，我们称之为**对常量的引用(reference to const)**,与普通引用不同的是，对常量的应用不能被用作修改它所绑定的对象
``` cpp
const int ci = 1024;
const int &r1 = ci;//正确，引用及其对应的对象都是常量
r1 = 42;// 错误 r1是对常量的引用
int &r2 = ci;//错误 试图让一个非常量引用指向一个常量对象

int i=42;
const int &r3 = i;//正确的，允许将const int &绑定到一个普通int对象上
```
因为不允许直接为ci赋值，当然也就不能通过引用去改变ci。因此，r2的初始化是错误的。如果次初始化合法，那么就可以用通过r2修改他引用对象的值，也就是修改ci的值，但是ci是const的变量不能修改的。

# 数据类型强制转换
举个例子
``` cpp
double dval = 3.14;
const int &ri = dval;
``` 
dval是双精度的数据类型，而却让它绑定到一个整型ri的引用上，编译器会把上述代码变成
``` cpp
double dval = 3.14;
const int temp = dval;//将double类型的变量临时赋值给临时const整型变量
const int &ri = temp;//让ri绑定到这个临时变量上
```
那么下面讨如果ri不是常量

如果ri不是常量，就允许给ri赋值，这样也就可以改变ri所引用对象的值。但是编译器会生成临时变量temp，这个时候ri绑定是的temp，而不是dval。而temp是const类型的变量，所以C++也就把这种行为归为非法行为

编译报错如下
``` cpp
$ g++ demo.cpp 
demo.cpp: In function ‘int main()’:
demo.cpp:39:15: error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
   39 |     int &ri = dval;
```
无法用"double"类型的值初始化"int &“类型的引用(非常量限定)

# constexpr和常量表达式

**常量表达式(const expression)**指的是：值不会改并且编译一过程中就能得到计算记过的表达式，显然字面值属于常量表达式，用常量表达式初始化的const对象也就是常量表达式。

一个对象或表达式是不是常量表达式由它的数据类型和初始值共同决定
``` cpp
const int max_files = 20;//max_files是常量表达式
const inr limit = max_files + 1;//limit 是常量表达式
int staff_size = 27;//staff_size不是常量表达式
const int az = get_size();//sz不是常量表达式
```
尽管staff_zise的初始值是个字面值常量，但由于它的数据类型只是一个普通的int而不是const int，多一它不属于常量表达式。另一方面，sz尽管本身是一个常量，但它的具体值指导运行时才能获得，所以也不是常量表达式

**constexpr变量**

在一个复杂系统中，很难（几乎肯定不能）分辨一个初始值到底是不是常量表达式。当然可以用定义一个const变量并把它的初始值作为我们认为的某个常量表达式，但在实际使用中，尽管要求如此，却常常发现初始值并非常量表达式的情况，可以这么说，在这种情况下，对象的定义和使用根本是两回事。

C++11新标准规定，允许变量声明为constexpr类型以便由编译器来验证变量的值是否是一个常量表达式，声明为constexpr的定量一定是一个常量，而且必须用常量表达式初始化
``` cpp
constexpr int mf = 20;//20是一个常量表达式
constexpr int limit = mf +1;// mf + 1 是一个常量表达式
constexpr int z = size();//只有当size()是一个constexpr 函数时才是一条正确的声明语句
```
尽管不能使用普通函数作为constexpr变量的初始值，但新标准允许定义一种特殊的constexpr函数。这种函数应该足够简单以使得编译时就可以计算其结果，这样就能用constexpr函数区初始化constexpr变量了。

> 一般来说，如果你认定变量是一个常量表达式，那就把它声明成constexpr类型

**字面值类型**

常量表达式的值时需要在编译时就可以得到计算，因为对声明constexpr时用到的类型必须时有所限制的。因为这些类型一般比较简单，值也显而易见，容易得到的，就把他称为“字面值类型”(literal type)

算术类型，引用和指针都属于字面值类型。自定义类sales_item，IO库，string类型则不属于字面值类型，也就不能被定义为constexpr。

尽管指针和引用都能被定义为constexpr，但他们的初始值的受到严格限制。一个constexpr指针的初始值必须时nullptr或者0，或者存储在某个固定地址中的对象。

函数体内的变量一般来说并非存放在固定地址中，因此constexpr指针不能指向这样的变量。相反的，定义与函数体外的对象，它的地址固定不变，可以使用用来初始化constexpr指针。函数体内允许定义一类有效范围超过函数本身的变量，比如函数体内static变量，这类变量和函数体外的变量一样都有固定的地址，因为constexpr引用可以绑定到这样的变量上，constexpr指针也可以指向这样的变量。

**指针和constexpr**

必须声明一点，在connstexpr声明中如果定义一个指针，限定符constexpr仅对指针有效，与指针所值的对象无关
``` cpp
const int *p = nullptr;// p是一个指向整型常量的指针
constexpr int *q = nullptr;// q是一个指向整数的常量指针
```
p和q的类型相差甚远，p是一个指向常量的指针，而q是一个常量指针。

与其他常量指针类似，constexpr指针既可以指向常量也可以指向一个非常量
``` cpp
constexpr int *np = nullptr;//np是一个指向整数的常量指针，其值为空
int j = 0;
constexpr int i = 42;//i的类型时整型常量
// i和j都必须定义在函数体外
constexpr const int *p = &i; // p是常量指针，指向整型常量i
constexpr int *pl = &j; // pl是常量指针，指向整数j
```

# const修饰成员函数

const成员函数的写法有两种
``` cpp
void fun(int a，int b) const{}
void const fun(int a,int b){}
```
这两种写法的本质是：void fun (const 类 *this, int a,int b);

const修饰的不是形参a和b；const修饰的是属性this->a和this->b。与const所写的位置无关。

为什么？

因为c++对类的this指针做了隐藏，本质上，const指针修饰的是被隐藏的this指针所指向的内存空间，修饰的是this指针。决定了在该成员函数中，任意修改它所在的类的成员的操作都是不允许的（因为隐含了对this指针的const引用）；

 

总结：

1.const成员函数可以访问非const对象的非const数据成员、const数据成员，也可以访问const对象内的所有数据成员；

2.非const成员函数可以访问非const对象的非const数据成员、const数据成员，但不可以访问const对象的任意数据成员；

3.const修饰的成员函数**只能**调用const修饰的成员函数，且不能修改数据成员变量的值。如果后面加了const的函数内部又需要对某变量进行修改，可以使用mutable关键字修饰成员变量。

4.加了const的成员函数可以被非const对象和const对象调用；

5.加const的成员函数只能被非const对象调用

# 例子

典型例子
``` cpp
inline const Cell &cell(const Pint &P) const;
```
第一个const关键字表示函数返回值是一个常量引用。const Cell &表示返回一个类型为cell的常量引用，这意味着函数函数返回的引用是只读的。

第二个const关键字位于函数末尾，表示这是一个const函数，这个函数不能修改对象的任何成员变量。

总结起来：这个函数声明表示它返回一个只读的Cell对象引用，并且这个函数本身不能修改对象的状态，也就是不能修改对象的任何成员变量。







