#include "utility.h"

int get_type(const std::any val) {
    if (std::any_cast<bool>(&val)) {
        return 0;
    } else if (std::any_cast<ll>(&val)) {
        return 1;
    } else if (std::any_cast<double>(&val)) {
        return 2;
    } else if (std::any_cast<std::string>(&val)) {
        return 3;
    } else if (!val.has_value()) {
        return -1;
    } else {
        return 4;
    }
}

bool to_bool(const std::any &val) {
    int tp = get_type(val);
    if (tp == 0) {
        return std::any_cast<bool>(val);
    } else if (tp == 1) {
        return std::any_cast<ll>(val) != 0;
    } else if (tp == 2) {
        return std::any_cast<double>(val) != 0.0;
    } else if (tp == 3) {
        return !std::any_cast<std::string>(val).empty();
    } else {
        return {};
    }
}

ll to_ll(const std::any &val) {
    int tp = get_type(val);
    if (tp == 0) {
        return {std::any_cast<bool>(val)};
    } else if (tp == 1) {
        return std::any_cast<ll>(val);
    } else if (tp == 2) {
        return {(long long) std::any_cast<double>(val)};
    } else if (tp == 3) {
        return {std::any_cast<std::string>(val)};
    } else {
        return {};
    }
}

double to_double(const std::any &val) {
    int tp = get_type(val);
    if (tp == 0) {
        return std::any_cast<bool>(val) ? 1.0 : 0.0;
    } else if (tp == 1) {
        return std::any_cast<ll>(val).to_double();
    } else if (tp == 2) {
        return std::any_cast<double>(val);
    } else if (tp == 3) {
        auto str = std::any_cast<std::string>(val);
        bool neg = false, flag = false;
        double ans = 0, dec = 0, pow = 1;
        for (char i: str) {
            if (i == '-') {
                neg = true;
            } else if (i == '.') {
                flag = true;
            } else {
                if (!flag) {
                    ans *= 10;
                    ans += i - '0';
                } else {
                    dec *= 10;
                    dec += i - '0';
                    pow *= 10;
                }
            }
        }
        ans += dec / pow;
        if (neg) {
            ans = -ans;
        }
        return ans;
    } else {
        return {};
    }
}

std::string to_string(std::any &val) {
    int tp = get_type(val);
    if (tp == 0) {
        return std::any_cast<bool>(val) ? "True" : "False";
    } else if (tp == 1) {
        return std::any_cast<ll>(val).to_string();
    } else if (tp == 2) {
        char s[200];
        sprintf(s, "%.6lf", std::any_cast<double>(val));
        return {s};
    } else if (tp == 3) {
        return std::any_cast<std::string>(val);
    } else if (tp == -1) {
        return "None";
    } else {
        return {};
    }
}

std::any operator+(const std::any &lhs, const std::any &rhs) {
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == 3 && tp2 == 3) {
        return std::any_cast<std::string>(lhs) + std::any_cast<std::string>(rhs);
    }
    if (tp1 == 2 || tp2 == 2) {
        return to_double(lhs) + to_double(rhs);
    }
    return to_ll(lhs) + to_ll(rhs);
}

std::any operator-(const std::any &lhs, const std::any &rhs) {
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == 2 || tp2 == 2) {
        return to_double(lhs) - to_double(rhs);
    }
    return to_ll(lhs) - to_ll(rhs);
}

std::any operator*(const std::any &lhs, const std::any &rhs) {
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == 3 && tp2 == 1) {
        // string * ll
        std::string res;
        auto add = std::any_cast<std::string>(lhs);
        auto upper = std::any_cast<ll>(rhs);
        if (upper <= 0) {
            return res;
        }
        for (ll i = 0; i < upper; i += 1) {
            res += add;
        }
        return res;
    }
    if (tp1 == 1 && tp2 == 3) {
        // ll * string
        std::string res;
        auto add = std::any_cast<std::string>(rhs);
        auto upper = std::any_cast<ll>(lhs);
        if (upper <= 0) {
            return res;
        }
        for (ll i = 0; i < upper; i += 1) {
            res += add;
        }
        return res;
    }
    if (tp1 == 2 || tp2 == 2) {
        return to_double(lhs) * to_double(rhs);
    }
    return to_ll(lhs) * to_ll(rhs);
}

std::any operator/(const std::any &lhs, const std::any &rhs) {
    return to_double(lhs) / to_double(rhs);
}


std::any operator%(const std::any &lhs, const std::any &rhs) {
    return to_ll(lhs) % to_ll(rhs);
}

std::any operator&(const std::any &lhs, const std::any &rhs) { // 整除
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == 2 || tp2 == 2) {
        return ll(floor(to_double(lhs) / to_double(rhs)));
    }
    return to_ll(lhs) / to_ll(rhs);
}

std::any operator-(const std::any &rhs) {
    int tp = get_type(rhs);
    if (tp == 0) {
        return -std::any_cast<bool>(rhs);
    } else if (tp == 1) {
        return -std::any_cast<ll>(rhs);
    } else if (tp == 2) {
        return -std::any_cast<double>(rhs);
    } else {
        return {};
    }
}

bool operator==(const std::any &lhs, const std::any &rhs) {
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == -1 || tp2 == -1) {
        if (tp1 == tp2) {
            return true;
        } else {
            return false;
        }
    }
    if (tp1 == 3 && tp2 == 3) {
        return std::any_cast<std::string>(lhs) == std::any_cast<std::string>(rhs);
    } else if (tp1 == 3 || tp2 == 3) {
        return false;
    }
    if (tp1 == 0 && tp2 == 0) {
        return std::any_cast<bool>(lhs) == std::any_cast<bool>(rhs);
    }
    if (tp1 == 2 || tp2 == 2) {
        return to_double(lhs) == to_double(rhs);
    }
    return to_ll(lhs) == to_ll(rhs);
}

bool operator!=(const std::any &lhs, const std::any &rhs) {
    return !(lhs == rhs);
}

bool operator<(const std::any &lhs, const std::any &rhs) {
    int tp1 = get_type(lhs), tp2 = get_type(rhs);
    if (tp1 == 3 && tp2 == 3) {
        return std::any_cast<std::string>(lhs) < std::any_cast<std::string>(rhs);
    }
    if (tp1 == 2 || tp2 == 2) {
        return to_double(lhs) < to_double(rhs);
    }
    return to_ll(lhs) < to_ll(rhs);
}

bool operator>(const std::any &lhs, const std::any &rhs) {
    return rhs < lhs;
}

bool operator<=(const std::any &lhs, const std::any &rhs) {
    return !(lhs > rhs);
}

bool operator>=(const std::any &lhs, const std::any &rhs) {
    return !(lhs < rhs);
}

std::vector<std::string> split_by_comma(const std::string &str) {
    int cur = 0, len = str.size();
    std::vector<std::string> res;
    while (cur < len) {
        for (int j = cur; j <= len; j++) {
            if (j == len || str[j] == ',') {
                res.push_back(str.substr(cur, j - cur));
                cur = j + 1;
                break;
            }
        }
    }
    return res;
}