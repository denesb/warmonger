#include "log/LogStream.h"

using namespace warmonger::log;

LogStream::LogStream(LogLevel lvl, const QString &name, const QString &file, const QString &func, int line) :
    textStream(),
    buffer(),
    logger(Logger::get(name)),
    record(lvl, name, file, func, line)
{
    this->textStream.setString(&this->buffer, QIODevice::WriteOnly);
}

LogStream::~LogStream()
{
    this->record.setMsg(this->buffer);
    this->logger->logRecord(this->record);
}

LogStream& LogStream::operator<<(QChar c)
{
    this->textStream << c;
    return *this;
}

LogStream& LogStream::operator<<(signed short i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(float f)
{
    this->textStream << f;
    return *this;
}

LogStream& LogStream::operator<<(const QString & string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(char c)
{
    this->textStream << c;
    return *this;
}

LogStream& LogStream::operator<<(unsigned short i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(signed int i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(unsigned int i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(signed long i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(unsigned long i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(qlonglong i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(qulonglong i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(double f)
{
    this->textStream << f;
    return *this;
}

LogStream& LogStream::operator<<(QLatin1String string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(const QByteArray & array)
{
    this->textStream << array;
    return *this;
}

LogStream& LogStream::operator<<(const char * string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(const void * ptr)
{
    this->textStream << ptr;
    return *this;
}

LogStream& LogStream::operator<<(bool b)
{
    static const QString trueStr("true");
    static const QString falseStr("false");
    this->textStream << (b ? trueStr : falseStr);

    return *this;
}

LogStream& LogStream::operator<<(const QPoint &point)
{
    this->textStream << "<QPoint (" << point.x() << "," << point.y() << ")";
    return *this;
}

LogStream& LogStream::operator<<(const QPointF &point)
{
    this->textStream << "<QPointF (" << point.x() << "," << point.y() << ")";
    return *this;
}

LogStream& LogStream::operator<<(const QRect &rect)
{
    this->textStream << "<QRect (" << rect.x() << "," << rect.y() << ") "
        << rect.width() << "x" << rect.height() << ">";
    return *this;
}

LogStream& LogStream::operator<<(const QRectF &rect)
{
    this->textStream << "<QRectF (" << rect.x() << "," << rect.y() << ") "
        << rect.width() << "x" << rect.height() << ">";
    return *this;
}

LogStream& LogStream::operator<<(const QSize &size)
{
    this->textStream << "<QSize " << size.width() << "x"
        << size.height() << ">";
    return *this;
}

LogStream& LogStream::operator<<(const QSizeF &size)
{
    this->textStream << "<QSizeF " << size.width() << "x"
        << size.height() << ">";
    return *this;
}

LogStream& LogStream::operator<<(const QJsonValue &value)
{
    switch(value.type())
    {
        case QJsonValue::Object:
            *this << value.toObject();
            break;
        case QJsonValue::Array:
            *this << value.toArray();
            break;
        case QJsonValue::String:
            this->textStream << "\"" << value.toString() << "\"";
            break;
        case QJsonValue::Double:
            this->textStream << value.toDouble();
            break;
        case QJsonValue::Bool:
            *this << value.toBool();
            break;
        case QJsonValue::Null:
        default:
            break;
    }

    return *this;
}

LogStream& LogStream::operator<<(const QJsonObject &obj)
{
    this->textStream << "{";

    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        this->textStream << "\"" << it.key() << "\":";
        *this << it.value();

        QJsonObject::ConstIterator next = it + 1;
        if (next != obj.constEnd())
        {
            this->textStream << ",";
        }
    }

    this->textStream << "}";

    return *this;
}

LogStream& LogStream::operator<<(const QJsonArray &array)
{
    this->textStream << "[";
    for (int i = 0; i < array.size(); i++)
    {
        *this << array[i];

        if (i + 1 < array.size())
        {
            this->textStream << ",";
        }
    }
    this->textStream << "]";

    return *this;
}

LogStream& LogStream::operator<<(const QStringList &list)
{
    this->textStream << "[";
    for (int i = 0; i < list.size(); i++)
    {
        *this << list[i];

        if (i + 1 < list.size())
        {
            this->textStream << ",";
        }
    }
    this->textStream << "]";

    return *this;
}


LogStream& LogStream::operator<<(const QObject * const obj)
{
    if (obj == nullptr)
    {
        this->textStream << "nullptr";
        return *this;
    }

    const QMetaObject *metaObj = obj->metaObject();
    this->textStream << metaObj->className() << "<" << obj->objectName() << ">";

    return *this;
}
