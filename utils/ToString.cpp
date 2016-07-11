#include <cstdint>

#include "utils/ToString.h"

std::ostream& operator<<(std::ostream &s, const QLatin1String &string)
{
    s << string;
    return s;
}

std::ostream& operator<<(std::ostream &s, const QByteArray &array)
{
    s << array;
    return s;
}

std::ostream& operator<<(std::ostream &s, const void *ptr)
{
    const std::ios::fmtflags oldFlags = s.flags();
    s.setf(std::ios::hex);

    s << reinterpret_cast<intptr_t>(ptr);

    s.flags(oldFlags);

    return s;
}

std::ostream& operator<<(std::ostream &s, const QPoint &point)
{
    s << "<QPoint (" << point.x() << "," << point.y() << ")";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QPointF &point)
{
    s << "<QPointF (" << point.x() << "," << point.y() << ")";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QRect &rect)
{
    s << "<QRect (" << rect.x() << "," << rect.y() << ") "
        << rect.width() << "x" << rect.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QRectF &rect)
{
    s << "<QRectF (" << rect.x() << "," << rect.y() << ") "
        << rect.width() << "x" << rect.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QSize &size)
{
    s << "<QSize " << size.width() << "x" << size.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QSizeF &size)
{
    s << "<QSizeF " << size.width() << "x" << size.height() << ">";
    return s;
}

std::ostream& operator<<(std::ostream &s, const QJsonValue &value)
{
    switch(value.type())
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

std::ostream& operator<<(std::ostream &s, const QJsonObject &obj)
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

std::ostream& operator<<(std::ostream &s, const QJsonArray &array)
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

std::ostream& operator<<(std::ostream &s, const QString &string)
{
    s << string.toStdString();
    return s;
}

std::ostream& operator<<(std::ostream &s, const QStringList &list)
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

std::ostream& operator<<(std::ostream &s, const QObject * const obj)
{
    if (obj == nullptr)
    {
        s << "nullptr";
        return s;
    }

    const QMetaObject *metaObj = obj->metaObject();
    s << metaObj->className() << "<" << obj->objectName() << ">";

    return s;
}
