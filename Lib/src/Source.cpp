/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Source.h"

Source::Source(const QString & source, QObject * parent)
    : QObject(parent)
{
    chars = source.split("");
    index = -2; //Магическое число, подобраное опытным путём
}

QString Source::shift()
{
    index++;
    return chars.takeFirst();
}

QString Source::first() const
{
    return chars.first();
}

bool Source::isEmpty() const
{
    return chars.isEmpty();
}

int Source::getIndex() const
{
    return index;
}
