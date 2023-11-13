#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

const int MAXN = 9999;
const int MAXLEN = 500003;
const int DIGLEN = 4;


namespace sjtu {
    class int2048 {

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

        void read(const std::string &);

        void print() const;

        int2048 &add(const int2048 &);

        friend int2048 add(int2048, const int2048 &);

        int2048 &minus(const int2048 &);

        friend int2048 minus(int2048, const int2048 &);

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

        double to_double();

        std::string to_string();
    };

} // namespace sjtu

#endif
