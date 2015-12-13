/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Parser.h"
#include "Error.h"
#include "Lexeme.h"

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
        throw Error(QString("Ожидалось ключевое слово \"Начало\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

// TODO Запилить switch по типам.
// Чтобы выводилось "но получено (ключевое слово | переменная | символ | ...)"
shared_ptr<Node> Parser::declarations()
{
    if (tokens->first().getType() == Lexeme::SingleDeclaration || tokens->first().getType() == Lexeme::MultipleDeclaration) {
        auto declarations = make_shared<QList<shared_ptr<Node>>>(QList<shared_ptr<Node>>());
        declarations->append(processDeclarations());
        while (tokens->first().getType() == Lexeme::Semicolon && (tokens->at(1).getType() == Lexeme::SingleDeclaration || tokens->at(1).getType() == Lexeme::MultipleDeclaration)) {
            tokens->takeFirst();
            declarations->append(processDeclarations());
        }
        auto node = make_shared<Node>(Node(Token(Lexeme::Declarations, "", 0, 0)));
        node->addChildren(declarations);
        return node;
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Анализ\" или слово \"Синтез\", но получено '") + tokens->first().getValue() + "'", tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
    }
}

shared_ptr<Node> Parser::processDeclarations()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::SingleDeclaration) {
        auto node = make_shared<Node>(Node(token));
        node->addChild(declareId());
        return node;
    } else if (token.getType() == Lexeme::MultipleDeclaration) {
        auto node = make_shared<Node>(Node(token));
        node->addChildren(declareIds());
        return node;
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Анализ\" или слово \"Синтез\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<Node> Parser::declareId()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Id) {
        return make_shared<Node>(Node(token));
    } else {
        throw Error(QString("После слова \"Анализ\" ожидался идентификатор, но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<QList<shared_ptr<Node>>> Parser::declareIds()
{
    if (tokens->first().getType() == Lexeme::Id) {
        auto ids = make_shared<QList<shared_ptr<Node>>>(QList<shared_ptr<Node>>());
        ids->append(make_shared<Node>(Node(tokens->takeFirst())));
        while (tokens->first().getType() == Lexeme::Comma && tokens->at(1).getType() == Lexeme::Id) {
            tokens->takeFirst();
            ids->append(make_shared<Node>(Node(tokens->takeFirst())));
        }
        return ids;
    } else {
        throw Error(QString("Ожидался идентификатор после слова \"Синтез\", но получено '") + tokens->first().getValue() + "'", tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
    }
}

shared_ptr<Node> Parser::assignments()
{
    if (tokens->first().getType() == Lexeme::Id && tokens->at(1).getType() == Lexeme::Equality) {
        auto as = make_shared<QList<shared_ptr<Node>>>(QList<shared_ptr<Node>>());
        as->append(createAssignment());
        while (tokens->first().getType() == Lexeme::Id && tokens->at(1).getType() == Lexeme::Equality) {
            as->append(createAssignment());
        }
        auto node = make_shared<Node>(Node(Token(Lexeme::Assignments, "", 0, 0)));
        node->addChildren(as);
        return node;
    } else {
        throw Error(QString("После объявления переменных ожидались присваивания, но получено '") + tokens->first().getValue() + "'", tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
    }
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

shared_ptr<Node> Parser::base()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::Num || token.getType() == Lexeme::Id) {
        return make_shared<Node>(Node(token));
    } else if (token.getType() == Lexeme::LeftParentheses) {
        paranthesisIndices.push(token.getIndexBegin());
        auto node = additiveExpression();
        if (tokens->first().getType() == Lexeme::RightParentheses) {
            tokens->takeFirst();
            paranthesisIndices.pop();
            return node;
        } else {
            throw Error(QString("Не найдена закрывающая скобка."), paranthesisIndices.pop(), tokens->first().getIndexEnd());
        }
    } else {
        throw Error(QString("Ожидалось число, идентификатор или открывающая скобка, но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

shared_ptr<Node> Parser::end()
{
    auto token = tokens->takeFirst();
    if (token.getType() == Lexeme::End) {
        if (tokens->first().getType() == Lexeme::Eof) {
            return make_shared<Node>(Node(token));
        } else {
            throw Error(QString("После ключевого слова \"Окончание\" не должны находиться ключевые слова, идентификаторы и знаки операций. Получено '") + tokens->first().getValue() + "'", tokens->first().getIndexBegin(), tokens->first().getIndexEnd());
        }
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Окончание\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}
