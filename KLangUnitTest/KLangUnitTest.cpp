#include <QtTest/QtTest>
#include <QDebug>

#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

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
        QCOMPARE(result["а"], 3.0);
        QCOMPARE(result["б"], 0.0);
        QCOMPARE(result["б1"], 0.0);
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
        QCOMPARE(result["а"], 3.0);
        QCOMPARE(result["б"], 12.0);
        QCOMPARE(result["б1"], 0.0);
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
        QCOMPARE(result["а112"], 0.0);
        QCOMPARE(result["б911"], 0.0);
        QCOMPARE(result["в"], 1.0);
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
        QCOMPARE(result["а"], 0.0);
        QCOMPARE(result["б"], 0.0);
        QCOMPARE(result["в"], 0.0);
        QCOMPARE(result["а1"], 2.0);
        QCOMPARE(result["б1"], 0.0);
        QCOMPARE(result["в1"], 0.0);
    }
    void example5() {
        auto source = R"doc(
            Начало
            Синтез П1
            П1 = П1
            Окончание
        )doc";
        auto result = interpret(source);
        QCOMPARE(result["П1"], 0.0);
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
        QCOMPARE(result["а"], 10.0);
        QCOMPARE(result["б"], 4.0);
        QCOMPARE(result["в"], 13.0);
        QCOMPARE(result["а1"], 6.0);
        QCOMPARE(result["б1"], 6.0);
        QCOMPARE(result["в1"], 5.5);
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
        QCOMPARE(result["а"], 8.0);
        QCOMPARE(result["б"], 8.0);
        QCOMPARE(result["в"], 15.0);
    }
// Precedence works
// - 5.0 - (- 1.0 - 1.0) + (- 2.0) => - ((5.0 - (- (1.0 - 1.0))) + (- 2.0))
// - 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0 + (- 1.0))))))) =>
// - (1.0 + (- (1.0 + (- (1.0 + (- (1.0 + (- (1.0 + (- (1.0 + (- (1.0 + (- 1.0))))))))))))))
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
        QCOMPARE(result["а"], -3.0);
        QCOMPARE(result["б"], 10.0);
        QCOMPARE(result["в"], 0.0);
        QCOMPARE(result["г"], 11.0);
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
        QCOMPARE(result["а"], 0.0);
        QCOMPARE(result["б"], 0.0);
        QCOMPARE(result["в"], 0.0);
    }
private:
    QMap<QString, double> interpret(const QString & source) {
        auto lexer = new Lexer(source);
        auto parser = new Parser(lexer->tokenize());
        auto interpreter = new Interpreter(parser->parse());
        return interpreter->evaluate();
    }
};

QTEST_MAIN(KLangUnitTest)
#include "KLangUnitTest.moc"
