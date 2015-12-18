#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>

class Variable
{
public:
    Variable(const QString & name);

    QString getName() const;
    void setName(const QString & name);

    double getValue() const;
    void setValue(long double value);

    bool isInitialized() const;

    long int getIndex();

    QString toString();

private:
    QString name;
    double value;
    bool initialized;
    long int index;
};

#endif // VARIABLE_H
