/**
 * Copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cstdint>

#include "utils/ToString.h"

std::ostream& operator<<(std::ostream& s, const QLatin1String& string)
{
    s << string;
    return s;
}

std::ostream& operator<<(std::ostream& s, const QByteArray& array)
{
    s << array;
    return s;
}

std::ostream& operator<<(std::ostream& s, const void* ptr)
{
    const std::ios::fmtflags oldFlags = s.flags();
    s.setf(std::ios::hex);

    s << reinterpret_cast<intptr_t>(ptr);

    s.flags(oldFlags);

    return s;
}

std::ostream& operator<<(std::ostream& s, const QPoint& point)
{
    s << "<QPoint (" << point.x() << "," << point.y() << ")>";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QPointF& point)
{
    s << "<QPointF (" << point.x() << "," << point.y() << ")>";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QRect& rect)
{
    s << "<QRect (" << rect.x() << "," << rect.y() << ") " << rect.width() << "x" << rect.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QRectF& rect)
{
    s << "<QRectF (" << rect.x() << "," << rect.y() << ") " << rect.width() << "x" << rect.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QSize& size)
{
    s << "<QSize " << size.width() << "x" << size.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QSizeF& size)
{
    s << "<QSizeF " << size.width() << "x" << size.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream& s, const QJsonValue& value)
{
    switch (value.type())
    {
        case QJsonValue::Object:
            s << value.toObject();
            break;
        case QJsonValue::Array:
            s << value.toArray();
            break;
        case QJsonValue::String:
            s << "\"" << value.toString() << "\"";
            break;
        case QJsonValue::Double:
            s << value.toDouble();
            break;
        case QJsonValue::Bool:
            s << value.toBool();
            break;
        case QJsonValue::Null:
        default:
            break;
    }

    return s;
}

std::ostream& operator<<(std::ostream& s, const QJsonObject& obj)
{
    s << "{";

    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        s << "\"" << it.key() << "\":";
        s << it.value();

        QJsonObject::ConstIterator next = it + 1;
        if (next != obj.constEnd())
        {
            s << ",";
        }
    }

    s << "}";

    return s;
}

std::ostream& operator<<(std::ostream& s, const QJsonArray& array)
{
    s << "[";
    for (int i = 0; i < array.size(); i++)
    {
        s << array[i];

        if (i + 1 < array.size())
        {
            s << ",";
        }
    }
    s << "]";

    return s;
}

std::ostream& operator<<(std::ostream& s, const QString& string)
{
    s << string.toStdString();
    return s;
}

std::ostream& operator<<(std::ostream& s, const QStringList& list)
{
    s << "[";
    for (int i = 0; i < list.size(); i++)
    {
        s << list[i];

        if (i + 1 < list.size())
        {
            s << ",";
        }
    }
    s << "]";

    return s;
}

std::ostream& operator<<(std::ostream& s, const QObject& obj)
{
    s << &obj;
    return s;
}

std::ostream& operator<<(std::ostream& s, const QObject* const obj)
{
    if (obj == nullptr)
    {
        s << "nullptr";
        return s;
    }

    const QMetaObject* metaObj = obj->metaObject();
    s << metaObj->className() << "<" << obj->objectName() << ">";

    return s;
}

std::ostream& operator<<(std::ostream& s, const QUrl& url)
{
    s << url.path();

    return s;
}
