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

#ifndef W_IO_JSON_UNSERIALIZER_HPP
#define W_IO_JSON_UNSERIALIZER_HPP

#include <cassert>
#include <map>
#include <memory>
#include <vector>

#include <QColor>
#include <QJsonObject>

#include "io/Visitor.hpp"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

template <typename T>
struct typeTag {};

int unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<int>);

QString unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<QString>);

QColor unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<QColor>);

template <typename T>
std::unique_ptr<T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::unique_ptr<T>>);

template <typename T>
T* unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<T*>);

template <typename T>
std::vector<T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::vector<T>>);

template <typename T>
std::map<QString, T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::map<QString, T>>);

template <typename T>
typename std::enable_if<std::is_enum<T>::value, T>::type
unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<T>);

template <typename T>
auto unserializeKeyFromJson(const QJsonObject& jobj, const char* name, QObject* parent);

template<typename ConstructorArgDefs, std::size_t... I>
inline auto unserializeConstructorArgsFromJsonImpl(const QJsonObject& jobj, QObject* parent, const ConstructorArgDefs& defs, std::index_sequence<I...>)
{
    return std::make_tuple(unserializeKeyFromJson<typename StripType<decltype(std::get<I>(defs))>::Type>(jobj, std::get<I>(defs).name, parent)...);
}

template <typename ConstructorArgDefs>
inline auto unserializeConstructorArgsFromJson(const QJsonObject& jobj, QObject* parent, const ConstructorArgDefs& defs)
{
    return unserializeConstructorArgsFromJsonImpl(jobj, parent, defs, std::make_index_sequence<std::tuple_size<ConstructorArgDefs>::value>());
}

template <typename T, typename Args, std::size_t... I>
inline std::unique_ptr<T> createObjectImpl(Args&& args, std::index_sequence<I...>)
{
    return std::make_unique<T>(std::get<I>(args)...);
}

template <typename T, typename Args>
inline std::unique_ptr<T> createObject(Args&& args)
{
    return createObjectImpl<T>(std::forward<Args>(args), std::make_index_sequence<std::tuple_size<Args>::value>());
}

template <typename C, typename T1, typename T2>
inline void invokeSetter(C& obj, const std::function<void(C&, T1)>& setter, T2&& value)
{
    setter(obj, std::move(std::forward<T2>(value)));
}

template <typename C, typename T1, typename T2, typename T3>
inline void invokeSetter(C& obj, const std::function<T1(C&, std::unique_ptr<T2>)>& setter, T3&& value)
{
    for (auto& e : value)
    {
        setter(obj, std::move(std::move(e)));
    }
}

template<typename T, typename MemberDef>
inline void unserializeMemberFromJsonImpl(const QJsonObject& jobj, T& obj, const MemberDef& def)
{
    if (!def.setter)
        return;

    invokeSetter(obj, def.setter, unserializeKeyFromJson<typename MemberDef::TypeSet>(jobj, def.name, &obj));
}

template<typename T, typename MemberDef>
inline void unserializeMembersFromJsonImpl(const QJsonObject& jobj, T& obj, const MemberDef& def)
{
    unserializeMemberFromJsonImpl(jobj, obj, def);
}

template<typename T, typename FirstMemberDef, typename ...MemberDefs>
inline void unserializeMembersFromJsonImpl(const QJsonObject& jobj, T& obj, const FirstMemberDef& firstDef, const MemberDefs&... defs)
{
    unserializeMemberFromJsonImpl(jobj, obj, firstDef);
    unserializeMemberFromJsonImpl(jobj, obj, defs...);
}

template <typename T, typename MemberDefs, std::size_t... I>
inline void unserializeMembersFromJson(const QJsonObject& jobj, T& obj, const MemberDefs& defs, std::index_sequence<I...>)
{
    unserializeMembersFromJsonImpl(jobj, obj, std::get<I>(defs)...);
}

template <typename T, typename MemberDefs>
inline void unserializeMembersFromJson(const QJsonObject& jobj, T& obj, const MemberDefs& defs)
{
    unserializeMembersFromJson(jobj, obj, defs, std::make_index_sequence<std::tuple_size<MemberDefs>::value>());
}

template <typename T>
std::unique_ptr<T> unserializeFromJson(const QJsonObject& jobj, QObject* parent)
{
    auto description = T::describe(Visitor<T>());

    auto unserializedConstructorArgs = unserializeConstructorArgsFromJson(jobj, parent, description.getConstructorArgs().asTuple());
    auto obj = createObject<T>(std::move(unserializedConstructorArgs));

    unserializeMembersFromJson(jobj, *obj, description.getMembers().asTuple());

    return obj;
}

// Implementations

inline QJsonValue getKey(const QJsonObject& jobj, const char* key)
{
    if (!jobj.contains(key))
        throw utils::ValueError(QString("Object doesn't have key %1").arg(key));

    return jobj[key];
}

inline int unserializeValueFromJson(const QJsonValue& jval, QObject*, typeTag<int>)
{
    if (!jval.isDouble())
        throw utils::ValueError("Value is not a number");

    const double d = jval.toDouble();
    const int i = d;
    if (static_cast<double>(i) != d)
        throw utils::ValueError("Value is not an integer");

    return i;
}

inline QString unserializeValueFromJson(const QJsonValue& jval, QObject*, typeTag<QString>)
{
    if (!jval.isString())
        throw utils::ValueError("Value is not string");

    return jval.toString();
}

inline QColor unserializeValueFromJson(const QJsonValue& jval, QObject*, typeTag<QColor>)
{
    if (!jval.isString())
        throw utils::ValueError("Expected color but value is not string");

    QColor color(jval.toString());

    if (!color.isValid())
        throw utils::ValueError(QStringLiteral("``%1'' is not a valid color name").arg(jval.toString()));

    return color;
}

template <typename T>
inline std::unique_ptr<T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::unique_ptr<T>>)
{
    if (!jval.isObject())
        throw utils::ValueError("Expected object but value is not object");

    return unserializeFromJson<T>(jval.toObject(), parent);
}

template <typename T>
inline T* unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<T*>)
{
    if (!jval.isString())
        throw utils::ValueError("Expected reference type but value is not string");

    return unserializeReferenceAs<T>(jval.toString(), parent);
}

template <typename T>
inline std::vector<T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::vector<T>>)
{
    if (!jval.isArray())
        throw utils::ValueError(QString("Value is not array"));

    auto jarr = jval.toArray();
    std::vector<T> array;
    array.reserve(jarr.size());
    for (auto jelement : jarr)
    {
        array.emplace_back(unserializeValueFromJson(jelement, parent, typeTag<T>{}));
    }

    return array;
}

template <typename T>
std::map<QString, T> unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<std::map<QString, T>>)
{
    if (!jval.isObject())
        throw utils::ValueError(QString("Expected map but value is not object"));

    const auto jobj = jval.toObject();
    std::map<QString, T> map;

    for (auto it = jobj.begin(); it != jobj.end(); ++it)
    {
        try
        {
            map.emplace(it.key(), unserializeValueFromJson(it.value(), parent, typeTag<T>{}));
        }
        catch (const utils::ValueError& e)
        {
            throw utils::ValueError(e, QStringLiteral("Failed to unserialize element with key ``%1''").arg(it.key()));
        }
    }

    return map;
}

template <typename T>
inline typename std::enable_if<std::is_enum<T>::value, T>::type
unserializeValueFromJson(const QJsonValue& jval, QObject*, typeTag<T>)
{
    if (!jval.isString())
        throw utils::ValueError("Expected enum type but value is not string");

    const QMetaEnum metaEnum{QMetaEnum::fromType<T>()};
    bool ok{true};
    auto e = metaEnum.keyToValue(jval.toString().toLocal8Bit().data(), &ok);

    if (!ok)
        throw utils::ValueError("Failed to unserialize enum");

    return static_cast<T>(e);
}

template <typename T>
inline auto unserializeKeyFromJson(const QJsonObject& jobj, const char* name, QObject* parent)
{
    try
    {
        return unserializeValueFromJson(getKey(jobj, name), parent, typeTag<T>{});
    }
    catch (const utils::ValueError& e)
    {
        throw utils::ValueError(e, QString("Failed to unserialize member ``%1''").arg(name));
    }
}

template <>
inline auto unserializeKeyFromJson<QObject*>(const QJsonObject&, const char* name, QObject* parent)
{
    // The only supported QObject* is the special reserved parent.
    assert(strncmp(name, "parent", 6) == 0);
    return parent;
}

} // namespace io
} // namespace warmonger

#endif // W_IO_JSON_UNSERIALIZER_HPP
