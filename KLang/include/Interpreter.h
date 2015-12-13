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

class KLANG_LIBRARY_EXPORT Interpreter : public QObject
{
    Q_OBJECT
public:
    explicit Interpreter(const shared_ptr<Node> ast, QObject * parent = 0);
    shared_ptr<QMap<QString, double>> evaluate();
    shared_ptr<QMap<QString, double>> getVariables() const;

private:
    void declarations(const shared_ptr<Node> declarationsNode);
    void addVariable(const shared_ptr<Node> id);
    void assignments(const shared_ptr<Node> assignmentsNode);
    double expression(const shared_ptr<Node> node) const;
    double op_division(const shared_ptr<Node> leftOperandNode, const shared_ptr<Node> rightOperandNode) const;
    double op_and(double leftOperand, double rightOperand) const;
    double op_or(double leftOperand, double rightOperand) const;
    double op_not(double operand) const;
    double variableValue(const Token & token) const;
    double getNumber(const Token & token) const;

    shared_ptr<Node> ast;
    shared_ptr<QMap<QString, double>> variables;
    const double DEFAULT_REAL_VAL = 0.0;
};

#endif // INTERPRETER_H
