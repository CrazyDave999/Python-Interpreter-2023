#include <any>
#include <string>
#include "int2048.h"
#include <cmath>


using ll = sjtu::int2048;

int get_type(std::any val);

bool to_bool(const std::any &val);

ll to_ll(const std::any &val);

double to_double(const std::any &val);

std::string to_string(std::any &val);

std::any operator+(const std::any &lhs, const std::any &rhs);

std::any operator-(const std::any &lhs, const std::any &rhs);

std::any operator*(const std::any &lhs, const std::any &rhs);

std::any operator/(const std::any &lhs, const std::any &rhs);

std::any operator%(const std::any &lhs, const std::any &rhs);

std::any operator&(const std::any &lhs, const std::any &rhs);

std::any operator-(const std::any &rhs);

bool operator==(const std::any &lhs, const std::any &rhs);

bool operator!=(const std::any &lhs, const std::any &rhs);

bool operator<(const std::any &lhs, const std::any &rhs);

bool operator>(const std::any &lhs, const std::any &rhs);

bool operator<=(const std::any &lhs, const std::any &rhs);

bool operator>=(const std::any &lhs, const std::any &rhs);

std::vector<std::string> split_by_comma(const std::string &str);
