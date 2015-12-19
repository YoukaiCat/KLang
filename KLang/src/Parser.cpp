/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Parser.h"
#include "Error.h"

using std::make_shared;

Parser::Parser(const shared_ptr<QList<Token>> tokens, QObject * parent)
    : QObject(parent)
    , tokens(tokens)
{}

shared_ptr<Node> Parser::parse()
{
    return begin();
}

shared_ptr<Node> Parser::begin()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Begin) {
        auto declarationsNode = declarations();
        auto assignmentsNode = assignments();
        auto endNode = end();
        auto node = make_shared<Node>(Node(token));
        node->addChild(declarationsNode)->addChild(assignmentsNode)->addChild(endNode);
        return node;
    } else {
        throw Error(100, QString("Ожидалось ключевое слово 'Начало', но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<Node> Parser::declarations()
{
    auto node = make_shared<Node>(Node(Token(Lexeme::Declarations, "", 0, 0)));
    auto declarations = QList<shared_ptr<Node>>();
    do {
        if (!declarations.isEmpty()) tokens->takeFirst();
        declarations.append(processDeclarations());
    } while (tokens->first().getType() == Lexeme::Semicolon);
    node->addChildren(declarations);
    return node;
}

shared_ptr<Node> Parser::processDeclarations()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::SingleDeclaration) {
        auto node = make_shared<Node>(Node(token));
        node->addChild(declareId());
        lastDeclaredType = Lexeme::SingleDeclaration;
        return node;
    } else if (token.getType() == Lexeme::MultipleDeclaration) {
        auto node = make_shared<Node>(Node(token));
        node->addChildren(*declareIds());
        lastDeclaredType = Lexeme::MultipleDeclaration;
        return node;
    } else {
        throw Error(102, QString("Ожидалось ключевое слово 'Анализ' или слово 'Синтез', но ")  + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<Node> Parser::declareId()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Id) {
        return make_shared<Node>(Node(token));
    } else {
        throw Error(103, QString("После слова 'Анализ' ожидалась переменная, но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<QList<shared_ptr<Node>>> Parser::declareIds()
{
    auto ids = make_shared<QList<shared_ptr<Node>>>(QList<shared_ptr<Node>>());
    do {
        if (!ids->isEmpty()) {
            tokens->takeFirst();
            if (tokens->first().getType() == Lexeme::Id) {
                ids->append(make_shared<Node>(Node(tokens->takeFirst())));
            } else {
                throw Error(104, QString("После запятой ожидалась переменная, но ") + tokens->first().toString(), tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
            }
        } else {
            if (tokens->first().getType() == Lexeme::Id) {
                ids->append(make_shared<Node>(Node(tokens->takeFirst())));
            } else {
                throw Error(104, QString("После слова 'Синтез' ожидалась переменная, но ") + tokens->first().toString(), tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
            }
        }
    } while (tokens->first().getType() == Lexeme::Comma);
    return ids;
}

shared_ptr<Node> Parser::assignments()
{
    auto node = make_shared<Node>(Node(Token(Lexeme::Assignments, "", 0, 0)));
    auto as = QList<shared_ptr<Node>>();
    do {
        if (tokens->first().getType() == Lexeme::Id) {
            if (tokens->at(1).getType() == Lexeme::Equality) {
                as.append(createAssignment());
            } else {
                throw Error(105, QString("После переменной ожидался символ '=', но ") + tokens->at(1).toString(), tokens->at(1).getIndexBegin(), tokens->at(1).getIndexEnd());
            }
        } else {
            if (lastDeclaredType == Lexeme::MultipleDeclaration) {
                throw Error(105, QString("Ожидался символ ';' или символ ',' или переменная, но ") + tokens->first().toString(), tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
            } else {
                throw Error(105, QString("Ожидался символ ';' или переменная, но ") + tokens->first().toString(), tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
            }
        }
    } while (tokens->first().getType() == Lexeme::Id);
    node->addChildren(as);
    return node;
}

shared_ptr<Node> Parser::createAssignment()
{
    auto idToken = tokens->takeFirst();
    auto eqToken = tokens->takeFirst();
    auto node = make_shared<Node>(Node(eqToken));
    node->addChild(make_shared<Node>(Node(idToken)))->addChild(additiveExpression());
    return node;
}

// Classic recursive-descent parser for expressions
// Inefficient, boilerplate and hardcoded,
// but close to grammar rules and easy to understand
shared_ptr<Node> Parser::additiveExpression()
{
    //Выражение более высокого приоретета либо его отрицание
    shared_ptr<Node> node;
    if (tokens->first().getType() == Lexeme::Minus) {
        //Eсли унарный минус есть, создаётся нода с отрицанием выражения высшего приоретета
        node = unaryMinus();
    } else {
        //Иначе возвращается нода выражения высшего приоретета
        node = multiplicativeExpression();
    }
    while (tokens->first().getType() == Lexeme::Plus || tokens->first().getType() == Lexeme::Minus) {
        auto sign = tokens->takeFirst();
        auto signNode = make_shared<Node>(Node(sign));
        signNode->addChild(node)->addChild(multiplicativeExpression());
        node = signNode;
    }
    return node;
}

shared_ptr<Node> Parser::unaryMinus()
{
    //Унарный минус по BNF должен одинаковый приоретет с + - и ниже * /
    //Однако, может стоять только в левой части выражения, тем самым, всегда в приоретете над + -
    auto token = tokens->takeFirst();
    auto unaryMinus = Token(Lexeme::UnaryMinus, token.getValue(), token.getIndexBegin(), token.getIndexEnd());
    auto node = make_shared<Node>(Node(unaryMinus));
    node->addChild(multiplicativeExpression());
    return node;
}

shared_ptr<Node> Parser::multiplicativeExpression()
{
    auto node = booleanBinaryExpression();
    while (tokens->first().getType() == Lexeme::Multiply || tokens->first().getType() == Lexeme::Divide) {
        auto sign = tokens->takeFirst();
        auto signNode = make_shared<Node>(Node(sign));
        signNode->addChild(node)->addChild(booleanBinaryExpression());
        node = signNode;
    }
    return node;
}

shared_ptr<Node> Parser::booleanBinaryExpression()
{
    auto node = notExpression();
    while (tokens->first().getType() == Lexeme::And || tokens->first().getType() == Lexeme::Or) {
        auto sign = tokens->takeFirst();
        auto signNode = make_shared<Node>(Node(sign));
        signNode->addChild(node)->addChild(notExpression());
        node = signNode;
    }
    return node;
}

shared_ptr<Node> Parser::notExpression()
{
    if (tokens->first().getType() == Lexeme::Not) {
        auto sign = tokens->takeFirst();
        auto signNode = make_shared<Node>(Node(sign));
        signNode->addChild(base());
        return signNode;
    } else {
        return base();
    }
}

// TODO
// а = ((5.0) 5.0)
// 106: Не найдена закрывающая скобка
shared_ptr<Node> Parser::base()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Num || token.getType() == Lexeme::Id) {
        return make_shared<Node>(Node(token));
    } else if (token.getType() == Lexeme::LeftParentheses) {
        parenthesisIndices.push(token.getIndexBegin());
        auto node = additiveExpression();
        if (tokens->first().getType() == Lexeme::RightParentheses) {
            tokens->takeFirst();
            parenthesisIndices.pop();
            return node;
        } else {
            throw Error(106, QString("Не найдена закрывающая скобка"), parenthesisIndices.pop(), token.getIndexEnd());
        }
    } else {
        throw Error(107, QString("Ожидалось число, переменная или открывающая скобка, но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<Node> Parser::end()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::End) {
        if (tokens->first().getType() == Lexeme::Eof) {
            return make_shared<Node>(Node(token));
        } else {
            throw Error(108, QString("После ключевого слова 'Окончание' не должны находиться ключевые слова, переменные и знаки операций.") + tokens->first().toString(), tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
        }
    } else {
        throw Error(109, QString("Ожидалась переменная, знак операции или ключевое слово 'Окончание', но ") + token.toString(), token.getIndexBegin(), token.getIndexEnd());
    }
}
