# BigInteger

这是一个基于二进制的大数运算C++实现。

其内部采用的是一个无符号32位整数vector存放所有的二进制数据，能够支持基本的加法、减法、乘法、除法、取余运算，以及全部的逻辑运算、位运算。

在进行具体运算时，使用重载的运算符实现。



*以下对BigInteger类的对象简称“大数”*

# 构造函数

其有多个构造函数的版本，具体版本的用法如下

## BigInteger()

默认构造函数，将构造一个值是0的大数

## BigInteger(const char* cstr)

字符串构造函数，将根据传入的字符串所对应的具体10进制数，将其转换成底层的二进制数据进行存储。具体的字符串规则如下：

1. 字符串开头可以是0-9的数字字符或'+'号，或'-'号，若是'-'号，则表示其是一个负数
2. 除开头字符以外，其余字符必须是0-9的数字字符。不能够有任何空白字符。

当字符规则不满足以上所有条件时，将构造成值是0的大数。

以下都是合法的构造字符串的实例

- "002312131"
- "+0"
- "-23"
- "-0"
- "+0123"

## BigInteger(uint64_t num,bool isNegative)

此构造函数接收一个无符号的64位整型数据，将其转换为内部二进制数据存储，还包括一个负数标志位，当其是负数时，传入true。



## BigInteger(uint32_t binaryArray,size_t len,bool isNegative)

此构造函数接收一个32位无符号整型数组作为大数的内部数据存储，该数组长度时len，当大数应该是负数时，isNegative传入true。



## BigInteger(const BigInteger& big)

此构造函数接收另一个大数，将其拷贝



# 信息转换函数

BigInteger可以将其内部的二进制数据转换为各种信息，具体函数说明如下。

## char* toString()

该函数获取大数对于的10进制字符串表示，需要注意的是该指针指向堆内存，需要手动释放。



## const uint32_t * toBinaryArray(uint64_t * len)

该函数返回大数内部的32位无符号整型数据的数组，并将其长度存放到len指针中。该数组是const的，不可以释放以及修改返回指针内存。



## uint64_t length()

该函数获取大数内部的32位无符号整型数组的长度



## bool isZero()

判断该大数是否为0



## void setZero()

将该大数设置为0



# 算数、逻辑、位运算

BigInteger支持所有的算数运算、逻辑运算、位运算。

算数运算包括：+、-、*、/、%、+=、-=、\*=、/=、%=、-（取负数）、++、--。

逻辑运算符包括：<、<=、>、>=、!=、==、!

位运算符包括：<<、<<=、>>、>>=、&、|、^、~、&=、|=、^=

其与C++中的运算符作用一样。

除以上运算外，还增加了一些使用的函数

## BigInteger& negate()

该函数的作用与取负数运算符'-'一样，区别在于作用的对象是自身，而不是返回一个新拷贝的经过取负数之后的大数。



## BigInteger dividedAndRemainder(const BigInteger& divisor,BigInteger& remainder)

该函数可以同时获取余数以及商。将商作为返回值，将余数作为引用传递。

divisor是除数的大数

remainder用以接收余数



## 绝对值逻辑运算

所有以abs开头的函数，其具体的名称代表了逻辑运算的类型，但是是基于大数本身的绝对值进行的逻辑判断。

例如：absGreaterEqual，表示该函数做的判断是 Greater Equal（大于等于）。



## 类型转换

大数支持bool类型以及无符号64位整数类型的转换，需要注意的是当大数超过2个uint32的长度后，64位无符号转换将会被截断。



# 实例程序

以下包含一段演示基本运算的程序

```c++
#include<iostream>
#include"BigInteger.h"



int main(){
    BigInteger i1("123456");
    BigInteger zero;
    BigInteger i2(998013213,false);
    
    //获取乘法运算结果
    BigInteger result1=i1*i2;
    
    //获取加法结果
    BigInteger result2=i1+i2;
    
    //在i1上加上i2
    i1+=i2;
    
    //...依次类推
    
    //打印二进制数据表示的具体10进制数字
    std::cout<<result1.toString()<<std::endl;
}

```

其他用法详情见BigInteger.h头文件
