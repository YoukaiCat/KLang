/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef ERROR_H
#define ERROR_H

#include <QException>
#include <QString>

//Исключение, позволяет передать сообщение об ошибке и координаты в исходнике
class Error : public QException
{
public:
    explicit Error(const QString & message, int begin, int end);

    void raise() const;
    Error * clone() const;

    const QString & getMessage() const;
    int getBegin() const;
    int getEnd() const;

private:
    QString message;
    int begin;
    int end;
};

#endif // ERROR_H
