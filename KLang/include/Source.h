/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef SOURCE_H
#define SOURCE_H

#include "KLang_global.h"

#include <QObject>
#include <QStringList>

class Source : public QObject
{
    Q_OBJECT
public:
    explicit Source(const QString & source, QObject * parent = 0);
    QString shift();
    QString first() const;
    bool isEmpty() const;
    int getIndex() const;

private:
    QStringList chars;
    int index;
};

#endif // SOURCE_H
