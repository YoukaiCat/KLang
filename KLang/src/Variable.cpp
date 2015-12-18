#include "include/Variable.h"

Variable::Variable(const QString & name)
    : name(name)
    , value(0.0)
    , initialized(false)
{
    static long int variableCount = 0;
    variableCount++;
    index = variableCount;
}

QString Variable::getName() const
{
    return name;
}

void Variable::setName(const QString & name)
{
    this->name = name;
}

double Variable::getValue() const
{
    return value;
}

void Variable::setValue(long double value)
{
    this->value = value;
    initialized = true;
}

bool Variable::isInitialized() const
{
    return initialized;
}

long int Variable::getIndex()
{
    return index;
}

QString Variable::toString()
{
    //Если число круглое, то #number не будет выводить точку и хотябы один ноль.
    //Поэтому проверяем остаток от деления, и если число круглое, выводим строго один ноль после запятой.
    //В ином случае вывод адаптивный: выводятся только значащие цифры после запятой.
    if (floor(value) == value) {
         return QString::number(value, 'f', 1);
    } else {
         return QString::number(value);
    }
}
