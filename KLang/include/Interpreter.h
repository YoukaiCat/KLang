/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QMap>

#include "Lexeme.h"
#include "Source.h"
#include "Token.h"
#include "Node.h"
#include "Error.h"

#include "KLang_global.h"

class KLANG_LIBRARY_EXPORT Interpreter : public QObject
{
    Q_OBJECT
public:
    explicit Interpreter(const Node & ast, QObject * parent = 0);
    QMap<QString, double> evaluate();
    QMap<QString, double> getVariables() const;

private:
    void declarations(const Node & declarationsNode);
    void addVariable(const Node & id);
    void assignments(const Node & assignmentsNode);
    double expression(const Node & node) const;
    double op_division(const Node & leftOperandNode, const Node & rightOperandNode) const;
    double op_and(double leftOperand, double rightOperand) const;
    double op_or(double leftOperand, double rightOperand) const;
    double op_not(double operand) const;
    double variableValue(const Token & token) const;
    double getNumber(const Token & token) const;

    Node ast;
    QMap<QString, double> variables;
    const double DEFAULT_REAL_VAL = 0.0;
};

#endif // INTERPRETER_H
