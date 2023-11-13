#include <vector>
#include <unordered_map>
#include <any>
#include <string>
#include <utility>
#include "Python3ParserBaseVisitor.h"

struct function {
    std::vector<std::pair<std::string, std::any>> parameters; // 形参和默认值
    Python3Parser::SuiteContext *suites;
};

class scope_stack {
private:
    struct scope {
        bool is_enter = false;
        std::unordered_map<std::string, std::any> data;
    };
    std::vector<scope> scopes;
    size_t current_entered; // 仅在enter和destroy时要修改
public:
    scope_stack();

    void var_assign(const std::string &var_name, const std::any &var_value);

    void var_init_assign(const std::string &var_name, const std::any &var_value);

    std::pair<bool, std::any> var_visit(const std::string &);

    std::pair<bool, std::any> var_init_visit(const std::string &);

    void create();

    void destroy();

    void enter();
};