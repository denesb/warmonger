/**
 * \copyright (C) 2015-2018 Botond Dénes
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

#include "io/JsonSerializer.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegExp>
#include <experimental/optional>
#include <fmt/format.h>

namespace warmonger {
namespace io {

static QJsonValue toQJsonValue(core::ir::Value v);
static QJsonValue toString(core::ir::Reference ref);
static QJsonValue toQJsonArray(std::vector<core::ir::Value> list);
static QJsonValue toQJsonObject(std::unordered_map<QString, core::ir::Value> map);

static QJsonDocument parseJson(const QByteArray& json);

static core::ir::Value toIRValue(QJsonValue v);
static std::experimental::optional<std::tuple<QString, QString, core::ObjectId>> parseReference(
    const QString& reference);
static bool isReference(const QString& str);
static core::ir::Value toIRReference(QString strRef);
static core::ir::Value toIRList(QJsonArray jarr);
static core::ir::Value toIRMap(QJsonObject jobj);

QByteArray JsonSerializer::serialize(core::ir::Value v) const
{
    auto jval = toQJsonValue(std::move(v));

    if (jval.isObject())
    {
        return QJsonDocument(jval.toObject()).toJson();
    }
    else if (jval.isArray())
    {
        return QJsonDocument(jval.toArray()).toJson();
    }
    else
    {
        // Shouldn't happen but just in case...
        QJsonObject jobj;
        jobj["value"] = jval;
        return QJsonDocument(jobj).toJson();
    }
}

core::ir::Value JsonSerializer::unserialize(const QByteArray& data) const
{
    auto jdoc = parseJson(data);

    if (jdoc.isObject())
    {
        auto jobj = jdoc.object();
        if (jobj.size() == 1 && jobj.contains("value"))
        {
            return toIRValue(jobj["value"]);
        }
        else
        {
            return toIRValue(std::move(jobj));
        }
    }
    else if (jdoc.isArray())
    {
        return toIRValue(jdoc.array());
    }

    return {};
}

static QJsonValue toQJsonValue(core::ir::Value v)
{
    switch (v.getType())
    {
        case core::ir::Type::Null:
            return {};
        case core::ir::Type::Boolean:
            return v.asBoolean();
        case core::ir::Type::Integer:
            return v.asInteger();
        case core::ir::Type::Real:
            return v.asReal();
        case core::ir::Type::String:
            return std::move(v).asString();
        case core::ir::Type::Reference:
            return toString(v.asReference());
        case core::ir::Type::List:
            return toQJsonArray(std::move(v).asList());
        case core::ir::Type::Map:
            return toQJsonObject(std::move(v).asMap());
    }

    throw utils::ValueError(fmt::format("ir::Value has invalid type `{}'", static_cast<int>(v.getType())));
}

static QJsonValue toString(core::ir::Reference ref)
{
    if (ref.id == core::ObjectId::Invalid)
        return QStringLiteral("ref:nullptr");
    else
        return QStringLiteral("ref:%1/%2#%3").arg(ref.parentClassName).arg(ref.objectClassName).arg(ref.id.get());
}

static QJsonValue toQJsonArray(std::vector<core::ir::Value> list)
{
    QJsonArray jar;

    for (auto& v : list)
    {
        jar.push_back(toQJsonValue(std::move(v)));
    }

    return jar;
}

static QJsonValue toQJsonObject(std::unordered_map<QString, core::ir::Value> map)
{
    QJsonObject jobj;

    for (auto& elem : map)
    {
        jobj[elem.first] = toQJsonValue(std::move(elem.second));
    }

    return jobj;
}

static QJsonDocument parseJson(const QByteArray& json)
{
    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(json, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError(parseError.errorString() + " at offset " + QString::number(parseError.offset));
    }

    return doc;
}

static core::ir::Value toIRValue(QJsonValue v)
{
    switch (v.type())
    {
        case QJsonValue::Null:
        case QJsonValue::Undefined: {
            return {};
        }
        case QJsonValue::Bool: {
            return {v.toBool()};
        }
        case QJsonValue::Double: {
            return {v.toDouble()};
        }
        case QJsonValue::String: {
            auto jstr = v.toString();
            if (isReference(jstr))
                return toIRReference(std::move(jstr));
            else
                return {std::move(jstr)};
        }
        case QJsonValue::Array: {
            return toIRList(v.toArray());
        }
        case QJsonValue::Object: {
            return toIRMap(v.toObject());
        }
    }

    throw utils::ValueError(fmt::format("QJsonValue has invalid type `{}'", static_cast<int>(v.type())));
}

static std::experimental::optional<std::tuple<QString, QString, core::ObjectId>> parseReference(
    const QString& reference)
{
    const QRegExp regExp("ref:(nullptr|(([a-zA-Z0-9:]+)/([a-zA-Z0-9:]+)#([0-9]+)))");

    if (!regExp.exactMatch(reference))
        return {};

    const auto captures = regExp.capturedTexts();

    // nullptr
    if (captures[1] == "nullptr")
        return std::make_tuple(QString(), QString(), core::ObjectId());

    bool ok{false};
    const core::ObjectId id{captures[5].toInt(&ok)};

    if (ok)
        return std::make_tuple(captures[3], captures[4], id);
    else
        return {};
}

static bool isReference(const QString& str)
{
    return bool(parseReference(str));
}

static core::ir::Value toIRReference(QString strRef)
{
    core::ir::Reference ref;
    std::tie(ref.parentClassName, ref.objectClassName, ref.id) = *parseReference(strRef);
    return {ref};
}

static core::ir::Value toIRList(QJsonArray jarr)
{
    std::vector<core::ir::Value> list;

    for (auto jval : jarr)
    {
        list.emplace_back(toIRValue(std::move(jval)));
    }

    return list;
}

static core::ir::Value toIRMap(QJsonObject jobj)
{
    std::unordered_map<QString, core::ir::Value> map;

    for (auto it = jobj.begin(); it != jobj.end(); ++it)
    {
        map.emplace(it.key(), toIRValue(it.value()));
    }

    return map;
}

} // namespace io
} // namespace warmonger
