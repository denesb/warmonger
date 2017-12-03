/**
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_IO_JSON_SERIALIZER_HPP
#define W_IO_JSON_SERIALIZER_HPP

#include <utility>
#include <vector>
#include <map>

#include <QJsonObject>
#include <QJsonArray>

#include "io/Visitor.hpp"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

QJsonValue serializeValueToJson(int value, const QObject&);
QJsonValue serializeValueToJson(const QString& value, const QObject&);
QJsonValue serializeValueToJson(const QColor& value, const QObject&);

template <typename T>
QJsonValue serializeValueToJson(const std::vector<T>& value, const QObject& obj);

template <typename K, typename T>
QJsonValue serializeValueToJson(const std::map<K, T>& value, const QObject& obj);

template <typename T>
typename std::enable_if<std::is_base_of<QObject, T>::value && !std::is_base_of<core::WObject, T>::value, QJsonValue>::type
serializeValueToJson(T* value, const QObject& obj);

template <typename T>
typename std::enable_if<std::is_base_of<core::WObject, T>::value, QJsonValue>::type
serializeValueToJson(T* value, const QObject& obj);

template <typename T>
typename std::enable_if<std::is_enum<T>::value, QJsonValue>::type
serializeValueToJson(T value, const QObject& obj);

template<typename T, typename Member>
void serializeMemberToJsonImpl(const T& obj, QJsonObject& jobj, const Member& member)
{
    jobj[member.name] = serializeValueToJson(member.getter(obj), obj);
}

template<typename T, typename LastMember>
void serializeMembersToJsonImpl(const T& obj, QJsonObject& jobj, const LastMember& lastMember)
{
    serializeMemberToJsonImpl(obj, jobj, lastMember);
}

template<typename T, typename FirstMember, typename ...Members>
void serializeMembersToJsonImpl(const T& obj, QJsonObject& jobj, const FirstMember& firstMember, const Members&... members)
{
    serializeMemberToJsonImpl(obj, jobj, firstMember);
    serializeMembersToJsonImpl(obj, jobj, members...);
}

template<typename T, typename Members, std::size_t... I>
void serializeMembersToJsonImpl(const T& obj, QJsonObject& jobj, const Members& members, std::index_sequence<I...>)
{
    serializeMembersToJsonImpl(obj, jobj, std::get<I>(members)...);
}

template<typename T, typename Members>
void serializeMembersToJson(const T& obj, QJsonObject& jobj, const Members& members)
{
    return serializeMembersToJsonImpl(obj, jobj, members, std::make_index_sequence<std::tuple_size<Members>::value>());
}

template <typename T>
QJsonObject serializeToJson(const T& obj);

template <>
QJsonObject serializeToJson<QObject>(const QObject&)
{
    return QJsonObject();
}

template <typename T>
QJsonObject serializeToJson(const T& obj)
{
    auto description = T::describe(Visitor<T>());
    using Description = decltype(description);

    QJsonObject jobj = serializeToJson<typename Description::ParentClass>(obj);
    serializeMembersToJson(obj, jobj, description.getMembers());
    return jobj;
}

// Implementations

QJsonValue serializeValueToJson(int value, const QObject&)
{
    return value;
}

QJsonValue serializeValueToJson(const QString& value, const QObject&)
{
    return value;
}

QJsonValue serializeValueToJson(const QColor& value, const QObject&)
{
    return value.name();
}

template <typename T>
QJsonValue serializeValueToJson(const std::vector<T>& value, const QObject& obj)
{
    QJsonArray jarr;

    for (const auto& element : value)
    {
        jarr.append(serializeValueToJson(element, obj));
    }

    return jarr;
}

template <typename K, typename T>
QJsonValue serializeValueToJson(const std::map<K, T>& value, const QObject& obj)
{
    QJsonObject jobj;

    for (const auto& element : value)
    {
        auto jkey = serializeValueToJson(element.first, obj);
        jobj[jkey.toString()] = serializeValueToJson(element.second, obj);
    }

    return jobj;
}

template <typename T>
typename std::enable_if<std::is_base_of<QObject, T>::value && !std::is_base_of<core::WObject, T>::value, QJsonValue>::type
serializeValueToJson(T* value, const QObject&)
{
    return serializeToJson(*value);
}

template <typename T>
typename std::enable_if<std::is_base_of<core::WObject, T>::value, QJsonValue>::type
serializeValueToJson(T* value, const QObject& obj)
{
    if (value->parent() == &obj)
        return serializeToJson(*value);
    else
        return serializeReference(value);
}

template <typename T>
typename std::enable_if<std::is_enum<T>::value, QJsonValue>::type
serializeValueToJson(T value, const QObject&)
{
    const QMetaEnum metaEnum{QMetaEnum::fromType<T>()};
    auto str = metaEnum.valueToKey(static_cast<int>(value));
    if (!str)
        throw utils::ValueError(
            "Cannot serialize unknown enum value: " + QString::number(static_cast<int>(value)));
    return QString(str);
}

} // namespace io
} // namespace warmonger

#endif // W_IO_JSON_SERIALIZER_HPP
