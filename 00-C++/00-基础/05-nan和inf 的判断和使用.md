# 引言

在数据处理输入输出时，极有可能遇到数据读入空值（极大、极小）、运算中分母为0或0.0，对0取对数等操作，这将产生nan或inf的产生。这篇博文旨在分析C/C++产生nan和inf的操作及判断是否有nan或inf产生。

# NAN的产生原因

nan: not a number，表示“无效数字”。

1. 对负数开方，例如：exp(-10)
2. 对负数求对数，例如：log(-10)
3. 除数是0，例如：0/0
4. 0乘以无穷大的数(在数学上貌似可以的啊)
5. 无穷大的数除以无穷大的数据
6. 无穷大的数减去无穷大的数据

注意：nan是无序的（unordered），无法对其进行逻辑运算。它不大于、小于或等于任何数（包括它自己），将<，>，<=，和>=作用于nan产生一个exception。得到nan时就查看是否有非法操作，如果表达式中含有nan，那么表达式的结果为nan。



# INF的产生原因

INF：infinite，表示“无穷大”。
超出浮点数的表示范围（溢出，即阶码部分超过其能表示的最大值）

1. 1除以0，为inf，例如：1/0
2. -1除以0，为-inf
3. 0加无穷大的数
4. log(0)

注意：+inf大于任何数（除了它自己和nan），-inf小于任何数（除了它自己和nan），得到inf时就查看是否有溢出或者除以0。inf在C语言表达式中就表示数学里无限的概念，如1.0/inf等于0.0，并可以与其他浮点数进行比较的（可以参与<=、>+、==、!=等运算）。



## nan和inf的判断

## C语言中

下面几个宏即包含在math.h头文件，可用于判断一个表达式的结果是否为inf、nan或其他。使用时包括include<math.h>

```c++
int isfinite(x);
int isnormal(x);
int isnan(x);
int isinf(x);
```

使用方法，及结果：

1. int isfinite(x) ，判断x是否有限，是返回1，其它返回0；
2. int isnormal(x)，判断x是否为一个数（非inf或nan），是返回1，其它返回0；
3. int isnan(x)，当x时nan返回1，其它返回0；
4. int isinf(x) ,当x是正无穷是返回1，当x是负无穷时返回-1，其它返回0。有些编译器不区分。

示例代码

```c++
#include <iostream>
#include <math.h>
using namespace std;

int main(int argc, char *argv[])
{
    //nan
    cout<<"nan: "<<sqrt(-1)<<endl;
    cout<<"nan: "<<log(-1.0)<<endl;
    cout<<"nan: "<<0.0/0.0<<endl;
    cout<<"nan: "<<0.0*sqrt(-1)<<endl;
    cout<<"nan: "<<sqrt(-1)/sqrt(-1)<<endl;
    cout<<"nan: "<<sqrt(-1)-sqrt(-1)<<endl;

    //inf
    cout<<"inf: "<<1/0.0<<endl;
    cout<<"-inf: "<<-1/0.0<<endl;
    cout<<"inf: "<<0.0+1/0.0<<endl;
    cout<<"-inf: "<<log(0)<<endl;

    cout<<"isfinite: 0"<<isfinite(0.0/0.0)<<endl;
    cout<<"isfinite: 0"<<isfinite(1/0.0)<<endl;
    cout<<"isfinite: 1"<<isfinite(1.1)<<endl;

    cout<<"isnormal: 0"<<isnormal(0.0/0.0)<<endl;
    cout<<"isnormal: 0"<<isnormal(1/0.0)<<endl;
    cout<<"isnormal: 1"<<isnormal(1.1)<<endl;

    cout<<"isnan: 1"<<isnan(0.0/0.0)<<endl;
    cout<<"isnan: 0"<<isnan(1/0.0)<<endl;
    cout<<"isnan: 0"<<isnan(1.1)<<endl;

    cout<<"isinf: 0"<<isinf(0.0/0.0)<<endl;
    cout<<"isinf: 1"<<isinf(1/0.0)<<endl;
    cout<<"isinf: 0"<<isinf(1.1)<<endl;


    return 0;
}
```

## C++中

详细看 std::isfinite 文章

