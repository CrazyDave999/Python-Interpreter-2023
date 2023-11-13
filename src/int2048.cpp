#include "int2048.h"
//#include <cassert>

namespace ntt {
    const int P[3]{998244353, 1004535809, 469762049};
    int rev[MAXLEN];

    int quick_pow(int x, int y, int p) {
        int res = 1;
        while (y) {
            if (y & 1)res = (int) (1ll * res * x % p);
            x = (int) (1ll * x * x % p);
            y >>= 1;
        }
        return res;
    }

    const long long INV0 = quick_pow(P[1] % P[0], P[0] - 2, P[0]);
    const long long INV1 = quick_pow(P[0] % P[1], P[1] - 2, P[1]);
    const long long PP = 1ll * P[0] * P[1];
    const long long INV2 = quick_pow((int) (PP % P[2]), P[2] - 2, P[2]);


    void set_rev(int len) {
        for (int i = 0; i < len; ++i) {
            rev[i] = rev[i >> 1] >> 1;
            if (i & 1) {
                rev[i] |= len >> 1;
            }
        }
    }

    void change(int *a, int len) {
        for (int i = 0; i < len; ++i) {
            if (i < rev[i]) {
                std::swap(a[i], a[rev[i]]);
            }
        }
    }

    void ntt(int *a[], int size, int len, int p, bool r) {
        for (int i = 0; i < size; ++i)change(a[i], len);
        for (int l = 2; l <= len; l <<= 1) {
            int k = l >> 1;
            int gn = quick_pow(3, (p - 1) / l, p);
            if (r) {
                gn = quick_pow(gn, p - 2, p);
            }
            for (int i = 0; i < len; i += l) {
                int g = 1;
                for (int j = i; j < i + k; ++j) {
                    for (int m = 0; m < size; ++m) {
                        int tmp = (int) (1ll * a[m][j + k] * g % p);
                        a[m][j + k] = (a[m][j] - tmp + p) % p;
                        a[m][j] = (a[m][j] + tmp) % p;
                    }
                    g = (int) (1ll * g * gn % p);
                }
            }
        }
        if (r) {
            int inv = quick_pow(len, p - 2, p);
            for (int i = 0; i < len; ++i) {
                for (int m = 0; m < size; ++m) {
                    a[m][i] = (int) (1ll * a[m][i] * inv % p);
                }
            }
        }
    }

    long long quick_mul(long long a, long long b, long long p) {
        long long res = a * b - ((long long) ((long double) a / p * b + 0.5)) * p;
        return res < 0 ? res + p : res;
    }

    long long get(long long v0, long long v1, long long v2) {
        long long a = quick_mul(v0 * P[1] % PP, INV0, PP);
        long long b = quick_mul(v1 * P[0] % PP, INV1, PP);

        long long c = (a + b) % PP;
        long long d = c;
        c = (v2 - c % P[2] + P[2]) % P[2] * INV2 % P[2];
        return c * PP + d;
    }
}

sjtu::int2048::int2048() {
    len = 1;
    neg = false;
    a.push_back(0);
}

sjtu::int2048::int2048(long long num) {
    if (num < 0) {
        neg = true;
        num = -num;
    } else {
        neg = false;
    }
    long long quo, rem;
    len = 0;
    while (num > MAXN) {
        quo = num / (MAXN + 1);
        rem = num - quo * (MAXN + 1);
        num = quo;
        a.push_back(rem);
        ++len;
    }
    a.push_back(num);
    ++len;
}

sjtu::int2048::int2048(const std::string &str) {
    if (str[0] == '-') {
        neg = true;
    } else {
        neg = false;
    }
    int l = (int) str.length();
    if (neg)--l;
    len = l / DIGLEN;
    if (l % DIGLEN)++len;
    int start = 0, end = l - 1;
    if (neg) {
        start = 1;
        end = l;
    }

    for (int high = end; high >= start; high -= DIGLEN) {
        int low = high - DIGLEN + 1;
        if (low < start)low = start;
        int dig = 0;
        for (int i = low; i <= high; ++i) {
            dig *= 10;
            dig += str[i] - '0';
        }
        a.push_back(dig);
    }
    check_len();
    if (a[len - 1] == 0)neg = false;
}

sjtu::int2048::int2048(const sjtu::int2048 &rhs) : len(rhs.len), neg(rhs.neg) {
    a.assign(rhs.a.begin(), rhs.a.end());
    check_len();
}

sjtu::int2048::int2048(sjtu::int2048 &&rhs) noexcept: len(rhs.len), neg(rhs.neg) {
    a.assign(rhs.a.begin(), rhs.a.end());
    check_len();
}

void sjtu::int2048::read(const std::string &str) {
    if (str[0] == '-') {
        neg = true;
    } else {
        neg = false;
    }

    int l = (int) str.length();
    if (neg)--l;
    len = l / DIGLEN;
    if (l % DIGLEN)++len;
    int start = 0, end = l - 1;
    if (neg) {
        start = 1;
        end = l;
    }
    a.clear();
    for (int high = end; high >= start; high -= DIGLEN) {
        int low = high - DIGLEN + 1;
        if (low < start)low = start;
        int dig = 0;
        for (int i = low; i <= high; ++i) {
            dig *= 10;
            dig += str[i] - '0';
        }
        a.push_back(dig);
    }
    check_len();
    if (a[len - 1] == 0)neg = false;
}

void sjtu::int2048::print() const {
    std::ios::sync_with_stdio(false);
    if (neg && a[len - 1] != 0)printf("-");
    for (int i = len - 1; i >= 0; --i) {
        if (i == len - 1)printf("%lld", a[i]);
        else printf("%04lld", a[i]);
    }
}

sjtu::int2048 &sjtu::int2048::add(const sjtu::int2048 &rhs) {
//    assert(false);
    if (neg != rhs.neg) {
        neg = !neg;
        minus(rhs);
        neg = !neg;
        return *this;
    }

    int max_len = std::max(len, rhs.len);
    int carry = 0;
    a.resize(max_len + 1);
    for (int i = 0; i < max_len; ++i) {
        long long x = 0;
        if (i < rhs.len)x = rhs.a[i];
        a[i] += x + carry;
        carry = 0;
        if (a[i] > MAXN) {
            a[i] -= MAXN + 1;
            ++carry;
        }
    }
    a[max_len] = carry;
    if (a[max_len])len = max_len + 1;
    else len = max_len;
    a.resize(len);
    return *this;
}

sjtu::int2048 sjtu::add(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs.add(rhs);
}

sjtu::int2048 &sjtu::int2048::operator+=(const sjtu::int2048 &rhs) {
    return add(rhs);
}

sjtu::int2048 sjtu::operator+(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs.add(rhs);
}

sjtu::int2048 &sjtu::int2048::minus(const sjtu::int2048 &rhs) {
//    assert(false);
    if (this == &rhs) {
        clear();
        return *this;
    }
    if (neg != rhs.neg) {
        neg = !neg;
        add(rhs);
        neg = !neg;
        return *this;
    }
    bool is_neg = false;
    int2048 x, y;// x绝对值大
    if (neg) {
        if (*this < rhs) {
            x = *this;
            y = rhs;
            is_neg = true;
        } else {
            x = rhs;
            y = *this;
        }
    } else {
        if (*this < rhs) {
            x = rhs;
            y = *this;
            is_neg = true;
        } else {
            x = *this;
            y = rhs;
        }
    }
    int max_len = x.len;
    y.a.resize(max_len);
    for (int i = 0; i < max_len; ++i) {
        if (x.a[i] < y.a[i]) {
            int j = i + 1;
            while (x.a[j] == 0)++j;
            --x.a[j--];
            while (j > i)x.a[j--] += MAXN;
            x.a[j] += MAXN + 1 - y.a[i];
        } else {
            x.a[i] -= y.a[i];
        }
    }
    a = x.a;
    while (x.a[x.len - 1] == 0)--x.len;
    len = x.len;
    neg = is_neg;
    if (len == 0)len = 1;
    a.resize(len);
    return *this;
}


sjtu::int2048 sjtu::minus(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs.minus(rhs);
}

sjtu::int2048 &sjtu::int2048::operator-=(const sjtu::int2048 &rhs) {
    return minus(rhs);
}

sjtu::int2048 sjtu::operator-(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs.minus(rhs);
}

sjtu::int2048 sjtu::int2048::operator+() const {
    return *this;
}


sjtu::int2048 sjtu::int2048::operator-() const {
    sjtu::int2048 res(*this);
    if (a[len - 1] != 0) {
        res.neg = !res.neg;
    } else {
        res.neg = false;
    }
    return res;
}

bool sjtu::operator==(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    if (lhs.len != rhs.len)return false;
    if (lhs.neg != rhs.neg)return false;
    for (int i = 0; i < lhs.len; ++i)
        if (lhs.a[i] != rhs.a[i])
            return false;
    return true;
}

bool sjtu::operator!=(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    return !(lhs == rhs);
}

bool sjtu::operator<(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    if (lhs.a[lhs.len - 1] == 0) {
        if (rhs.a[rhs.len - 1] == 0) {
            return false;
        }
        return !rhs.neg;
    }
    if (rhs.a[rhs.len - 1] == 0) {
        return lhs.neg;
    }
    if (lhs.neg != rhs.neg) {
        return lhs.neg;
    }
    if (lhs.len != rhs.len)
        return (lhs.len < rhs.len) ^ lhs.neg;
    for (int i = lhs.len - 1; i >= 0; --i) {
        if (lhs.a[i] != rhs.a[i]) {
            if (lhs.neg) {
                return lhs.a[i] > rhs.a[i];
            } else {
                return lhs.a[i] < rhs.a[i];
            }
        }
    }
    return false;
}

bool sjtu::operator<=(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    if (lhs.a[lhs.len - 1] == 0) {
        if (rhs.a[rhs.len - 1] == 0) {
            return true;
        }
        return !rhs.neg;
    }
    if (rhs.a[rhs.len - 1] == 0) {
        return lhs.neg;
    }
    if (lhs.neg != rhs.neg) {
        return lhs.neg;
    }
    if (lhs.len != rhs.len) {
        return (lhs.len < rhs.len) ^ lhs.neg;
    }
    for (int i = lhs.len - 1; i >= 0; --i) {
        if (lhs.a[i] != rhs.a[i]) {
            if (lhs.neg) {
                return lhs.a[i] > rhs.a[i];
            } else {
                return lhs.a[i] < rhs.a[i];
            }
        }
    }
    return true;
}

bool sjtu::operator>(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    return !(lhs <= rhs);
}


bool sjtu::operator>=(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    return !(lhs < rhs);
}

sjtu::int2048 &sjtu::int2048::operator=(const sjtu::int2048 &rhs) {
    if (this == &rhs)return *this;
    len = rhs.len;
    neg = rhs.neg;
    a.clear();
    a.assign(rhs.a.begin(), rhs.a.end());
    check_len();
    return *this;
}

sjtu::int2048 &sjtu::int2048::operator=(sjtu::int2048 &&rhs) noexcept {
    len = rhs.len;
    neg = rhs.neg;
    a.resize(len);
    a.assign(rhs.a.begin(), rhs.a.end());
    check_len();
    return *this;
}

void sjtu::int2048::clear() {
    a.clear();
    a.push_back(0);
    neg = false;
    len = 1;
}


sjtu::int2048 &sjtu::int2048::operator*=(const sjtu::int2048 &rhs) {
//    assert(false);
    neg = neg ^ rhs.neg;
    int ntt_len = 1;
    while (ntt_len < len + rhs.len) {
        ntt_len <<= 1;
    }
    int x[3][2][ntt_len], z[3][ntt_len];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < ntt_len; ++j) {
            x[i][0][j] = j < len ? (int) a[j] : 0;
            x[i][1][j] = j < rhs.len ? (int) rhs.a[j] : 0;
        }
    }
    static int last_len;
    if (ntt_len != last_len) {
        ntt::set_rev(ntt_len);
        last_len = ntt_len;
    }
    int *p[2];
    for (int i = 0; i < 3; ++i) {
        p[0] = x[i][0];
        p[1] = x[i][1];
        ntt::ntt(p, 2, ntt_len, ntt::P[i], false);

        for (int j = 0; j < ntt_len; ++j) {
            z[i][j] = (int) (1ll * p[0][j] * p[1][j] % ntt::P[i]);
        }
        p[0] = z[i];
        ntt::ntt(p, 1, ntt_len, ntt::P[i], true);
    }

    a.resize(ntt_len + 1);
    for (int i = 0; i < ntt_len; ++i) {
        a[i] = ntt::get(z[0][i], z[1][i], z[2][i]);
    }

    len = 1;
    for (int i = 0; i < ntt_len; ++i) {
        if (a[i] > MAXN) {
            a[i + 1] += a[i] / (MAXN + 1);
            a[i] %= MAXN + 1;
            len = i + 2;
        }
        if (a[i])len = std::max(len, i + 1);
    }
    while (a[len - 1] > MAXN) {
        if (len == a.size())
            a.push_back(a[len - 1] / (MAXN + 1));
        else
            a[len] += a[len - 1] / (MAXN + 1);
        a[len - 1] %= MAXN + 1;
        ++len;
    }
    a.resize(len);
    if (a[len - 1] == 0)neg = false;
    return *this;
}

sjtu::int2048 sjtu::operator*(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs *= rhs;
}

sjtu::int2048 sjtu::divide(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    // 暴力竖式除法
    if (rhs == 1)return lhs;
    bool res_neg = lhs.neg ^ rhs.neg;
    int2048 a(lhs);
    a.neg = false;
    int2048 b(rhs);
    b.neg = false;

    int init_len = b.len;

    if (a < b)return {0};
    int2048 res;
    int pos = a.len - b.len;

    b <<= (a.len - b.len); // 对齐
    if (a < b) {
        b >>= 1;
        --pos;
    }

    while (true) {
        int l = 0, r = MAXN, mid, ans = 0;
        while (l <= r) {
            mid = (l + r) >> 1;
            if (a >= int2048(mid) * b) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }


        a -= int2048(ans) * b;
        int2048 q(ans);
        q <<= pos;
        res += q;

        if (b.len == init_len)
            break;
        b >>= 1;
        --pos;
    }
    res.neg = res_neg;

    return res;
}


std::istream &sjtu::operator>>(std::istream &is, sjtu::int2048 &rhs) {
    std::string s;
    is >> s;
    rhs.read(s);
    return is;
}

std::ostream &sjtu::operator<<(std::ostream &os, const sjtu::int2048 &rhs) {
    std::ios::sync_with_stdio(false);
    if (rhs.neg && rhs.a[rhs.len - 1] != 0)os << "-";
    for (int i = rhs.len - 1; i >= 0; --i) {
        if (i == rhs.len - 1)os << rhs.a[i];
        else {
            if (rhs.a[i] < 10)os << "000";
            else if (rhs.a[i] < 100)os << "00";
            else if (rhs.a[i] < 1000)os << "0";
            os << rhs.a[i];
        }
    }
    return os;
}

sjtu::int2048 &sjtu::int2048::operator<<=(int offset) {
    if (offset == 0)return *this;
    a.resize(len + offset);
    for (int i = len - 1; i >= 0; --i) {
        a[i + offset] = a[i];
        a[i] = 0;
    }
    len += offset;
    return *this;
}

sjtu::int2048 &sjtu::operator<<(sjtu::int2048 lhs, int offset) {
    return lhs <<= offset;
}

sjtu::int2048 &sjtu::int2048::operator>>=(int offset) {
    if (len <= offset) {
        clear();
        return *this;
    }
    if (offset == 0) {
        return *this;
    }
    for (int i = offset; i < len; ++i) {
        a[i - offset] = a[i];
    }
    len -= offset;
    a.resize(len);
    return *this;
}

sjtu::int2048 &sjtu::operator>>(sjtu::int2048 lhs, int offset) {
    return lhs >>= offset;
}

bool sjtu::abs_less(const sjtu::int2048 &lhs, const sjtu::int2048 &rhs) {
    if (lhs.len != rhs.len)
        return lhs.len < rhs.len;
    for (int i = lhs.len - 1; i >= 0; --i) {
        if (lhs.a[i] != rhs.a[i]) {
            return lhs.a[i] < rhs.a[i];
        }
    }
    return false;
}


sjtu::int2048 sjtu::inverse(const sjtu::int2048 &a) {
//    int l = a.get_len_base_10();
    int l = a.len;
    if (l <= 10) {
        return divide(int2048(1) <<= (l << 1), a);
    }
    int k = (l + 5) >> 1;
    int2048 b = a;
    b >>= (l - k);
    int2048 c = inverse(b);
    int2048 d = ((a * c * c) >> (k << 1));
    c += c;
    c <<= (l - k);

    c -= d;
    c -= int2048(1);
    int2048 delta = (int2048(1) << (l << 1)) - c * a;
    while ((int2048(1) << (l << 1)) - c * a >= a)
        c += int2048(1);
    return c;
}


sjtu::int2048 &sjtu::int2048::sub_mul(const sjtu::int2048 &b, int mul, int offset) {
    if (mul == 0) return *this;
    long long borrow = 0;
    for (size_t i = 0; i < b.a.size(); ++i) {
        borrow += a[i + offset] - b.a[i] * mul - BASE + 1;
        a[i + offset] = borrow % BASE + BASE - 1;
        borrow /= BASE;
    }
    for (size_t i = b.a.size(); borrow; ++i) {
        borrow += a[i + offset] - BASE + 1;
        a[i + offset] = borrow % BASE + BASE - 1;
        borrow /= BASE;
    }
    return *this;
}


sjtu::int2048 &sjtu::int2048::operator/=(const sjtu::int2048 &rhs) {
    int2048 d;
    int2048 r = *this;
    bool res_neg = neg ^ rhs.neg;
    if (a[len - 1] == 0) {
        return *this;
    }
    if (abs_less(*this, rhs)) {
        *this = res_neg ? -1 : 0;
        return *this;
    }
    d.len = a.size() - rhs.a.size() + 1;
    d.a.resize(a.size() - rhs.a.size() + 1);

    long long x1 = rhs.a.size() >= 2 ? rhs.a[rhs.a.size() - 2] : 0;
    long long x2 = rhs.a.size() >= 3 ? rhs.a[rhs.a.size() - 3] : 0;
    double t = (x1 + (x2 + 1.0) / BASE);
    double db = 1.0 / (rhs.a.back() + t / BASE);
    long long tmp;
    for (size_t i = a.size() - 1, j = d.a.size() - 1; j <= a.size();) {
        long long rm = i == r.a.size() - 1 ? r.a[i] : r.a[i + 1] * BASE + r.a[i];
        tmp = i == r.a.size() - 1 ? 0 : r.a[i + 1];
        long long m = std::max((long long) (db * rm), tmp);
        r.sub_mul(rhs, m, j);
        d.a[j] += m;
        tmp = i == r.a.size() - 1 ? 0 : r.a[i + 1];
        if (!tmp)
            --i, --j;
    }
    r.check_len();
    long long carry = 0;
    while (!abs_less(r, rhs)) {
        r.minus(rhs);
        ++carry;
    }
    for (long long &i: d.a) {
        carry += i;
        i = carry % BASE;
        carry /= BASE;
    }
    d.check_len();
    *this = d;
    return *this;
}

sjtu::int2048 sjtu::operator/(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs /= rhs;
}

sjtu::int2048 &sjtu::int2048::operator%=(const sjtu::int2048 &rhs) {
    return (*this -= ((*this / rhs) * rhs));
}

sjtu::int2048 sjtu::operator%(sjtu::int2048 lhs, const sjtu::int2048 &rhs) {
    return lhs %= rhs;
}

void sjtu::int2048::check_len() {
    while (a[len - 1] == 0 && len != 1) {
        --len;
    }
    a.resize(len);
}

double sjtu::int2048::to_double() {
    double res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res *= 10000;
        res += a[i];
    }
    if (neg) {
        res = -res;
    }
    return res;
}

std::string sjtu::int2048::to_string() {
    std::string res;
    if (neg) {
        res += "-";
    }
    for (int i = len - 1; i >= 0; --i) {
        if (i < len - 1) {
            if (a[i] < 10) {
                res += "000";
            } else if (a[i] < 100) {
                res += "00";
            } else if (a[i] < 1000) {
                res += "0";
            }
        }
        res += std::to_string(a[i]);
    }
    return res;
}
