/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Interpreter.h"

#include <QDebug>

Interpreter::Interpreter(const Node & ast, QObject * parent)
    : QObject(parent)
    , ast(ast)
{}

QMap<QString, double> Interpreter::evaluate()
{
    declarations(ast.at(0));
    assignments(ast.at(1));
    return variables;
}

QMap<QString, double> Interpreter::getVariables() const
{
    return variables;
}

void Interpreter::declarations(const Node & declarationsNode)
{
    for (Node declaration : declarationsNode.getChildren()) {
        if (declaration.getToken().getType() == Lexeme::SingleDeclaration) {
            addVariable(declaration.at(0));
        } else if (declaration.getToken().getType() == Lexeme::MultipleDeclaration) {
            for (Node id : declaration.getChildren()) {
                addVariable(id);
            }
        }
    }
}

void Interpreter::addVariable(const Node & id)
{
    auto name = id.getToken().getValue();
    if (variables.contains(name)) {
        throw Error("Переменная с именем '" + name + "' уже объявлена", id.getToken().getIndexBegin(), id.getToken().getIndexEnd());
    } else {
        variables[name] = DEFAULT_REAL_VAL;
    }
}

void Interpreter::assignments(const Node & assignmentsNode)
{
    for (Node assignment : assignmentsNode.getChildren()) {
        variables[assignment.at(0).getToken().getValue()] = expression(assignment.at(1));
    }
}

double Interpreter::expression(const Node & node) const
{
    auto token = node.getToken();
    switch (token.getType()) {
    case Lexeme::UnaryMinus:
        return - expression(node.at(0));
    case Lexeme::Plus:
        return expression(node.at(0)) + expression(node.at(1));
    case Lexeme::Minus:
        return expression(node.at(0)) - expression(node.at(1));
    case Lexeme::Multiply:
        return expression(node.at(0)) * expression(node.at(1));
    case Lexeme::Divide:
        return op_division(node.at(0), node.at(1));
    case Lexeme::And:
        return op_and(expression(node.at(0)), expression(node.at(1)));
    case Lexeme::Or:
        return op_or(expression(node.at(0)), expression(node.at(1)));
    case Lexeme::Not:
        return op_not(expression(node.at(0)));
    case Lexeme::Num:
        return getNumber(token);
    case Lexeme::Id:
        return variableValue(token);
    default:
        throw Error("Неизвестный оператор", node.getToken().getIndexBegin(), node.getToken().getIndexEnd());
    }
}

double Interpreter::op_division(const Node & leftOperandNode, const Node & rightOperandNode) const
{
    double leftOperand = expression(leftOperandNode);
    double rightOperand = expression(rightOperandNode);
    if (rightOperand == 0.0) {
        throw Error("Деление на ноль!", leftOperandNode.getToken().getIndexBegin(), rightOperandNode.getToken().getIndexEnd());
    } else {
        return leftOperand / rightOperand;
    }
}

// Логика операторов "как в си".
// Оба оператора ленивые.
double Interpreter::op_and(double leftOperand, double rightOperand) const
{
    // Если какой либо из операндов равен нулю
    if (leftOperand == 0.0 || rightOperand == 0.0) {
        return 0.0; // false
    } else {
        return 1.0; // true
    }
}

double Interpreter::op_or(double leftOperand, double rightOperand) const
{
    // Если какой либо из операндов не равен нулю
    if (leftOperand != 0.0 || rightOperand != 0.0) {
        return 1.0; // true
    } else {
        return 0.0; // false
    }
}

double Interpreter::op_not(double operand) const
{
    if (operand == 0.0) {
        return 1.0;
    } else {
        return 0.0;
    }
}

double Interpreter::variableValue(const Token & token) const
{
    if (variables.contains(token.getValue())) {
        return variables[token.getValue()];
    } else {
        throw Error("Переменная '" + token.getValue() + "' не определена", token.getIndexBegin(), token.getIndexEnd());
    }
}

double Interpreter::getNumber(const Token & token) const
{
    bool ok;
    double num = token.getValue().toDouble(&ok);
    if (ok) {
        return num;
    } else {
        throw Error("'" + token.getValue() + "'Невозможно преобразовать в вещественное число.", token.getIndexBegin(), token.getIndexEnd());
    }
}
