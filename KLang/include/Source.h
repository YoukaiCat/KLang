/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef SOURCE_H
#define SOURCE_H

#include "KLang_global.h"

#include <QStringList>

class Source
{
public:
    explicit Source(const QString & source);
    QString shift();
    QString first() const;
    bool isEmpty() const;
    int getIndex() const;

private:
    QStringList chars;
    int index;
};

#endif // SOURCE_H
