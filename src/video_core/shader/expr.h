// Copyright 2019 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <variant>

#include "video_core/engines/shader_bytecode.h"

namespace VideoCommon::Shader {

using Tegra::Shader::ConditionCode;
using Tegra::Shader::Pred;

class ExprAnd;
class ExprOr;
class ExprNot;
class ExprPredicate;
class ExprCondCode;
class ExprVar;
class ExprBoolean;

using ExprData =
    std::variant<ExprVar, ExprCondCode, ExprPredicate, ExprNot, ExprOr, ExprAnd, ExprBoolean>;
using Expr = std::shared_ptr<ExprData>;

class ExprAnd final {
public:
    explicit ExprAnd(Expr a, Expr b) : operand1{a}, operand2{b} {}

    bool operator==(const ExprAnd& b) const;

    Expr operand1;
    Expr operand2;
};

class ExprOr final {
public:
    explicit ExprOr(Expr a, Expr b) : operand1{a}, operand2{b} {}

    bool operator==(const ExprOr& b) const;

    Expr operand1;
    Expr operand2;
};

class ExprNot final {
public:
    explicit ExprNot(Expr a) : operand1{a} {}

    bool operator==(const ExprNot& b) const;

    Expr operand1;
};

class ExprVar final {
public:
    explicit ExprVar(u32 index) : var_index{index} {}

    bool operator==(const ExprVar& b) const {
        return var_index == b.var_index;
    }

    u32 var_index;
};

class ExprPredicate final {
public:
    explicit ExprPredicate(u32 predicate) : predicate{predicate} {}

    bool operator==(const ExprPredicate& b) const {
        return predicate == b.predicate;
    }

    u32 predicate;
};

class ExprCondCode final {
public:
    explicit ExprCondCode(ConditionCode cc) : cc{cc} {}

    bool operator==(const ExprCondCode& b) const {
        return cc == b.cc;
    }

    ConditionCode cc;
};

class ExprBoolean final {
public:
    explicit ExprBoolean(bool val) : value{val} {}

    bool operator==(const ExprBoolean& b) const {
        return value == b.value;
    }

    bool value;
};

template <typename T, typename... Args>
Expr MakeExpr(Args&&... args) {
    static_assert(std::is_convertible_v<T, ExprData>);
    return std::make_shared<ExprData>(T(std::forward<Args>(args)...));
}

bool ExprAreEqual(Expr first, Expr second);

bool ExprAreOpposite(Expr first, Expr second);

Expr MakeExprNot(Expr first);

Expr MakeExprAnd(Expr first, Expr second);

Expr MakeExprOr(Expr first, Expr second);

bool ExprIsTrue(Expr first);

} // namespace VideoCommon::Shader
