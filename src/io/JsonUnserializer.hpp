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

#include <memory>
#include <cassert>

#include <QJsonObject>

#include "io/Visitor.hpp"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

template <typename T>
struct typeTag {};

int unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<int>);

QString unserializeValueFromJson(const QJsonValue& jval, QObject* parent, typeTag<QString>);

template <typename T>
T unserializeValueFromJson(const QJsonObject& jobj, const char* name, QObject* parent);

/*
template<typename ConstructorArgDef>
auto unserializeConstructorArgsFromJson(const QJsonObject& jobj, const ConstructorArgDef& def)
{
   return unserializeValueFromJson(jobj, def.name, nullptr, typename typeTag<ConstructorArgDef::Type>);
}

template<typename ConstructorArgDef, typename... ConstructorArgDefs>
auto unserializeConstructorArgsFromJson(const QJsonObject& jobj, const ConstructorArgDef& firstDef, const ConstructorArgDefs... defs)
{
   return unserializeValueFromJson(jobj, def.name, nullptr, typename typeTag<ConstructorArgDef::Type>);
}
*/

template<typename ConstructorArgDefs, std::size_t... I>
inline auto unserializeConstructorArgsFromJsonImpl(const QJsonObject& jobj, QObject* parent, const ConstructorArgDefs& defs, std::index_sequence<I...>)
{
    return std::make_tuple(unserializeValueFromJson<typename StripType<decltype(std::get<I>(defs))>::Type>(jobj, std::get<I>(defs).name, parent)...);
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

template<typename T, typename MemberDef>
inline void unserializeMemberFromJsonImpl(const QJsonObject& jobj, T& obj, const MemberDef& def)
{
    if (!def.setter)
        return;

    def.setter(obj, unserializeValueFromJson<typename MemberDef::Type>(jobj, def.name, &obj));
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

    //TODO: validate that it's a whole number (no trimming)
    return jval.toInt();
}

inline QString unserializeValueFromJson(const QJsonValue& jval, QObject*, typeTag<QString>)
{
    if (!jval.isString())
        throw utils::ValueError("Value is not string");

    return jval.toString();
}

template <typename T>
inline T unserializeValueFromJson(const QJsonObject& jobj, const char* name, QObject* parent)
{
    try
    {
        return unserializeValueFromJson(getKey(jobj, name), parent, typeTag<T>{});
    }
    catch (...)
    {
        std::throw_with_nested(utils::ValueError(QString("Failed to unserialize member %1").arg(name)));
    }
}

template <>
inline QObject* unserializeValueFromJson<QObject*>(const QJsonObject&, const char* name, QObject* parent)
{
	// The only supported QObject* is the special reserved parent.
	assert(strncmp(name, "parent", 6) == 0);
	return parent;
}

} // namespace io
} // namespace warmonger

#endif // W_IO_JSON_UNSERIALIZER_HPP
