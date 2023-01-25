//
// Created by luqi_ on 2023/1/12.
//

#ifndef BIGDATA_BIGINTEGER_H
#define BIGDATA_BIGINTEGER_H

#include<iostream>
#include<vector>
#include<cstring>

//低32位掩码
#define LOWMARK (0xffffffff)

using namespace std;

class BigInteger {
private:
    //存放二进制数据的容器
    vector<::uint32_t> data;
    //是否是负数
    bool isNegative;

    //实际的减法运算
    static void _minus_bigInteger(BigInteger& result,const BigInteger& num);


    //实际的加法逻辑
    static void _add_bigInteger(BigInteger& result,const BigInteger& num);

    //实际的乘法逻辑
    static void _multiply_bigInteger(BigInteger& result,const BigInteger& num1,const BigInteger& num2);

    //实际的除法逻辑
    static void _divided_bigInteger(BigInteger* quotient,BigInteger* remainder,
                                    const BigInteger& dividend,const BigInteger& divisor);



    //清空前导0，辅助计算
    static void _clear_last_zero(BigInteger& big);

    //位与运算的实现
    static void _and(BigInteger &result,size_t len, const BigInteger &maxBig);

    //位或运算的实现
    static void _or(BigInteger &result, size_t len, const BigInteger &minBig);

    //位异或运算的实现
    static void _xor(BigInteger &result, size_t len, const BigInteger &minBig);

    //按位取反运算的实现
    static void _not(BigInteger& result);

    //位右移的实现
    static void _right_move(BigInteger& result,::uint64_t num);

    //位左移的实现
    static void _left_move(BigInteger& result,::uint64_t num);


public:
    /**
     * 默认初始化，构造一个值为0的对象
     */
    BigInteger();

    /**
     * 字符串初始化，将会对传入的字符串分解为二进制数据
     * 字符串的格式规定如下：
     * 1、字符串开头的一位可以为‘+'或'-'或0-9的数字
     * 当不传入符号时，默认为非负数
     * 2、从第2个字符之后的所有字符，都为0-9的数字字符
     *
     * 当字符串违背以上规定的任何一个时，都将被构造为
     * 值为0的对象
     * @param number 传入的10进制字符串
     */
    explicit BigInteger(const char* number);

    /**
     * 传入一个64位的无符号整型数据以及一个负数标记位进行构造
     * 其构造的数据即为整数表示的值，其符号位即为传入的值
     * @param data 64位无符号整数
     * @param isNegative 负数标记位，当整数为负数，则为true，否则为false
     */
    BigInteger(uint64_t data,bool isNegative);

    /**
     * 传入一个无符号的32位整数数组进行构造，其值为该数组所对应的数据
     * @param data 传入的32位无符号整型数组
     * @param len 数组的长度
     * @param isNegative 是否是负数的标记位
     */
    BigInteger(::uint32_t data[],size_t len,bool isNegative);

    //拷贝初始化
    BigInteger(const BigInteger& number);

    /**
     * 检测一个字符串是否是合法的数字形式
     * @param str 待判断的字符串
     * @param hasToken 用于接收字符串是否包含符号位
     * @param isNegative 用于接收符号位
     * @return 合法则返回字符串的长度，否则返回0
     */
    static size_t isNumberString(const char* str,bool* hasToken,bool* isNegative);

    //判断一个字符是否是数字
    static bool IsNumber(char ch);

    //默认析构
    ~BigInteger();

    //获取等价的10进制数字符串
    //需要手动释放返回的指针的内存
    [[nodiscard]] char* toString()const;

    /**
     * 获取等价的32位无符号整数数组
     * @param len 用于获取数组的长度
     * @return 常量数组的指针
     */
    [[nodiscard]] const ::uint32_t * toBinaryArray(::uint64_t * len)const;

    /**
     * 获取内部32位整型数据的长度
     * @return
     */
    [[nodiscard]] ::uint64_t length()const;

    /**
     * 执行除法运算，同时获取余数
     * @param divisor 除数
     * @param remainder 获取余数的对象
     * @return 商数
     */
    BigInteger dividedAndRemainder(const BigInteger& divisor,BigInteger& remainder)const;

    /**
     * 将自身进行正负号取反
     * @return 返回经过取反之后的值
     */
    BigInteger& negate();

    //重载的加减乘除运算
    BigInteger operator+(const BigInteger& num)const;
    BigInteger operator-(const BigInteger& num)const;
    BigInteger operator*(const BigInteger& num)const;
    BigInteger operator/(const BigInteger& num)const;
    BigInteger operator%(const BigInteger& num)const;

    //重载的取负号运算符
    BigInteger operator-()const;

    //重载的加减乘除运算符
    BigInteger& operator+=(const BigInteger& num);
    BigInteger& operator-=(const BigInteger& num);
    BigInteger& operator*=(const BigInteger& num);
    BigInteger& operator/=(const BigInteger& num);
    BigInteger& operator%=(const BigInteger& num);

    BigInteger& operator++();
    BigInteger operator++(int);

    BigInteger& operator--();
    BigInteger& operator--(int);

    BigInteger& operator=(const BigInteger& num);

    //大小判断
    bool operator<(const BigInteger& num)const;
    bool operator<=(const BigInteger& num)const;
    bool operator>(const BigInteger& num)const;
    bool operator>=(const BigInteger& num)const;
    bool operator==(const BigInteger& num)const;
    bool operator!=(const BigInteger& num)const;

    //绝对值大于
    [[nodiscard]] bool absGreaterThan(const BigInteger& num)const;

    //绝对值大于等于
    [[nodiscard]] bool absGreaterEqual(const BigInteger& num)const;

    //绝对值小于
    [[nodiscard]] bool absLessThan(const BigInteger& num)const;

    //绝对值小于等于
    [[nodiscard]] bool absLessEqual(const BigInteger& num)const;

    //绝对值等于
    [[nodiscard]] bool absEqual(const BigInteger& num)const;

    //绝对值不等于
    [[nodiscard]] bool absNotEqual(const BigInteger& num)const;

    //判断是否为0
    [[nodiscard]] bool isZero()const;

    //设置为0
    void setZero();

    //重置默认类型转换
    explicit operator bool()const;

    //重置数值类型转换
    explicit operator ::uint64_t ()const;

    //位运算的实现
    bool operator !()const;
    BigInteger operator &(const BigInteger& num)const;
    BigInteger operator |(const BigInteger& num)const;
    BigInteger operator ^(const BigInteger& num)const;
    BigInteger operator ~()const;

    BigInteger& operator |=(const BigInteger& num);
    BigInteger& operator &=(const BigInteger& num);
    BigInteger& operator ^=(const BigInteger& num);

    BigInteger operator<<(::uint64_t num)const;
    BigInteger operator>>(::uint64_t num)const;

    BigInteger& operator>>=(::uint64_t num);
    BigInteger& operator<<=(::uint64_t num);

};


#endif //BIGDATA_BIGINTEGER_H
