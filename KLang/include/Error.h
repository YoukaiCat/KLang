/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef ERROR_H
#define ERROR_H

#include "KLang_global.h"

#include <QException>
#include <QString>

//Исключение, позволяет передать сообщение об ошибке и координаты в исходнике
class KLANG_LIBRARY_EXPORT Error : public QException
{
public:
    explicit Error(int errorCode, const QString & message, int begin, int end);

    void raise() const;
    Error * clone() const;

    int getErrorCode();
    const QString & getMessage() const;
    int getBegin() const;
    int getEnd() const;

private:
    int errorCode;
    QString message;
    int begin;
    int end;
};

#endif // ERROR_H
