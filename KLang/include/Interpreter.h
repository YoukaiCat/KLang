/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "KLang_global.h"

#include <memory>

#include <QObject>
#include <QMap>

#include "Token.h"
#include "Node.h"
#include "Variable.h"

class KLANG_LIBRARY_EXPORT Interpreter : public QObject
{
    Q_OBJECT
public:
    explicit Interpreter(const shared_ptr<Node> ast, QObject * parent = 0);
    shared_ptr<QMap<QString, shared_ptr<Variable>>> evaluate();
    shared_ptr<QMap<QString, shared_ptr<Variable>>> getVariables() const;

private:
    void declarations(const shared_ptr<Node> declarationsNode);
    void addVariable(const shared_ptr<Node> id);
    void assignments(const shared_ptr<Node> assignmentsNode);
    double expression(const shared_ptr<Node> node) const;
    double opDivision(const shared_ptr<Node> leftOperandNode, const shared_ptr<Node> rightOperandNode) const;
    double opAnd(double leftOperand, double rightOperand) const;
    double opOr(double leftOperand, double rightOperand) const;
    double opNot(double operand) const;
    double variableValue(const Token & token) const;
    double getNumber(const Token & token) const;

    shared_ptr<Node> ast;
    shared_ptr<QMap<QString, shared_ptr<Variable>>> variables;
};

#endif // INTERPRETER_H
