/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Parser.h"
#include "Error.h"
#include "Lexeme.h"

Parser::Parser(const QList<Token> & tokens, QObject * parent)
    : QObject(parent)
    , tokens(tokens)
{}

Node Parser::parse()
{
    return begin();
}

Node Parser::begin()
{
    auto token = tokens.takeFirst();
    if (token.getType() == Lexeme::Begin) {
        auto declarationsNode = declarations();
        auto assignmentsNode = assignments();
        auto endNode = end();
        return Node(token).addChild(declarationsNode).addChild(assignmentsNode).addChild(endNode);
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Начало\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

// TODO Запилить switch по типам.
// Чтобы выводилось "но получено (ключевое слово | переменная | символ | ...)"
Node Parser::declarations()
{
    if (tokens.first().getType() == Lexeme::SingleDeclaration || tokens.first().getType() == Lexeme::MultipleDeclaration) {
        QList<Node> declarations;
        declarations.append(processDeclarations());
        while (tokens.first().getType() == Lexeme::Semicolon && (tokens.at(1).getType() == Lexeme::SingleDeclaration || tokens.at(1).getType() == Lexeme::MultipleDeclaration)) {
            tokens.takeFirst();
            declarations.append(processDeclarations());
        }
        return Node(Token(Lexeme::Declarations, "", 0, 0)).addChildren(declarations);
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Анализ\" или слово \"Синтез\", но получено '") + tokens.first().getValue() + "'", tokens.first().getIndexBegin(), tokens.first().getIndexEnd());
    }
}

Node Parser::processDeclarations()
{
    auto token = tokens.takeFirst();
    if (token.getType() == Lexeme::SingleDeclaration) {
        return Node(token).addChild(declareId());
    } else if (token.getType() == Lexeme::MultipleDeclaration) {
        return Node(token).addChildren(declareIds());
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Анализ\" или слово \"Синтез\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

Node Parser::declareId()
{
    auto token = tokens.takeFirst();
    if (token.getType() == Lexeme::Id) {
        return Node(token);
    } else {
        throw Error(QString("После слова \"Анализ\" ожидался идентификатор, но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

QList<Node> Parser::declareIds()
{
    if (tokens.first().getType() == Lexeme::Id) {
        QList<Node> ids;
        ids.append(Node(tokens.takeFirst()));
        while (tokens.first().getType() == Lexeme::Comma && tokens.at(1).getType() == Lexeme::Id) {
            tokens.takeFirst();
            ids.append(Node(tokens.takeFirst()));
        }
        return ids;
    } else {
        throw Error(QString("Ожидался идентификатор после слова \"Синтез\", но получено '") + tokens.first().getValue() + "'", tokens.first().getIndexBegin(), tokens.first().getIndexEnd());
    }
}

Node Parser::assignments()
{
    if (tokens.first().getType() == Lexeme::Id && tokens.at(1).getType() == Lexeme::Equality) {
        QList<Node> as;
        as.append(createAssignment());
        while (tokens.first().getType() == Lexeme::Id && tokens.at(1).getType() == Lexeme::Equality) {
            as.append(createAssignment());
        }
        return Node(Token(Lexeme::Assignments, "", 0, 0)).addChildren(as);
    } else {
        throw Error(QString("После объявления переменных ожидались присваивания, но получено '") + tokens.first().getValue() + "'", tokens.first().getIndexBegin(), tokens.first().getIndexEnd());
    }
}

Node Parser::createAssignment()
{
    auto idToken = tokens.takeFirst();
    auto eqToken = tokens.takeFirst();
    return Node(eqToken).addChild(Node(idToken)).addChild(additiveExpression());
}

// Classic recursive-descent parser for expressions
// Inefficient, boilerplate and hardcoded,
// but close to grammar rules and easy to understand
Node Parser::additiveExpression()
{
    //Выражение более высокого приоретета либо его отрицание
    Node node;
    if (tokens.first().getType() == Lexeme::Minus) {
        //Eсли унарный минус есть, создаётся нода с отрицанием выражения высшего приоретета
        node = unaryMinus();
    } else {
        //Иначе возвращается нода выражения высшего приоретета
        node = multiplicativeExpression();
    }
    while (tokens.first().getType() == Lexeme::Plus || tokens.first().getType() == Lexeme::Minus) {
        auto sign = tokens.takeFirst();
        node = Node(sign).addChild(node).addChild(multiplicativeExpression());
    }
    return node;
}

Node Parser::unaryMinus()
{
    //Унарный минус по BNF должен одинаковый приоретет с + - и ниже * /
    //Однако, может стоять только в левой части выражения, тем самым, всегда в приоретете над + -
    auto token = tokens.takeFirst();
    auto unaryMinus = Token(Lexeme::UnaryMinus, token.getValue(), token.getIndexBegin(), token.getIndexEnd());
    return Node(unaryMinus).addChild(multiplicativeExpression());
}

Node Parser::multiplicativeExpression()
{
    auto node = booleanBinaryExpression();
    while (tokens.first().getType() == Lexeme::Multiply || tokens.first().getType() == Lexeme::Divide) {
        auto sign = tokens.takeFirst();
        node = Node(sign).addChild(node).addChild(booleanBinaryExpression());
    }
    return node;
}

Node Parser::booleanBinaryExpression()
{
    auto node = notExpression();
    while (tokens.first().getType() == Lexeme::And || tokens.first().getType() == Lexeme::Or) {
        auto sign = tokens.takeFirst();
        node = Node(sign).addChild(node).addChild(notExpression());
    }
    return node;
}

Node Parser::notExpression()
{
    if (tokens.first().getType() == Lexeme::Not) {
        auto sign = tokens.takeFirst();
        return Node(sign).addChild(base());
    } else {
        return base();
    }
}

Node Parser::base()
{
    auto token = tokens.takeFirst();
    if (token.getType() == Lexeme::Num || token.getType() == Lexeme::Id) {
        return Node(token);
    } else if (token.getType() == Lexeme::LeftParentheses) {
        paranthesisIndices.push(token.getIndexBegin());
        auto node = additiveExpression();
        if (tokens.first().getType() == Lexeme::RightParentheses) {
            tokens.takeFirst();
            paranthesisIndices.pop();
            return node;
        } else {
            throw Error(QString("Не найдена закрывающая скобка."), paranthesisIndices.pop(), tokens.first().getIndexEnd());
        }
    } else {
        throw Error(QString("Ожидалось число, идентификатор или открывающая скобка, но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}

Node Parser::end()
{
    auto token = tokens.takeFirst();
    if (token.getType() == Lexeme::End) {
        if (tokens.first().getType() == Lexeme::Eof) {
            return Node(token);
        } else {
            throw Error(QString("После ключевого слова \"Окончание\" не должны находиться ключевые слова, идентификаторы и знаки операций. Получено '") + tokens.first().getValue() + "'", tokens.first().getIndexBegin(), tokens.first().getIndexEnd());
        }
    } else {
        throw Error(QString("Ожидалось ключевое слово \"Окончание\", но получено '") + token.getValue() + "'", token.getIndexBegin(), token.getIndexEnd());
    }
}
