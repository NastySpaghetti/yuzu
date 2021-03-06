// Copyright 2019 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <variant>

#include "video_core/shader/expr.h"

namespace VideoCommon::Shader {

bool ExprAnd::operator==(const ExprAnd& b) const {
    return (*operand1 == *b.operand1) && (*operand2 == *b.operand2);
}

bool ExprOr::operator==(const ExprOr& b) const {
    return (*operand1 == *b.operand1) && (*operand2 == *b.operand2);
}

bool ExprNot::operator==(const ExprNot& b) const {
    return (*operand1 == *b.operand1);
}

bool ExprIsBoolean(Expr expr) {
    return std::holds_alternative<ExprBoolean>(*expr);
}

bool ExprBooleanGet(Expr expr) {
    return std::get_if<ExprBoolean>(expr.get())->value;
}

Expr MakeExprNot(Expr first) {
    if (std::holds_alternative<ExprNot>(*first)) {
        return std::get_if<ExprNot>(first.get())->operand1;
    }
    return MakeExpr<ExprNot>(first);
}

Expr MakeExprAnd(Expr first, Expr second) {
    if (ExprIsBoolean(first)) {
        return ExprBooleanGet(first) ? second : first;
    }
    if (ExprIsBoolean(second)) {
        return ExprBooleanGet(second) ? first : second;
    }
    return MakeExpr<ExprAnd>(first, second);
}

Expr MakeExprOr(Expr first, Expr second) {
    if (ExprIsBoolean(first)) {
        return ExprBooleanGet(first) ? first : second;
    }
    if (ExprIsBoolean(second)) {
        return ExprBooleanGet(second) ? second : first;
    }
    return MakeExpr<ExprOr>(first, second);
}

bool ExprAreEqual(Expr first, Expr second) {
    return (*first) == (*second);
}

bool ExprAreOpposite(Expr first, Expr second) {
    if (std::holds_alternative<ExprNot>(*first)) {
        return ExprAreEqual(std::get_if<ExprNot>(first.get())->operand1, second);
    }
    if (std::holds_alternative<ExprNot>(*second)) {
        return ExprAreEqual(std::get_if<ExprNot>(second.get())->operand1, first);
    }
    return false;
}

bool ExprIsTrue(Expr first) {
    if (ExprIsBoolean(first)) {
        return ExprBooleanGet(first);
    }
    return false;
}

} // namespace VideoCommon::Shader
