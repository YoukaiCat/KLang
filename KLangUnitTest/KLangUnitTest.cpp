#include <memory>

#include <QtTest/QtTest>

#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Variable.h"

class KLangUnitTest: public QObject
{
    Q_OBJECT
private slots:
    void example1() {
        auto source = R"doc(
            Начало
            Синтез а, б, б1
            а = 2.0 * 1.0 + 1.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 3.0);
        QCOMPARE(result->value("б")->getValue(), 0.0);
        QCOMPARE(result->value("б1")->getValue(), 0.0);
    }
    void example2() {
        auto source = R"doc(
            Начало
            Синтез а, б, б1
            а = 1.0 + 1.0 + 1.0
            б = а * 5.0
            б = б - 3.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 3.0);
        QCOMPARE(result->value("б")->getValue(), 12.0);
        QCOMPARE(result->value("б1")->getValue(), 0.0);
    }
    void example3() {
        auto source = R"doc(
            Начало
            Анализ а112;
            Анализ б911;
            Анализ в
            в = 0.0 / 1.0 + 1.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а112")->getValue(), 0.0);
        QCOMPARE(result->value("б911")->getValue(), 0.0);
        QCOMPARE(result->value("в")->getValue(), 1.0);
    }
    void example4() {
        auto source = R"doc(
            Начало
            Синтез а, б, в;
            Синтез а1, б1, в1
            а1 = 0.0 ИЛИ 5.0 + 1.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 0.0);
        QCOMPARE(result->value("б")->getValue(), 0.0);
        QCOMPARE(result->value("в")->getValue(), 0.0);
        QCOMPARE(result->value("а1")->getValue(), 2.0);
        QCOMPARE(result->value("б1")->getValue(), 0.0);
        QCOMPARE(result->value("в1")->getValue(), 0.0);
    }
    void example6() {
        auto source = R"doc(
            Начало
            Синтез а, б, в;
            Анализ а1;
            Анализ б1;
            Синтез в1
            а = 2.0 + 2.0 + 2.0 + 2.0 + 2.0
            б = 2.0 * 1.0 + 1.0 * 2.0
            в = 3.0 + 2.0 * 4.0 + 2.0
            а1 = 2.0 + 6.0 / 3.0 * 2.0
            б1 = 6.0 / 3.0 * 2.0 + 2.0
            в1 = 3.0 + 2.0 / 4.0 + 2.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 10.0);
        QCOMPARE(result->value("б")->getValue(), 4.0);
        QCOMPARE(result->value("в")->getValue(), 13.0);
        QCOMPARE(result->value("а1")->getValue(), 6.0);
        QCOMPARE(result->value("б1")->getValue(), 6.0);
        QCOMPARE(result->value("в1")->getValue(), 5.5);
    }
    void example7() {
        auto source = R"doc(
            Начало
            Синтез а, б, в
            а = (6.0 + 6.0) / 3.0 * 2.0
            б = 2.0 * (1.0 + 1.0) * 2.0
            в = 3.0 + 2.0 * (4.0 + 2.0)
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 8.0);
        QCOMPARE(result->value("б")->getValue(), 8.0);
        QCOMPARE(result->value("в")->getValue(), 15.0);
    }
    void example8() {
        auto source = R"doc(
            Начало
            Синтез а, б, в, г
            а = - 5.0 - (- 1.0 - 1.0) + (- 2.0)
            б = 5.0 - (- 5.0)
            в = - 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0)))))))
            г = (((((((((1.0 + 1.0) + 1.0) + 1.0) + 1.0) + 1.0) + 1.0) + 1.0 + 1.0) + 1.0) + 1.0)
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), -5.0);
        QCOMPARE(result->value("б")->getValue(), 10.0);
        QCOMPARE(result->value("в")->getValue(), -8.0);
        QCOMPARE(result->value("г")->getValue(), 11.0);
    }
    void example9() {
        auto source = R"doc(
            Начало
            Синтез а, б, в
            а = НЕ 1.0 И НЕ 2.0 И НЕ 3.0 И НЕ 4.0 И НЕ 5.0
            б = НЕ 1.0 И НЕ (1.0 И НЕ 1.0) И НЕ 1.0 И НЕ 1.0
            в = НЕ 1.0 И НЕ (- 1.0 И НЕ 1.0) И НЕ 1.0 И НЕ 1.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 0.0);
        QCOMPARE(result->value("б")->getValue(), 0.0);
        QCOMPARE(result->value("в")->getValue(), 0.0);
    }
    // AND
    void example10() {
        auto source = R"doc(
            Начало
            Синтез а, б, в, г
            а = 1.0 И 1.0
            б = 1.0 И 0.0
            в = 0.0 И 1.0
            г = 0.0 И 0.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 1.0);
        QCOMPARE(result->value("б")->getValue(), 0.0);
        QCOMPARE(result->value("в")->getValue(), 0.0);
        QCOMPARE(result->value("г")->getValue(), 0.0);
    }
    // OR
    void example11() {
        auto source = R"doc(
            Начало
            Синтез а, б, в, г
            а = 1.0 ИЛИ 1.0
            б = 1.0 ИЛИ 0.0
            в = 0.0 ИЛИ 1.0
            г = 0.0 ИЛИ 0.0
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result->value("а")->getValue(), 1.0);
        QCOMPARE(result->value("б")->getValue(), 1.0);
        QCOMPARE(result->value("в")->getValue(), 1.0);
        QCOMPARE(result->value("г")->getValue(), 0.0);
    }
private:
    shared_ptr<QMap<QString, shared_ptr<Variable>>> interpret(const QString & source) {
        auto lexer = new Lexer(source);
        auto parser = new Parser(lexer->tokenize());
        auto interpreter = new Interpreter(parser->parse());
        return interpreter->evaluate();
    }
};

QTEST_MAIN(KLangUnitTest)
#include "KLangUnitTest.moc"
