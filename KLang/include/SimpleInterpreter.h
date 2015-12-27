#ifndef SIMPLEINTERPRETER_H
#define SIMPLEINTERPRETER_H

#include "KLang_global.h"

#include <memory>

#include <QObject>
#include <QStack>
#include <QString>
#include <QMap>

#include "Token.h"
#include "Node.h"
#include "Lexeme.h"
#include "Variable.h"

using std::shared_ptr;

class KLANG_LIBRARY_EXPORT SimpleInterpreter : public QObject
{
    Q_OBJECT
public:
    explicit SimpleInterpreter(const shared_ptr<QList<Token>> tokens, QObject * parent = 0);
    shared_ptr<QMap<QString, shared_ptr<Variable>>> evaluate();
    shared_ptr<QMap<QString, shared_ptr<Variable>>> getVariables() const;

private:
     void program();
     void declaration();
     void addVariable(const Token & token);
     void assignment();
     void setVariable(const Token & token, double value);
     double additiveExpression();
     double multiplicativeExpression();
     double opDivision(const Token & sign, double leftOperand, double rightOperand);
     double booleanBinaryExpression();
     double notExpression();
     double base();
     double variableValue(const Token & token) const;
     double numberValue(const Token & token) const;

     Lexeme lastDeclaredType;
     bool expectUnaryMinus = true;

     shared_ptr<QList<Token>> tokens;
     QStack<int> parenthesisIndices;

     shared_ptr<QMap<QString, shared_ptr<Variable>>> variables;
};

#endif // SIMPLEINTERPRETER_H
