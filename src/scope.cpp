#include "scope.h"

scope_stack::scope_stack() {
    create(); // 全局变量
    enter();
}

void scope_stack::var_assign(const std::string &var_name, const std::any &var_value) {
    if (scopes[current_entered].data.count(var_name)) {
        scopes[current_entered].data[var_name] = var_value;
    } else if (scopes.front().data.count(var_name)) {
        scopes.front().data[var_name] = var_value;
    } else {
        scopes[current_entered].data[var_name] = var_value;
    }
}

void scope_stack::var_init_assign(const std::string &var_name, const std::any &var_value) {
    scopes.back().data[var_name] = var_value;
}

std::pair<bool, std::any> scope_stack::var_visit(const std::string &var_name) {
    auto &cur = scopes[current_entered].data;
    auto itr = cur.find(var_name);
    if (itr == cur.end()) {
        itr = scopes.front().data.find(var_name);
        if (itr == scopes.front().data.end()) {
            return {false, 0};
        }
    }
    return {true, itr->second};
}

std::pair<bool, std::any> scope_stack::var_init_visit(const std::string &var_name) {
    auto itr = scopes.back().data.find(var_name);
    if (itr == scopes.back().data.end()) {
        return {false, {}};
    }
    return {true, itr->second};
}

void scope_stack::create() {
    scopes.emplace_back(); // 调用函数时创建新的作用域，并在新的作用域中初始化变量，但是并未进入这个作用域，因为还要使用原作用域变量
}

void scope_stack::destroy() {
    scopes.pop_back();
    --current_entered;
    while (!scopes[current_entered].is_enter) {
        --current_entered;
    }
}

void scope_stack::enter() {
    scopes.back().is_enter = true;
    current_entered = scopes.size() - 1;
}


