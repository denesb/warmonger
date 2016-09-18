#ifndef W_UTILS_TO_STRING_H
#define W_UTILS_TO_STRING_H

#include <ostream>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QString>
#include <QUrl>

std::ostream& operator<<(std::ostream& s, const QLatin1String& string);
std::ostream& operator<<(std::ostream& s, const QByteArray& array);
std::ostream& operator<<(std::ostream& s, const void* ptr);
std::ostream& operator<<(std::ostream& s, const QPoint& point);
std::ostream& operator<<(std::ostream& s, const QPointF& point);
std::ostream& operator<<(std::ostream& s, const QRect& rect);
std::ostream& operator<<(std::ostream& s, const QRectF& rect);
std::ostream& operator<<(std::ostream& s, const QSize& size);
std::ostream& operator<<(std::ostream& s, const QSizeF& size);
std::ostream& operator<<(std::ostream& s, const QJsonValue& value);
std::ostream& operator<<(std::ostream& s, const QJsonObject& obj);
std::ostream& operator<<(std::ostream& s, const QJsonArray& array);
std::ostream& operator<<(std::ostream& s, const QString& string);
std::ostream& operator<<(std::ostream& s, const QStringList& list);
std::ostream& operator<<(std::ostream& s, const QObject* const obj);
std::ostream& operator<<(std::ostream& s, const QUrl& url);

#endif // W_UTILS_TO_STRING_H
