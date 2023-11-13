#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

const int MAXN = 9999;
const int MAXLEN = 500003;
const int DIGLEN = 4;
const int BASE = 10000;
// 请不要使用 using namespace std;

namespace sjtu {
    class int2048 {
        // todo

    private:
        std::vector<long long> a;
        int len = 1;
        bool neg = false;
    public:
        // 构造函数
        int2048();

        int2048(long long);

        int2048(const std::string &);

        int2048(const int2048 &);

        int2048(int2048 &&) noexcept;

        void check_len();

        void clear();

        // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
        // 如果需要，可以自行增加其他所需的函数
        // ===================================
        // Integer1
        // ===================================

        // 读入一个大整数
        void read(const std::string &);

        // 输出储存的大整数，无需换行
        void print() const;

        // 加上一个大整数
        int2048 &add(const int2048 &);

        // 返回两个大整数之和
        friend int2048 add(int2048, const int2048 &);

        // 减去一个大整数
        int2048 &minus(const int2048 &);

        // 返回两个大整数之差
        friend int2048 minus(int2048, const int2048 &);

        // ===================================
        // Integer2
        // ===================================

        int2048 operator+() const;

        int2048 operator-() const;

        int2048 &operator=(const int2048 &);

        int2048 &operator=(int2048 &&) noexcept;

        int2048 &operator+=(const int2048 &);

        friend int2048 operator+(int2048, const int2048 &);

        int2048 &operator-=(const int2048 &);

        friend int2048 operator-(int2048, const int2048 &);

        int2048 &operator*=(const int2048 &);

        friend int2048 operator*(int2048, const int2048 &);

        int2048 &operator/=(const int2048 &);

        friend int2048 operator/(int2048, const int2048 &);

        int2048 &operator%=(const int2048 &);

        friend int2048 operator%(int2048, const int2048 &);

        friend std::istream &operator>>(std::istream &, int2048 &);

        friend std::ostream &operator<<(std::ostream &, const int2048 &);

        friend bool operator==(const int2048 &, const int2048 &);

        friend bool operator!=(const int2048 &, const int2048 &);

        friend bool operator<(const int2048 &, const int2048 &);

        friend bool operator>(const int2048 &, const int2048 &);

        friend bool operator<=(const int2048 &, const int2048 &);

        friend bool operator>=(const int2048 &, const int2048 &);

        int2048 &operator<<=(int offset);

        friend int2048 &operator<<(int2048, int offset);

        int2048 &operator>>=(int offset);

        friend int2048 &operator>>(int2048, int offset);

        friend int2048 inverse(const int2048 &);

        friend int2048 divide(const int2048 &, const int2048 &);

        friend bool abs_less(const int2048 &, const int2048 &);

        int2048 &sub_mul(const sjtu::int2048 &b, int mul, int offset);

        double to_double();

        std::string to_string();
    };

} // namespace sjtu

#endif
