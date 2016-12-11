/** \file
 * To-string functions.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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
std::ostream& operator<<(std::ostream& s, const QObject& obj);
std::ostream& operator<<(std::ostream& s, const QUrl& url);

#endif // W_UTILS_TO_STRING_H
