//
// Created by luqi_ on 2023/1/12.
//

#include "BigInteger.h"

//倍数的数值形式
static const ::uint64_t TenNumbers[]={4294967296,8589934592,12884901888,
                                      17179869184,21474836480,25769803776,
                                      30064771072,34359738368,38654705664};


BigInteger::BigInteger() {
    data.push_back(0);
    isNegative=false;
}


/**
 * 将字符串num2加到num1上
 * @param num1 被加数
 * @param num2 加数
 */
static void CharPlusChar(vector<char>& num1,vector<char>& num2){
    size_t i;
    int jin=0;
    int sum;
    for(i=0;i<num2.size() && i<num1.size();++i){
        sum=jin+num2[i]-'0'+num1[i]-'0';
        if(sum>=10){
            jin=1;
            sum-=10;
        }
        else
            jin=0;
        num1[i]=sum+'0';
    }

    while(i<num1.size()){
        sum=jin+num1[i]-'0';
        if(sum>=10)
            sum-=10;
        else
            jin=0;
        num1[i]=sum+'0';
        ++i;
    }
    while(i<num2.size()){
        sum=jin+num2[i]-'0';
        if(sum>=10)
            sum-=10;
        else
            jin=0;
        num1.push_back(sum+'0');
        ++i;
    }
    if(jin!=0)
        num1.push_back('1');
}

/**
 * 在一个数组上乘上一个个位数
 * @param num 数字数组
 * @param base 十进制个位数
 */
static void CharMultiplyChar(vector<char>& num,int base){
    int jin=0,sum;
    size_t i;

    for(i=0;i<num.size();++i){
        sum=jin+(num[i]-'0')*base;
        if(sum>=10){
            jin=sum/10;
            sum%=10;
        }
        else
            jin=0;
        num[i]=sum+'0';
    }
    if(jin!=0)
        num.push_back(jin+'0');
}


/**
 * 在字符数组中找到第一个不为0的字符
 * @param cstr 字符串
 * @return 不为0的字符的位置
 */
static int GetFirstNotZero(const char* cstr){
    int i=0;
    while(*(cstr+i)=='0')
        ++i;
    return i;
}

BigInteger::BigInteger(const char *number) {
    bool token;

    //验证并获取字符数组的信息
    size_t len= isNumberString(number,&token,&this->isNegative);

    if(len==0){
        data.push_back(0);
        isNegative=false;
        return;
    }

    //当前的字符串位置
    ::uint32_t start=0;

    //跳过符号位
    if(token)
        ++start;

    //商的长度
    int qlen;

    //复制一个数组
    char* copy_number=new char[len+1];
    //排除前导0
    qlen= GetFirstNotZero(number+start);
    strcpy(copy_number,number+start+ qlen);

    //减去符号位和前导0的长度
    len-=qlen+token;

    //保存商的数组
    char* quotient=new char [len+1];

    //前一个商的位置
    ::uint32_t prev_quotient;

    ::uint32_t i;

    //保存被除数的数值
    ::uint64_t sum;

    do{
        //初始化
        prev_quotient=0xffffff;
        qlen=0;
        start=0;
        sum=0;

        do{
            while(sum<TenNumbers[0] && start<len) {
                sum = sum * 10 + copy_number[start] - '0';
                ++start;
            }

            //不能进行除法
            if(sum<TenNumbers[0])
                break;

            //补0
            i=start;
            while(prev_quotient+1<i){
                quotient[qlen++]='0';
                --i;
            }

            //获取倍数值
            for(i=1;i<9 && sum>=TenNumbers[i];++i);

            //所需的倍数
            --i;

            //写入商
            quotient[qlen++]=i+'1';

            //获取差
            sum-=TenNumbers[i];

            prev_quotient=start;
        }while(true);

        //写入余数到数据中
        data.push_back(sum);

        //不需要在除
        if(qlen==0)
            break;

        while(prev_quotient<start){
            quotient[qlen++]='0';
            --start;
        }

        //复制商数组到被除数数组
        quotient[qlen]=0;
        strcpy(copy_number,quotient);

        len=qlen;
    }while(true);

    if(data.size()==1 && data[0]==0)
        isNegative=false;
}

BigInteger::BigInteger(uint64_t data, bool isNegative) {
    //判断是否超过了32位
    uint32_t leftNumber=(data>>32);
    //将低32位存入
    this->data.push_back(data&LOWMARK);

    //
    if(leftNumber!=0)
        this->data.push_back(leftNumber);

    if(data==0)
        isNegative=false;
    else
        this->isNegative=isNegative;
}

BigInteger::~BigInteger() {
}

size_t BigInteger::isNumberString(const char *str,bool* hasToken,bool* isNegative) {
    size_t i=0;
    //返回的结果
    char ch=*str;

    if(ch=='+'||ch=='-'){
        *hasToken=true;
        if(ch=='-')
            *isNegative=true;
        else
            *isNegative=false;
        ++i;
    }
    else{
        *hasToken=false;
        *isNegative=false;
    }


    //判断之后的字符串是否全为数字
    while((ch=*(str+i))!=0){
        if(!IsNumber(ch))
            return 0;
        ++i;
    }
    return i;
}

bool BigInteger::IsNumber(char ch) {
    return (ch>='0' && ch<='9');
}

char *BigInteger::toString() const {
    vector<char> base;
    vector<char> sum;

    sum.push_back('0');
    base.push_back('1');

    size_t i,j;
    ::uint32_t w;
    ::uint32_t num;

    ::uint32_t _32one[32];
    for(i=0;i<32;++i)
        _32one[i]=(1<<i);


    for(i=0;i<data.size();++i){
        num=data[i];
        for(j=0;j<32;++j){
            w=(_32one[j]&num);
            if(w!=0)
                CharPlusChar(sum,base);
            CharMultiplyChar(base,2);
        }
    }

    i=0;
    char *str=new char[sum.size()+1+isNegative];
    char* rt=str;
    if(isNegative){
        *rt='-';
        ++str;
    }

    for(j=sum.size()-1;i<sum.size();++i,--j)
        str[i]=sum[j];
    str[i]=0;
    return rt;
}

/**
 * 从被除数数组的特定位置开始，判断在除数的特定位数内，是否
 * 大于等于除数
 * @param dividend 被除数的二进制数组
 * @param dividend_byte 被除数的二进制数组的开始比较位置的字节长度(长度)
 * @param dividend_bits 被除数的二进制数组的开始比较位置字节内，距离左端最近的非0位字节的距离(索引)
 * @param divisor 含义类比同上
 * @param divisor_byte 含义类比同上
 * @param divisor_bits 含义类比同上
 * @return 大于等于返回true
 */
static bool _dividend_greaterEqual_divisor(::uint8_t* dividend,size_t dividend_byte,
                                           size_t dividend_bits,::uint8_t* divisor,
                                           size_t divisor_byte,size_t divisor_bits){
    bool tk1,tk2;
    while(divisor_byte>0){
        tk1=(dividend[dividend_byte-1]&(0x80>>dividend_bits));
        tk2=(divisor[divisor_byte-1]&(0x80>>divisor_bits));

        if(tk1 && !tk2)
            return true;

        if(!tk1 && tk2)
            return false;

        if(dividend_bits==7){
            dividend_bits=0;
            --dividend_byte;
        }
        else
            ++dividend_bits;

        if(divisor_bits==7){
            divisor_bits=0;
            --divisor_byte;
        }
        else
            ++divisor_bits;
    }
    return true;
}

/**
 * 在被除数中减去除数
 * @param dividend 被除数数组
 * @param dividend_byte 被除数数组执行减法的开始字节(索引)
 * @param dividend_bit 被除数数组执行减法的开始字节内的自右向左便宜位(索引)
 * @param divisor 同上
 * @param divisor_allBits 除数数组的所有二进制位数量
 */
static void _dividend_minus_divisor(::uint8_t* dividend,size_t dividend_byte,size_t dividend_bit,
                                    ::uint8_t* divisor,size_t divisor_allBits){
    bool tk1,tk2;
    int jie=0;
    int r;
    //除数数组的当前位置
    size_t divisor_byte=0;
    size_t divisor_bit=0;

    while(divisor_allBits>0){
        --divisor_allBits;

        tk1=*(dividend+dividend_byte)&(0x1<<dividend_bit);
        tk2=*(divisor+divisor_byte)&(0x1<<divisor_bit);

        r=(int)tk1-tk2-jie;
        if(r<0){
            r+=2;
            jie=1;
        }
        else
            jie=0;

        if(r)
            *(dividend+dividend_byte)|=(0x1<<dividend_bit);
        else
            *(dividend+dividend_byte)&=(~(0x1<<dividend_bit));

        if(dividend_bit==7){
            dividend_bit=0;
            ++dividend_byte;
        }
        else
            ++dividend_bit;

        if(divisor_bit==7){
            divisor_bit=0;
            ++divisor_byte;
        }
        else
            ++divisor_bit;
    }

    if(jie){
        do{
            tk1=*(dividend+dividend_byte)&(0x1<<dividend_bit);
            if(!tk1)
                *(dividend+dividend_byte)|=(0x1<<dividend_bit);
            else{
                *(dividend+dividend_byte)&=(~(0x1<<dividend_bit));
                break;
            }

            if(dividend_bit==7){
                dividend_bit=0;
                ++dividend_byte;
            }
            else
                ++dividend_bit;
        }while(true);
    }
}


void BigInteger::_minus_bigInteger(BigInteger &result, const BigInteger &num) {
    ::int64_t sum;
    bool hasJin=false;

    size_t i;
    for(i=0;i<num.data.size();++i){
        sum=((::int64_t)result.data[i]-(::int64_t)num.data[i])-hasJin;
        if(sum<0){
            sum+=TenNumbers[0];
            hasJin=true;
        }
        else
            hasJin=false;

        result.data[i]=sum;
    }

    while(i<result.data.size()){
        sum=(::int64_t)result.data[i]-hasJin;
        if(sum<0)
            sum+=TenNumbers[0];
        else
            hasJin=false;
        result.data[i]=sum;
        ++i;
    }

    //清除前导0
    i=result.data.size();

    while(i>1 && result.data[i-1]==0){
        result.data.pop_back();
        --i;
    }
}

void BigInteger::_add_bigInteger(BigInteger &result, const BigInteger &num) {
    ::uint64_t sum;
    ::uint32_t jin=0;

    size_t i;
    for(i=0;i<result.data.size() && i<num.data.size();++i){
        sum=(::uint64_t)result.data[i]+num.data[i]+jin;
        result.data[i]=sum;
        jin=(sum>>32);
    }

    while(i<result.data.size()){
        sum=(::uint64_t)result.data[i]+jin;
        result.data[i]=sum;
        jin=(sum>>32);
        ++i;
    }

    while(i<num.data.size()){
        sum=(::uint64_t)num.data[i]+jin;
        result.data[i]=sum;
        jin=(sum>>32);
        ++i;
    }

    if(jin!=0)
        result.data.push_back(jin);
}

bool BigInteger::operator<(const BigInteger &num) const {
    if(isNegative && !num.isNegative)
        return true;
    if(!isNegative && num.isNegative)
        return false;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return true;
    if(l1>l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1]){
            if(isNegative)
                return true;
            return false;
        }
        if(data[l1]<num.data[l1]){
            if(isNegative)
                return false;
            return true;
        }
    }

    return false;
}

bool BigInteger::operator<=(const BigInteger &num) const {
    if(isNegative && !num.isNegative)
        return true;
    if(!isNegative && num.isNegative)
        return false;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return true;
    if(l1>l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1]){
            if(isNegative)
                return true;
            return false;
        }
        if(data[l1]<num.data[l1]){
            if(isNegative)
                return false;
            return true;
        }
    }

    return true;
}

bool BigInteger::operator>(const BigInteger &num) const {
    if(!isNegative && num.isNegative)
        return true;
    if(isNegative && !num.isNegative)
        return false;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1>l2)
        return true;
    if(l1<l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1]) {
            if(isNegative)
                return false;
            return true;
        }
        if(data[l1]<num.data[l1]) {
            if(isNegative)
                return true;
            return false;
        }
    }

    return false;
}

bool BigInteger::operator>=(const BigInteger &num) const {
    if(!isNegative && num.isNegative)
        return true;
    if(isNegative && !num.isNegative)
        return false;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1>l2)
        return true;
    if(l1<l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1]) {
            if(isNegative)
                return false;
            return true;
        }
        if(data[l1]<num.data[l1]) {
            if(isNegative)
                return true;
            return false;
        }
    }

    return true;
}

bool BigInteger::operator==(const BigInteger &num) const {
    if(isNegative!=num.isNegative)
        return false;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1!=l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]!=num.data[l1])
            return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger &num) const {
    if(isNegative!=num.isNegative)
        return true;

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1==l2)
        return true;

    while(l1>0){
        --l1;
        if(data[l1]!=num.data[l1])
            return true;
    }
    return false;
}

bool BigInteger::absGreaterThan(const BigInteger &num) const {
    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return false;
    if(l1>l2)
        return true;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1])
            return true;
        if(data[l1]<num.data[l1])
            return false;
    }
    return false;
}

bool BigInteger::absGreaterEqual(const BigInteger &num) const {
    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return false;
    if(l1>l2)
        return true;

    while(l1>0){
        --l1;
        if(data[l1]>num.data[l1])
            return true;
        if(data[l1]<num.data[l1])
            return false;
    }
    return true;
}

bool BigInteger::absLessThan(const BigInteger &num) const {
    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return true;
    if(l1>l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]<num.data[l1])
            return true;
        if(data[l1]>num.data[l1])
            return false;
    }
    return false;
}

bool BigInteger::absLessEqual(const BigInteger &num) const {
    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1<l2)
        return true;
    if(l1>l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]<num.data[l1])
            return true;
        if(data[l1]>num.data[l1])
            return false;
    }
    return true;
}

bool BigInteger::absEqual(const BigInteger &num) const {
    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1!=l2)
        return false;

    while(l1>0){
        --l1;
        if(data[l1]!=num.data[l1])
            return false;
    }
    return true;
}

bool BigInteger::absNotEqual(const BigInteger &num) const {

    size_t l1=data.size();
    size_t l2=num.data.size();

    if(l1!=l2)
        return true;

    while(l1>0){
        --l1;
        if(data[l1]!=num.data[l1])
            return true;
    }
    return false;
}

bool BigInteger::isZero() const {
    return data.size()==1 && data[0]==0;
}

BigInteger::BigInteger(const BigInteger &number):data(number.data) {
    isNegative=number.isNegative;
}

BigInteger BigInteger::dividedAndRemainder(const BigInteger &divisor, BigInteger &remainder) const {
    BigInteger result;
    _divided_bigInteger(&result,&remainder,*this,divisor);
    return result;
}

void BigInteger::_divided_bigInteger(BigInteger* quotient,BigInteger* remainder,
                                      const BigInteger& dividend,const BigInteger& divisor) {

    //若被除数不比除数大，或除数是0，则直接返回
    if(dividend.absLessThan(divisor) || divisor.isZero()){
        //需要获取商
        if(quotient!= nullptr){
            quotient->data.clear();
            quotient->data.push_back(0);
        }
        //需要获取余数
        if(remainder!= nullptr){
            remainder->data.assign(dividend.data.begin(),dividend.data.end());
            remainder->isNegative=dividend.isNegative;
        }
        return;
    }

    //被除数的数据的长度
    size_t dividend_len=dividend.data.size();
    //除数的数据长度
    size_t divisor_len=divisor.data.size();

    //保存商数组的实际长度
    size_t quotient_len;

    //临时变量
    size_t i,j,k;

    //被除数的字节长度(索引)
    size_t dividend_byte=dividend_len*4-1;

    //被除数最高位的字节中距离字节内左端最近的非0位的距离(索引)
    size_t dividend_bit=0;

    //除数的字节长度(索引)
    size_t divisor_byte=divisor_len*4-1;

    //除数最高位的字节中距离字节内左端最近的非0位的距离(索引)
    size_t divisor_bit=0;

    //被除数的有效二进制位的数量
    size_t dividend_all_bits;

    //除数的有效二进制位的数量
    size_t divisor_all_bits;

    //商数组的当前写入字节位置
    size_t quotient_byte;

    //商数组的当前写入字节中距离字节内左端的写入距离
    size_t quotient_bit;

    //进行除法之前的被除数有效长度
    size_t prev_divided_len;

    //进行除法的除数有效位数
    size_t div_divisor_len;

    //保存被除数数据的数组
    ::uint8_t * dividend_data=new ::uint8_t [dividend_len*4];
    //保存除数数据的数组
    ::uint8_t * divisor_data=(::uint8_t*)divisor.data.data();
    //保存商数据的数组
    ::uint8_t *quotient_data;


    //复制被除数数据到新的数组
    for(i=0;i<dividend_len;++i)
        *(::uint32_t*)(dividend_data+i*4)=dividend.data[i];

    //排除被除数数组的高地址的0字节
    while(dividend_data[dividend_byte]==0)
        --dividend_byte;

    //排除除数数组的高地址的0字节
    while(divisor_data[divisor_byte]==0)
        --divisor_byte;


    //找到被除数数组的最高字节的距离字节内左端最近的非0位的距离
    while(!(dividend_data[dividend_byte]&(0x80>>dividend_bit)))
        ++dividend_bit;
    //意义同上
    while(!(divisor_data[divisor_byte]&(0x80>>divisor_bit)))
        ++divisor_bit;

    //计算被除数数组的总有效二进制位数
    dividend_all_bits=(dividend_byte)*8+(8-dividend_bit);
    //同上
    divisor_all_bits=(divisor_byte)*8+(8-divisor_bit);

    //计算商数组的当前写入位置
    i=dividend_all_bits-divisor_all_bits+1;
    quotient_bit=(8-i%8)%8;//自左向右(索引)

    //(索引)
    quotient_byte=i/8+((quotient_bit==0)?0:1)-1;

    //保存商数组的实际长度
    quotient_len=quotient_byte+1;

    //初始化商数组
    quotient_data=new ::uint8_t [quotient_byte+1];
    for(i=0;i<=quotient_byte;++i)
        quotient_data[i]=0;

    //初始化为被除数有效长度
    prev_divided_len=dividend_all_bits;

    //可以开始除法
    do {
        //判断能否在被除数的当前位置可以直接整除除数
        if (_dividend_greaterEqual_divisor(dividend_data, dividend_byte + 1, dividend_bit,
                                           divisor_data, divisor_byte + 1, divisor_bit)) {
            //在商的对于位置设置1
            quotient_data[quotient_byte] |= (0x80 >> quotient_bit);
            //更新商数组的写入位置
            if (quotient_bit == 7) {
                --quotient_byte;
                quotient_bit = 0;
            } else
                ++quotient_bit;

            //除数的有效位数
            div_divisor_len = divisor_all_bits;

        }//若不能，判断当前的被除数还能否多出一位二进制位
        else if (dividend_all_bits >= divisor_all_bits + 1) {
            //更新商数组的写入位置
            if (quotient_bit == 7) {
                --quotient_byte;
                quotient_bit = 0;
            } else
                ++quotient_bit;

            quotient_data[quotient_byte] |= (0x80 >> quotient_bit);

            //除数的有效位数
            div_divisor_len = divisor_all_bits + 1;
        } else
            break;

        //计算出进行除法的位置
        //开始执行除法的被除数的最低有效位的位置
        i = dividend_all_bits - div_divisor_len;

        //得到开始进行除法的被除数的字节位置
        j = i / 8;
        //得到开始进行除法的被除数字节位置的二进制位置（从右到左）
        i %= 8;

        //执行减法操作
        _dividend_minus_divisor(dividend_data, j, i, divisor_data, divisor_all_bits);

        //更新被除数数组的有效长度
        while (!(dividend_data[dividend_byte] & (0x80 >> dividend_bit))) {
            if (dividend_bit == 7) {
                dividend_bit = 0;
                if(dividend_byte==0)
                    break;
                --dividend_byte;

            } else
                ++dividend_bit;
            --dividend_all_bits;
        }

        //当被除数有效位数小于等于除数时，退出
        if(dividend_all_bits<divisor_all_bits)
            break;

        //从商数组后补0，相当于移动位置
        while (dividend_all_bits + 1 < prev_divided_len) {
            //更新商数组的写入位置
            if (quotient_bit == 7) {
                --quotient_byte;
                quotient_bit = 0;
            } else
                ++quotient_bit;
            --prev_divided_len;
        }

        //记录下当前的被除数数组有效二进制位数长度
        prev_divided_len = dividend_all_bits;

    }while(true);

    //需要获取商
    if(quotient!= nullptr){
        //判断商数组是否多出了一个0位
        if(quotient_data[quotient_len-1]==0)
            --quotient_len;

        //清空商数组
        quotient->data.clear();
        //将商数组复制到结果中
        for(i=0;i<quotient_len/4;++i)
            quotient->data.push_back(*(::uint32_t*)(quotient_data+i*4));

        //将不满4字节的数据进行补全
        if((j=quotient_len%4)!=0){
            ::uint32_t n=0;
            ::uint8_t *p=(::uint8_t*)&n;
            for(i=0;i<j;++i)
                *(p+i)=quotient_data[quotient_len-j+i];
            quotient->data.push_back(n);
        }
        //更新商的符号位
        quotient->isNegative=(dividend.isNegative^divisor.isNegative);
    }

    //需要获取余数
    if(remainder!= nullptr){
        //获取余数的字节数(长度)
        ++dividend_byte;
        j=dividend_byte;

        //清空余数数组
        remainder->data.clear();

        //先整4字节复制
        for(i=0;i<j/4;++i)
            remainder->data.push_back(*(::uint32_t*)(dividend_data+i*4));
        //将不满4字节的数据进补全
        if((j%=4)!=0){
            ::uint32_t n=0;
            ::uint8_t *p=(::uint8_t*)&n;
            for(i=0;i<j;++i)
                *(p+i)=dividend_data[dividend_byte-j+i];
            remainder->data.push_back(n);
        }
        remainder->isNegative=(dividend.isNegative^divisor.isNegative);
    }
}

BigInteger::BigInteger(::uint32_t *data, size_t len, bool isNegative) {
    while(len>0 && data[len-1]==0)
        --len;
    //若len==0，则为0
    if(len==0){
        this->data.push_back(0);
        this->isNegative=false;
        return;
    }
    for(size_t i=0;i<len;++i)
        this->data.push_back(data[i]);
    this->isNegative=isNegative;
}

BigInteger BigInteger::operator+(const BigInteger &num) const {
    BigInteger result;

    if(!(isNegative ^ num.isNegative)){
        result=*this;
        _add_bigInteger(result,num);
        result.isNegative=isNegative;
    }
    else{
        //符号不一样
        //若大于等于num
        if(absGreaterEqual(num)){
            result=*this;
            _minus_bigInteger(result,num);
            if(isNegative){
                if(result.isZero())
                    result.isNegative=false;
                else
                    result.isNegative=true;
            }
            else
                result.isNegative=false;
        }
        else{
            result=num;
            _minus_bigInteger(result,*this);
            if(isNegative)
                result.isNegative=false;
            else
                result.isNegative = true;
        }
    }
    return result;
}

BigInteger BigInteger::operator-(const BigInteger &num) const {
    BigInteger result;

    //是否异号
    bool isNotSym=(isNegative^ num.isNegative);

    //
    if(absGreaterEqual(num)){
        result=*this;
        if(isNotSym){
            _add_bigInteger(result,num);
            if(isNegative){

                if(result.isZero())
                    result.isNegative=false;
                else
                    result.isNegative=true;
            }
            else
                result.isNegative=false;
        }
        else{
            _minus_bigInteger(result,num);
            if(isNegative){
                if(result.isZero())
                    result.isNegative=false;
                else
                    result.isNegative=true;
            }
            else
                result.isNegative=false;
        }
    }
    else{
        if(isNotSym){
            result=*this;
            _add_bigInteger(result,num);
            if(isNegative)
                result.isNegative=true;
            else
                result.isNegative=false;
        }
        else{
            result=num;
            _minus_bigInteger(result,*this);
            if(isNegative)
                result.isNegative=false;
            else
                result.isNegative=true;
        }
    }
    return result;
}

BigInteger BigInteger::operator*(const BigInteger &num) const {
    BigInteger result;
    _multiply_bigInteger(result,*this,num);
    return result;
}

BigInteger BigInteger::operator/(const BigInteger &num) const {
    BigInteger result;
    _divided_bigInteger(&result, nullptr,*this,num);
    return result;
}

BigInteger BigInteger::operator%(const BigInteger &num) const {
    BigInteger result;
    _divided_bigInteger(nullptr,&result,*this,num);
    return result;
}

BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    result.isNegative=!result.isNegative;
    return result;
}

BigInteger &BigInteger::operator+=(const BigInteger &num) {

    if(!(isNegative ^ num.isNegative)){
        _add_bigInteger(*this,num);
    }
    else{
        //符号不一样
        //若大于等于num
        if(absGreaterEqual(num)){

            _minus_bigInteger(*this,num);
            if(isNegative){
                if(this->isZero())
                    this->isNegative=false;
                else
                    this->isNegative=true;
            }
        }
        else{
            BigInteger btemp(*this);
            *this=num;
            _minus_bigInteger(*this,btemp);

            if(isNegative)
                this->isNegative=false;
            else {
                this->isNegative=true;
            }
        }
    }
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &num) {

    //是否异号
    bool isNotSym=(isNegative^ num.isNegative);

    //
    if(absGreaterEqual(num)){
        if(isNotSym){
            _add_bigInteger(*this,num);
            if(isNegative){

                if(this->isZero())
                    this->isNegative=false;
                else
                    this->isNegative=true;
            }
            else
                this->isNegative=false;
        }
        else{
            _minus_bigInteger(*this,num);
            if(isNegative){
                if(this->isZero())
                    this->isNegative=false;
                else
                    this->isNegative=true;
            }
            else
                this->isNegative=false;
        }
    }
    else{
        if(isNotSym){
            _add_bigInteger(*this,num);
            if(isNegative)
                this->isNegative=true;
            else
                this->isNegative=false;
        }
        else{
            BigInteger btemp(*this);
            *this=num;
            _minus_bigInteger(*this,btemp);
            if(isNegative)
                this->isNegative=false;
            else
                this->isNegative=true;
        }
    }
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &num) {
    *this=*this*num;
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &num) {
    *this=*this/num;
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &num) {
    *this=*this%num;
    return *this;
}

BigInteger &BigInteger::operator++() {
    *this=*this+BigInteger(1,false);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger result(*this);
    *this+=BigInteger(1,false);
    return result;
}

BigInteger &BigInteger::operator--() {
    *this=*this-BigInteger(1,false);
    return *this;
}

BigInteger &BigInteger::operator--(int) {
    BigInteger result(*this);
    --*this;
    return result;
}

BigInteger &BigInteger::operator=(const BigInteger &num) {
    data=num.data;
    isNegative=num.isNegative;
    return *this;
}

BigInteger::operator bool() const {
    return !isZero();
}

BigInteger::operator ::uint64_t() const {
    ::uint64_t num=0;
    num|=data[0];
    if(data.size()>1)
        num|=((::uint64_t)data[1]<<32);
    return num;
}

bool BigInteger::operator!() const {
    bool t=(bool)*this;
    return !t;
}



BigInteger BigInteger::operator&(const BigInteger &num) const {
    BigInteger result;
    const BigInteger* maxBig;
    const BigInteger* minBig;

    size_t len1=data.size();
    size_t len2=num.data.size();

    size_t i;

    if(len1>len2){
        result=num;
        maxBig=this;
        minBig=&num;
    }
    else{
        result=*this;
        maxBig=&num;
        minBig=this;
    }

    _and(result,minBig->data.size(),*maxBig);
    return result;
}

BigInteger BigInteger::operator|(const BigInteger &num) const {
    BigInteger result;
    const BigInteger* maxBig;
    const BigInteger* minBig;

    size_t len1=data.size();
    size_t len2=num.data.size();

    size_t i;

    if(len1>len2){
        result=*this;
        maxBig=this;
        minBig=&num;
    }
    else{
        result=num;
        maxBig=&num;
        minBig=this;
    }

    _or(result,minBig->data.size(),*minBig);
    return result;
}

BigInteger BigInteger::operator^(const BigInteger &num) const {
    BigInteger result;
    const BigInteger* maxBig;
    const BigInteger* minBig;

    size_t len1=data.size();
    size_t len2=num.data.size();

    size_t i;

    if(len1>len2){
        result=*this;
        maxBig=this;
        minBig=&num;
    }
    else{
        result=num;
        maxBig=&num;
        minBig=this;
    }

    _xor(result,minBig->data.size(),*minBig);
    return result;
}

BigInteger BigInteger::operator~() const {
    BigInteger result(*this);
    _not(result);
    return result;
}

BigInteger &BigInteger::operator|=(const BigInteger &num) {
    size_t i;
    size_t len1,len2;
    len1=data.size();
    len2=num.data.size();
    if(len1<len2){
        for(i=len1;i<len2;++i)
            data.push_back(num.data[i]);
        _or(*this,len1,num);
    }
    else
        _or(*this,len2,num);
    return *this;
}

BigInteger &BigInteger::operator&=(const BigInteger &num) {
    size_t i;
    size_t len1,len2;
    len1=data.size();
    len2=num.data.size();
    if(len1>len2){
        for(i=len1;i<len2;++i)
            data.pop_back();
        _and(*this,len2,num);
    }
    else
        _and(*this,len1,num);
    return *this;
}

BigInteger &BigInteger::operator^=(const BigInteger &num) {
    size_t i;
    size_t len1,len2;
    len1=data.size();
    len2=num.data.size();
    if(len1<len2){
        for(i=len1;i<len2;++i)
            data.push_back(num.data[i]);
        _xor(*this,len1,num);
    }
    else
        _xor(*this,len2,num);
    return *this;
}

BigInteger BigInteger::operator<<(::uint64_t num) const {
    BigInteger result=(*this);
    _left_move(result,num);
    return result;
}

BigInteger BigInteger::operator>>(::uint64_t num) const {
    BigInteger result(*this);
    _right_move(result,num);
    return result;
}

BigInteger &BigInteger::operator>>=(::uint64_t num) {
    _right_move(*this,num);
    return *this;
}

BigInteger &BigInteger::operator<<=(::uint64_t num) {
    _left_move(*this,num);
    return *this;
}

void BigInteger::_clear_last_zero(BigInteger &big) {
    size_t len=big.data.size();
    while(len>1 && big.data[len-1]==0){
        big.data.pop_back();
        --len;
    }
    if(len==1 && big.data[0]==0)
        big.isNegative=false;
}

void BigInteger::_and(BigInteger &result,size_t len, const BigInteger &maxBig) {
    for(size_t i=0;i<len;++i)
        result.data[i]&=maxBig.data[i];
    _clear_last_zero(result);
}

void BigInteger::_or(BigInteger &result, size_t len, const BigInteger &minBig) {
    for(size_t i=0;i<minBig.data.size();++i)
        result.data[i]|=minBig.data[i];
    _clear_last_zero(result);
}

void BigInteger::_xor(BigInteger &result, size_t len, const BigInteger &minBig) {
    for(size_t i=0;i<minBig.data.size();++i)
        result.data[i]^=minBig.data[i];
    _clear_last_zero(result);
}

void BigInteger::_not(BigInteger &result) {
    size_t i=0;
    while(i<result.data.size()){
        result.data[i]=~(result.data[i]);
        ++i;
    }
}

void BigInteger::setZero() {
    data.clear();
    data.push_back(0);
    isNegative=false;
}

void BigInteger::_right_move(BigInteger &result, ::uint64_t num) {
    ::uint64_t i,j;
    ::uint64_t move_byte=num/32;
    ::uint64_t move_bit=num%32;
    ::uint64_t move_x_bit=32-move_bit;

    if(move_byte>=result.data.size())
        result.setZero();
    else{
        //整体右移
        for(i=0,j=move_byte;j<result.data.size();++j,++i)
            result.data[i]=result.data[j];
        for(i=0;i<move_byte;++i)
            result.data.pop_back();

        //确定需要继续移动字节
        if(move_bit!=0){
            //先将首位数据移动
            result.data[0]>>=move_bit;
            //前一个数据
            i=0;
            //当前的数据
            j=1;

            while(j<result.data.size()){
                result.data[i]|=(result.data[j]<<move_bit);
                result.data[j]>>=move_x_bit;
                ++j;
                ++i;
            }
            _clear_last_zero(result);
        }
    }
}

void BigInteger::_left_move(BigInteger &result, ::uint64_t num) {
    ::uint64_t i,j,k;
    ::uint64_t move_byte=num/32;
    ::uint64_t move_bit=num%32;
    ::uint64_t move_x_bit=32-move_bit;
    ::uint64_t result_origin_byte=result.data.size();

    ::uint32_t dtemp;

    for(i=0;i<move_byte;++i)
        result.data.push_back(result.data[result_origin_byte-move_byte+i]);

    for(i=result_origin_byte;i>move_byte;--i)
        result.data[i-1]=result.data[i-move_byte-1];

    while(i>0){
        --i;
        result.data[i]=0;
    }

    if(move_bit!=0){

        result.data.push_back(0);
        for(i=result.data.size(),j=result.data.size()-1,k=result_origin_byte;k>0;--i,--j,--k){
            result.data[i-1]|=(result.data[j-1]>>move_x_bit);
            result.data[j-1]<<=move_bit;
        }
    }

    _clear_last_zero(result);
}

void BigInteger::_multiply_bigInteger(BigInteger &result, const BigInteger &num1, const BigInteger &num2) {
    ::uint64_t sum;
    ::uint64_t jin;


    //乘0的判断
    if(num1.isZero() || num2.isZero())
        return;

    size_t i,j;
    size_t tk;
    for(i=0;i<num2.data.size();++i){
        jin=0;
        for(j=0;j<num1.data.size();++j){
            sum=(::uint64_t)num2.data[i]*num1.data[j]+jin;
            tk=i+j;

            if(result.data.size()<=tk)
                result.data.push_back(0);

            sum+=(::uint64_t)result.data[tk];
            result.data[tk]=sum;
            jin=(sum>>32);
        }
        //加法
        if(jin!=0)
            result.data.push_back(jin);
    }

    result.isNegative=(num1.isNegative^num2.isNegative);
}

const ::uint32_t *BigInteger::toBinaryArray(::uint64_t* len) const {
    *len=data.size();
    return data.data();
}

::uint64_t BigInteger::length() const {
    return data.size();
}

BigInteger &BigInteger::negate() {
    if(!isZero())
        isNegative=!isNegative;
    return *this;
}



