/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Interpreter.h"
#include "Error.h"

Interpreter::Interpreter(const shared_ptr<Node> ast, QObject * parent)
    : QObject(parent)
    , ast(ast)
    , variables(std::make_shared<QMap<QString, shared_ptr<Variable>>>(QMap<QString, shared_ptr<Variable>>()))
{}

shared_ptr<QMap<QString, shared_ptr<Variable>>> Interpreter::evaluate()
{
    declarations(ast->at(0));
    assignments(ast->at(1));
    return variables;
}

shared_ptr<QMap<QString, shared_ptr<Variable>>> Interpreter::getVariables() const
{
    return variables;
}

void Interpreter::declarations(const shared_ptr<Node> declarationsNode)
{
    for (auto declaration : declarationsNode->getChildren()) {
        if (declaration->getToken().getType() == Lexeme::SingleDeclaration) {
            addVariable(declaration->at(0));
        } else if (declaration->getToken().getType() == Lexeme::MultipleDeclaration) {
            for (auto id : declaration->getChildren()) {
                addVariable(id);
            }
        }
    }
}

void Interpreter::addVariable(const shared_ptr<Node> id)
{
    auto name = id->getToken().getValue();
    if (variables->contains(name)) {
        throw Error(200, "Переменная с именем '" + name + "' уже объявлена", id->getToken().getIndexBegin(), id->getToken().getIndexEnd());
    } else {
        variables->insert(name, std::make_shared<Variable>(Variable(name)));
    }
}

void Interpreter::assignments(const shared_ptr<Node> assignmentsNode)
{
    for (auto assignment : assignmentsNode->getChildren()) {
        auto variableToken = assignment->at(0)->getToken();
        auto variableName = variableToken.getValue();
        if (variables->contains(variableName)) {
            auto variable = variables->value(variableName);
            auto value = expression(assignment->at(1));
            variable->setValue(value);
        } else {
            throw Error(203, "Переменная '" + variableName + "' не определена", variableToken.getIndexBegin(), variableToken.getIndexEnd());
        }
    }
}

double Interpreter::expression(const shared_ptr<Node> node) const
{
    auto token = node->getToken();
    switch (token.getType()) {
    case Lexeme::UnaryMinus:
        return - expression(node->at(0));
    case Lexeme::Plus:
        return expression(node->at(0)) + expression(node->at(1));
    case Lexeme::Minus:
        return expression(node->at(0)) - expression(node->at(1));
    case Lexeme::Multiply:
        return expression(node->at(0)) * expression(node->at(1));
    case Lexeme::Divide:
        return opDivision(node->at(0), node->at(1));
    case Lexeme::And:
        return opAnd(expression(node->at(0)), expression(node->at(1)));
    case Lexeme::Or:
        return opOr(expression(node->at(0)), expression(node->at(1)));
    case Lexeme::Not:
        return opNot(expression(node->at(0)));
    case Lexeme::Num:
        return getNumber(token);
    case Lexeme::Id:
        return variableValue(token);
    default:
        throw Error(201, "Неизвестный оператор", node->getToken().getIndexBegin(), node->getToken().getIndexEnd());
    }
}

// TODO было бы правильнее подсвечивать знак деления и правый операнд
double Interpreter::opDivision(const shared_ptr<Node> leftOperandNode, const shared_ptr<Node> rightOperandNode) const
{
    double leftOperand = expression(leftOperandNode);
    double rightOperand = expression(rightOperandNode);
    if (rightOperand == 0.0) {
        throw Error(202, "Деление на ноль!",
                    leftOperandNode->getChildren().at(0)->getToken().getIndexBegin(),
                    rightOperandNode->getChildren().at(1)->getToken().getIndexEnd());
    } else {
        return leftOperand / rightOperand;
    }
}

// Логика операторов "как в си"
// x == 0.0 => false
// x != 0.0 => true
// Операторы возвращают 1.0, когда вычисляются в true
double Interpreter::opAnd(double leftOperand, double rightOperand) const
{
    // Если оба операнда не равны нулю
    if (leftOperand != 0.0 && rightOperand != 0.0) {
        return 1.0; // true
    } else {
        return 0.0; // false
    }
}

double Interpreter::opOr(double leftOperand, double rightOperand) const
{
    // Если какой-либо из операндов не равен нулю
    if (leftOperand != 0.0 || rightOperand != 0.0) {
        return 1.0; // true
    } else {
        return 0.0; // false
    }
}

double Interpreter::opNot(double operand) const
{
    if (operand == 0.0) {
        return 1.0;
    } else {
        return 0.0;
    }
}

double Interpreter::variableValue(const Token & token) const
{
    auto variableName = token.getValue();
    if (variables->contains(variableName)) {
        auto variable = variables->value(variableName);
        if (variable->isInitialized()) {
            return variable->getValue();
        } else {
            throw Error(203, "Переменная '" + variableName + "' не инициализирована", token.getIndexBegin(), token.getIndexEnd());
        }
    } else {
        throw Error(203, "Переменная '" + variableName + "' не определена", token.getIndexBegin(), token.getIndexEnd());
    }
}

double Interpreter::getNumber(const Token & token) const
{
    bool ok;
    double num = token.getValue().toDouble(&ok);
    if (ok) {
        return num;
    } else {
        throw Error(204, "'" + token.getValue() + "'Невозможно преобразовать в вещественное число.", token.getIndexBegin(), token.getIndexEnd());
    }
}
