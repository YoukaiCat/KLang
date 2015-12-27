#include "include/SimpleInterpreter.h"

#include "Error.h"

SimpleInterpreter::SimpleInterpreter(const shared_ptr<QList<Token>> tokens, QObject * parent)
    : QObject(parent)
    , tokens(tokens)
    , variables(std::make_shared<QMap<QString, shared_ptr<Variable>>>(QMap<QString, shared_ptr<Variable>>()))
{}

shared_ptr<QMap<QString, shared_ptr<Variable>>> SimpleInterpreter::evaluate()
{
    program();
    return variables;
}

shared_ptr<QMap<QString, shared_ptr<Variable>>> SimpleInterpreter::getVariables() const
{
    return variables;
}

void SimpleInterpreter::program()
{
    auto begin = tokens->takeFirst();
    if (begin.getType() != Lexeme::Begin) {
        throw Error(100, QString("Ожидалось ключевое слово 'Начало', но ") + begin.toString(), begin.getIndexBegin(), begin.getIndexEnd());
    }

    auto first = true;
    do {
        if (!first) tokens->takeFirst();
        first = false;
        declaration();
    } while (tokens->first().getType() == Lexeme::Semicolon);

    do {
        assignment();
    } while (tokens->first().getType() == Lexeme::Id);

    auto end = tokens->takeFirst();
    if (end.getType() == Lexeme::End) {
        auto eof = tokens->takeFirst();
        if (eof.getType() != Lexeme::Eof) {
            throw Error(101, QString("После ключевого слова 'Окончание' не должны находиться ключевые слова, переменные и знаки операций.") + eof.toString(), eof.getIndexBegin(), eof.getIndexEnd());
        }
    } else {
        throw Error(102, QString("Ожидалась переменная, знак бинарной операции или ключевое слово 'Окончание', но ") + end.toString(), end.getIndexBegin(), end.getIndexEnd());
    }
}

void SimpleInterpreter::declaration()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::SingleDeclaration) {
        auto id = tokens->takeFirst();
        if (id.getType() == Lexeme::Id) {
            addVariable(id);
        } else {
            throw Error(103, QString("После слова 'Анализ' ожидалась переменная, но ") + id.toString(), id.getIndexBegin(), id.getIndexEnd());
        }
        lastDeclaredType = Lexeme::SingleDeclaration;
    } else if (token.getType() == Lexeme::MultipleDeclaration) {
        auto first = true;
        do {
            if (!first) {
                tokens->takeFirst();
                auto id = tokens->takeFirst();
                if (id.getType() == Lexeme::Id) {
                    addVariable(id);
                } else {
                    throw Error(104, QString("После запятой ожидалась переменная, но ") + id.toString(), id.getIndexBegin(), id.getIndexEnd());
                }
            } else {
                first = false;
                auto id = tokens->takeFirst();
                if (id.getType() == Lexeme::Id) {
                    addVariable(id);
                } else {
                    throw Error(105, QString("После слова 'Синтез' ожидалась переменная, но ") + id.toString(), id.getIndexBegin(), id.getIndexEnd());
                }
            }
        } while (tokens->first().getType() == Lexeme::Comma);
        lastDeclaredType = Lexeme::MultipleDeclaration;
    } else {
        throw Error(106, QString("Ожидалось ключевое слово 'Анализ' или слово 'Синтез', но ")  + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}

void SimpleInterpreter::addVariable(const Token & token)
{
    auto name = token.getValue();
    if (variables->contains(name)) {
        throw Error(107, "Переменная с именем '" + name + "' уже объявлена", token.getIndexBegin(), token.getIndexEnd());
    } else {
        variables->insert(name, std::make_shared<Variable>(Variable(name)));
    }
}

void SimpleInterpreter::assignment()
{
    auto idToken = tokens->takeFirst();
    if (idToken.getType() == Lexeme::Id) {
        auto eqToken = tokens->takeFirst();
        if (eqToken.getType() == Lexeme::Equality) {
            expectUnaryMinus = true;
            setVariable(idToken, additiveExpression());
        } else {
            throw Error(108, QString("После переменной ожидался символ '=', но ") + eqToken.toString(), eqToken.getIndexBegin(), eqToken.getIndexEnd());
        }
    } else {
        if (lastDeclaredType == Lexeme::MultipleDeclaration) {
            throw Error(109, QString("Ожидался символ ';' или символ ',' или переменная, но ") + idToken.toString(), idToken.getIndexBegin(), idToken.getIndexEnd());
        } else {
            throw Error(110, QString("Ожидался символ ';' или переменная, но ") + idToken.toString(), idToken.getIndexBegin(), idToken.getIndexEnd());
        }
    }
}

void SimpleInterpreter::setVariable(const Token & token, double value)
{
    auto variableName = token.getValue();
    if (variables->contains(variableName)) {
        auto variable = variables->value(variableName);
        variable->setValue(value);
    } else {
        throw Error(111, "Переменная '" + variableName + "' не определена", token.getIndexBegin(), token.getIndexEnd());
    }
}

double SimpleInterpreter::additiveExpression()
{
    //Выражение более высокого приоретета либо его отрицание
    double val;
    if (tokens->first().getType() == Lexeme::Minus) {
        expectUnaryMinus = false;
        //Eсли унарный минус есть, создаётся нода с отрицанием выражения высшего приоретета
        tokens->takeFirst();
        val = - multiplicativeExpression();
    } else {
        //Иначе возвращается нода выражения высшего приоретета
        val = multiplicativeExpression();
    }
    expectUnaryMinus = false;
    while (tokens->first().getType() == Lexeme::Plus || tokens->first().getType() == Lexeme::Minus) {
        auto sign = tokens->takeFirst();
        if (sign.getType() == Lexeme::Plus) {
            val = val + multiplicativeExpression();
        } else {
            val = val - multiplicativeExpression();
        }
    }
    return val;
}

double SimpleInterpreter::multiplicativeExpression()
{
    double val = booleanBinaryExpression();
    while (tokens->first().getType() == Lexeme::Multiply || tokens->first().getType() == Lexeme::Divide) {
        auto sign = tokens->takeFirst();
        if (sign.getType() == Lexeme::Multiply) {
            val = val * booleanBinaryExpression();
        } else {
            val = opDivision(sign, val, booleanBinaryExpression());
        }
    }
    return val;
}

double SimpleInterpreter::opDivision(const Token & sign, double leftOperand, double rightOperand)
{
    if (rightOperand == 0.0) {
        throw Error(112, "Деление на ноль!", sign.getIndexBegin(), sign.getIndexEnd());
    } else {
        return leftOperand / rightOperand;
    }
}

double SimpleInterpreter::booleanBinaryExpression()
{
    auto val = notExpression();
    while (tokens->first().getType() == Lexeme::And || tokens->first().getType() == Lexeme::Or) {
        auto sign = tokens->takeFirst();
        if (sign.getType() == Lexeme::And) {
            val = (bool)val & (bool)notExpression();
        } else {
            val = (bool)val | (bool)notExpression();
        }
    }
    return val;
}

double SimpleInterpreter::notExpression()
{
    if (tokens->first().getType() == Lexeme::Not) {
        expectUnaryMinus = false;
        tokens->takeFirst();
        return !base();
    } else {
        return base();
    }
}

double SimpleInterpreter::base()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Num) {
        expectUnaryMinus = false;
        return numberValue(token);
    } else if(token.getType() == Lexeme::Id) {
        expectUnaryMinus = false;
        return variableValue(token);
    } else if (token.getType() == Lexeme::LeftParentheses) {
        expectUnaryMinus = true;
        parenthesisIndices.push(token.getIndexBegin());
        double val = additiveExpression();
        if (tokens->first().getType() == Lexeme::RightParentheses) {
            expectUnaryMinus = false;
            tokens->takeFirst();
            parenthesisIndices.pop();
            return val;
        } else {
            throw Error(113, QString("Не найдена закрывающая скобка"), parenthesisIndices.pop(), token.getIndexEnd());
        }
    } else {
        if (expectUnaryMinus) {
            throw Error(114, QString("Ожидалось число, переменная, открывающая скобка или унарный минус, но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
        } else {
            throw Error(115, QString("Ожидалось число, переменная или открывающая скобка, но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
        }
    }
}

double SimpleInterpreter::variableValue(const Token & token) const
{
    auto variableName = token.getValue();
    if (variables->contains(variableName)) {
        auto variable = variables->value(variableName);
        if (variable->isInitialized()) {
            return variable->getValue();
        } else {
            throw Error(116, "Переменная '" + variableName + "' не инициализирована", token.getIndexBegin(), token.getIndexEnd());
        }
    } else {
        throw Error(117, "Переменная '" + variableName + "' не определена", token.getIndexBegin(), token.getIndexEnd());
    }
}

double SimpleInterpreter::numberValue(const Token & token) const
{
    bool ok;
    double num = token.getValue().toDouble(&ok);
    if (ok) {
        return num;
    } else {
        throw Error(118, "'" + token.getValue() + "'Невозможно преобразовать в вещественное число.", token.getIndexBegin(), token.getIndexEnd());
    }
}
