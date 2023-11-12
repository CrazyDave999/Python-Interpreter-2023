#include "Evalvisitor.h"
#include "scope.h"
#include "utility.h"
#include "int2048.h"

using ll = sjtu::int2048;
scope var_scope, func_scope;

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    std::string func_name = ctx->NAME()->getText();
    auto parameters = std::any_cast<std::vector<std::pair<std::string, std::any>>>(visitParameters(ctx->parameters()));
    function func_value{parameters, ctx->suite()};
    func_scope.var_register(func_name, func_value);
    return {};
}

std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
    if (ctx->typedargslist()) {
        return visitTypedargslist(ctx->typedargslist());
    } else {
        return std::vector<std::pair<std::string, std::any>>();
    }
}

std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
    std::vector<std::pair<std::string, std::any>> res;
    auto name_array = ctx->tfpdef();
    auto value_array = ctx->test();
    auto init_start = name_array.size() - value_array.size();
    for (int i = 0; i < name_array.size(); ++i) {
        if (i < init_start) {
            res.emplace_back(name_array[i]->NAME()->getText(), std::any());
        } else {
            res.emplace_back(name_array[i]->NAME()->getText(), visitTest(value_array[i - init_start]));
        }
    }
    return res;
}

std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    if (ctx->simple_stmt()) {
        auto ret = visitSimple_stmt(ctx->simple_stmt());
        return ret;
    } else if (ctx->compound_stmt()) {
        return visitCompound_stmt(ctx->compound_stmt());
    }
    return {};
//    return visitChildren(ctx);
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    if (ctx->small_stmt()) {
        auto ret = visitSmall_stmt(ctx->small_stmt());
        return ret;
    }
    return {};
//    return visitChildren(ctx);

}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    if (ctx->expr_stmt()) {
        return visitExpr_stmt(ctx->expr_stmt());
    } else if (ctx->flow_stmt()) {
        auto ret = visitFlow_stmt(ctx->flow_stmt());
        return ret;
    }
//    return visitChildren(ctx);

}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    auto test_list_array = ctx->testlist();
    if (ctx->augassign()) {
        auto op = ctx->augassign()->getText();
        auto var_name = test_list_array[0]->getText();
        auto visit = var_scope.var_visit(var_name);


        auto aug = std::any_cast<std::vector<std::any>>(visitTestlist(test_list_array[1]))[0];
        if (op == "+=") {
            var_scope.var_register(var_name, visit.second + aug);
        } else if (op == "-=") {
            var_scope.var_register(var_name, visit.second - aug);
        } else if (op == "*=") {
            var_scope.var_register(var_name, visit.second * aug);
        } else if (op == "/=") {
            var_scope.var_register(var_name, visit.second / aug);
        } else if (op == "//=") {
            var_scope.var_register(var_name, visit.second & aug);
        } else if (op == "%=") {
            var_scope.var_register(var_name, visit.second % aug);
        }
        return var_scope.var_visit(var_name).second;
    } else {
        if (test_list_array.size() == 1) {
            return visitTestlist(test_list_array[0]);
        } else {
            for (int i = test_list_array.size() - 2; i >= 0; --i) {
//         连等有问题
                auto rhs_array = std::any_cast<std::vector<std::any>>(visitTestlist(test_list_array[i + 1]));
                auto name_array = split_by_comma(test_list_array[i]->getText());
                for (int j = 0; j < name_array.size(); ++j) {
                    var_scope.var_register(name_array[j], rhs_array[j]);
                }
            }
            return visitTestlist(test_list_array[0]);
        }
    }
}

std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    if (ctx->break_stmt()) {
        return std::pair<int, std::any>(0, std::vector<std::any>());
    } else if (ctx->continue_stmt()) {
        return std::pair<int, std::any>(1, std::vector<std::any>());
    } else if (ctx->return_stmt()) {
        return std::pair<int, std::any>(2, visitReturn_stmt(ctx->return_stmt()));
    }
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
    if (ctx->testlist()) {
        return visitTestlist(ctx->testlist());
    } else {
        return std::vector<std::any>();
    }
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    auto test_array = ctx->test();
    auto suite_array = ctx->suite();
    for (int i = 0; i < test_array.size(); ++i) {
        if (to_bool(visitTest(test_array[i]))) {
            return visitSuite(suite_array[i]);
        }
    }
    if (test_array.size() < suite_array.size()) {
        return visitSuite(suite_array.back());
    }
    return {};
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    auto test = ctx->test();
    auto suite = ctx->suite();
    while (to_bool(visitTest(test))) {
        auto ret = visitSuite(suite);
        if (std::any_cast<std::pair<int, std::any>>(&ret)) {
            auto type = std::any_cast<std::pair<int, std::any>>(ret).first;
            if (type == 0) {
                break;
            } else if (type == 2) {
                return ret;
            }
        }
    }
    return {};
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    if (ctx->simple_stmt()) {
        return visitSimple_stmt(ctx->simple_stmt());
    } else {
        auto stmt_array = ctx->stmt();
        for (auto &i: stmt_array) {
            auto ret = visitStmt(i);
            if (std::any_cast<std::pair<int, std::any>>(&ret)) {
                return ret;
            }
        }
        return {};
    }
}


std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    auto test_array = ctx->and_test();
    if (test_array.size() == 1) {
        return visitAnd_test(test_array[0]); // 有可能是各种类型
    }
    for (auto test: test_array) {
        auto is_true = std::any_cast<bool>(visitAnd_test(test));
        if (is_true) {
            return true;
        }
    }
    return false;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    auto test_array = ctx->not_test();
    if (test_array.size() == 1) {
        return visitNot_test(test_array[0]); // 有可能是各种类型
    }
    for (auto test: test_array) {
        auto is_true = std::any_cast<bool>(visitNot_test(test));
        if (!is_true) {
            return false;
        }
    }
    return true;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    if (ctx->comparison()) {
        return visitComparison(ctx->comparison());
    } else if (ctx->not_test()) {
        auto ret = visitNot_test(ctx->not_test());
        if (std::any_cast<bool>(&ret)) {
            return !std::any_cast<bool>(ret);
        } else {
            return ret;
        }
    }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    auto expr_array = ctx->arith_expr();
    auto op_array = ctx->comp_op();
    auto last_num = visitArith_expr(expr_array[0]);
    if (expr_array.size() == 1) {
        return last_num;
    }
    for (int i = 1; i < expr_array.size(); ++i) {
        auto cur_num = visitArith_expr(expr_array[i]);
        auto op = op_array[i - 1]->getText();
        bool res;
        if (op == "==") {
            res = last_num == cur_num;
        } else if (op == "<") {
            res = last_num < cur_num;
        } else if (op == ">") {
            res = last_num > cur_num;
        } else if (op == "<=") {
            res = last_num <= cur_num;
        } else if (op == ">=") {
            res = last_num >= cur_num;
        } else if (op == "!=") {
            res = last_num != cur_num;
        }
        if (!res) {
            return false;
        }
        last_num = cur_num;
    }
    return true;
}

std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
    return visitChildren(ctx);
}


std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    auto term_array = ctx->term();
    auto op_array = ctx->addorsub_op();
    auto res = visitTerm(term_array[0]);
    if (term_array.size() == 1) {
        return res;
    }
    for (int i = 1; i < term_array.size(); ++i) {
        auto op = op_array[i - 1]->getText();
        auto rhs = visitTerm(term_array[i]);
        if (op == "+") {
            res = res + rhs;
        } else if (op == "-") {
            res = res - rhs;
        }
    }
    return res;
}

std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    auto fac_array = ctx->factor();
    auto op_array = ctx->muldivmod_op();
    auto res = visitFactor(fac_array[0]);
    if (fac_array.size() == 1) {
        return res;
    }
    for (int i = 1; i < fac_array.size(); ++i) {
        auto op = op_array[i - 1]->getText();
        auto rhs = visitFactor(fac_array[i]);
        if (op == "*") {
            res = res * rhs;
        } else if (op == "/") {
            res = res / rhs;
        } else if (op == "//") {
            res = res & rhs;
        } else if (op == "%") {
            res = res % rhs;
        }
    }
    return res;
}

std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
    return visitChildren(ctx);
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
    if (ctx->atom_expr()) {
        return visitAtom_expr(ctx->atom_expr());
    } else if (ctx->factor()) {
        if (ctx->MINUS()) {
            return -visitFactor(ctx->factor());
        } else {
            return visitFactor(ctx->factor());
        }
    }
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    if (!ctx->trailer()) {
        return visitAtom(ctx->atom());
    }
    auto func_name = ctx->atom()->getText();
    var_scope.create();
    auto pr = std::any_cast<std::pair<int, std::vector<std::any>>>(visitTrailer(ctx->trailer()));

    auto kw_start = pr.first;
    auto args_array = pr.second;

    if (func_name == "print") {
        if (args_array.size() == 1) {
            if (std::any_cast<std::vector<std::any>>(&args_array[0])) {
                auto func_ret_array = std::any_cast<std::vector<std::any>>(args_array[0]);
                for (auto &i: func_ret_array) {
                    std::cout << to_string(i) << ' ';
                }
                std::cout << std::endl;
                var_scope.destroy();
                return {};
            }
        }
        for (auto &i: args_array) {
            std::cout << to_string(i) << ' ';
        }
        std::cout << std::endl;
        var_scope.destroy();
        return {};
    } else if (func_name == "bool") {
        var_scope.destroy();
        return to_bool(args_array[0]);
    } else if (func_name == "int") {
        var_scope.destroy();
        return to_ll(args_array[0]);
    } else if (func_name == "float") {
        var_scope.destroy();
        return to_double(args_array[0]);
    } else if (func_name == "str") {
        var_scope.destroy();
        return to_string(args_array[0]);
    }

    auto visit = func_scope.var_visit(func_name);
    if (visit.first) {
        auto func = std::any_cast<function>(visit.second);
        for (int i = 0; i < func.parameters.size(); ++i) {
            // 对于未被 keyword argument 初始化的形参，优先用 positional argument 初始化，再考虑用默认值初始化
            if (i < kw_start) {
                auto var_name = func.parameters[i].first;
                auto var_val = args_array[i];
                var_scope.get_current_map()[var_name] = var_val;
            } else {
                auto var_name = func.parameters[i].first;
                auto var_val = func.parameters[i].second;
                if (!var_scope.get_current_map().count(var_name)) {
                    var_scope.get_current_map()[var_name] = var_val;
                }
            }
        }

        var_scope.enter();
        auto ret = visitSuite(func.suites);
        var_scope.destroy();
        if (std::any_cast<std::pair<int, std::any>>(&ret)) {
            auto pr_flow = std::any_cast<std::pair<int, std::any>>(ret);
            if (pr_flow.first == 2) {
                if (std::any_cast<std::vector<std::any>>(&pr_flow.second)) {
                    auto func_ret_array = std::any_cast<std::vector<std::any>>(pr_flow.second); // 函数返回了一个右值元组
                    if (func_ret_array.size() == 1) {
                        return func_ret_array[0];
                    }
                }
                return pr_flow.second;
            }
        }
    }
    return {};
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    if (ctx->arglist()) {
        return visitArglist(ctx->arglist());
    } else {
        return std::pair<int, std::vector<std::any>>(-1, {});
    }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    if (ctx->NAME()) {
        auto var_name = ctx->NAME()->getText();
        auto visit = var_scope.var_visit(var_name);
        if (visit.first) {
            return visit.second;
        }
    } else if (ctx->NUMBER()) {
        auto num = ctx->NUMBER()->getText();
        if (num.find('.') == std::string::npos) {
            return to_ll(num);
        } else {
            return to_double(num);
        }
    } else if (ctx->TRUE()) {
        return true;
    } else if (ctx->FALSE()) {
        return false;
    } else if (ctx->NONE()) {
        return {};
    } else if (ctx->test()) {
        return visitTest(ctx->test());
    } else {
        auto str_array = ctx->STRING();
        std::string res;
        for (auto &i: str_array) {
            auto str = i->getText();
            res += str.substr(1, str.size() - 2);
        }
        return res;
    }
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    // 返回包含右值的元组
    auto test_array = ctx->test();
    std::vector<std::any> res;
    for (auto &i: test_array) {
        res.emplace_back(visitTest(i));
    }
    if (res.size() == 1) {
        // 函数调用可能返回元组
        if (std::any_cast<std::vector<std::any>>(&res[0])) {
            return res[0];
        }
    }
    return res;
}

std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto args_array = ctx->argument();
    std::vector<std::any> res;
    int kw_start = args_array.size();
    for (int i = 0; i < args_array.size(); ++i) {
        if (args_array[i]->ASSIGN() && kw_start == args_array.size()) {
            kw_start = i;
        }
        res.push_back(visitArgument(args_array[i]));
    }
    return std::pair<int, std::vector<std::any>>(kw_start, res);
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
    auto test_array = ctx->test();
    if (!ctx->ASSIGN()) {
        return visitTest(test_array[0]); // 作为值返回
    } else {
        auto var_name = test_array[0]->getText(); // 在新函数作用域中新建变量
        auto var_val = visitTest(test_array[1]);
        var_scope.get_current_map()[var_name] = var_val; // 强行在当前作用域新建变量
        return var_val;
    }
}



