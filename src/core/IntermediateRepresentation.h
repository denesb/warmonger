/** \file
 * Intermedate Representation.
 *
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
#ifndef W_CORE_INTERMEDIATE_REPRESENTATION_H
#define W_CORE_INTERMEDIATE_REPRESENTATION_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <fmt/ostream.h>
#include <QColor>
#include <QString>

#include "core/WObject.h"
#include "utils/Exception.h"
#include "utils/ToString.h"
#include "utils/Hash.h"
#include "utils/Utils.h"

namespace warmonger {
namespace core {
namespace ir {

enum class Type
{
    Null = 0,
    Boolean,
    Integer,
    Real,
    String,
    Reference,
    List,
    Map
};

std::ostream& operator<<(std::ostream& os, Type t);

struct Reference
{
    QString parentClassName;
    QString objectClassName;
    ObjectId id;
};

std::ostream& operator<<(std::ostream& os, const Reference& ref);

class Data;

/**
 * Intermediate representation of a value, object or container.
 *
 * Intermediate representation provides a bridge between on-disk or
 * on-network serialization formats and in-memory objects. All core
 * objects know how to serialize/unserialize themselves to/from the
 * intermediate representation. Thus the representation logic is kept
 * local to the objects themselves and at the same time the io layer
 * only needs to know how to serialize the intermediate representation
 * and thus stay simple and independent of the serialized objects.
 *
 * Value can represent 8 different types. These largly match the types
 * representable by JSON. The types are mapped to C++ types as follows:
 *
 * Type            | C++ type
 * ----------------|--------------------------
 * Type::Null      | -
 * Type::Integer   | int64
 * Type::Real      | double
 * Type::String    | QString
 * Type::Reference | Reference
 * Type::List      | std::vector<Value>
 * Type::Map       | std::map<QString, Value>
 *
 * In addiotion the following additional C++ types are supported
 * indirectly (with the help of the above types):
 *
 * C++ Type        | Via Type
 * ----------------|--------------------------
 * Enums           | Type::String
 * QColor          | Type::String
 */
class Value
{
public:
    /**
     * Construct a Null Value.
     */
    Value();

    /**
     * @name Construct directly from a value.
     *
     * @{
     */
    Value(bool boolean);
    Value(int integer);
    Value(double real);
    Value(QString string);
    Value(WObject* reference);
    Value(Reference reference);
    Value(std::vector<Value> list);
    Value(std::unordered_map<QString, Value> map);
    /** @} */

    /**
     * @name Construct directly from indirectly supported value.
     *
     * @{
     */
    template <typename Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
    Value(Enum e);

    Value(QColor color);
    /** @} */

    ~Value();

    Value(Value&&);
    Value& operator=(Value&&);

    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;

    Type getType() const;

    /**
     * @name Accessors.
     *
     * Will return a reference to the underlying value.
     * Non-literal types have both lvalue and rvalue overloads.
     *
     * \throws utils::ValueError if the underlying type is different
     * than the requested one.
     * @{
     */
    bool asBoolean() const;
    int asInteger() const;
    double asReal() const;
    const QString& asString() const&;
    QString asString() &&;
    const Reference& asReference() const&;
    Reference asReference() &&;
    const std::vector<Value>& asList() const&;
    std::vector<Value> asList() &&;
    const std::unordered_map<QString, Value>& asMap() const&;
    std::unordered_map<QString, Value> asMap() &&;
    /**
     * Alias to asMap();
     */
    const std::unordered_map<QString, Value>& asObject() const&
    {
        return this->asMap();
    }
    /**
     * Alias to asMap();
     */
    std::unordered_map<QString, Value> asObject() &&
    {
        return std::move(*this).asMap();
    }

    /**
     * Resolve the reference as `T'.
     *
     * If the reference cannot be resolved or the resulting pointer
     * cannot be casted to `T' utils::ValueError is throws.
     *
     * \throws utils::ValueError if the reference cannot be resolved or
     * casted to `T'.
     */
    template <typename T>
    T* asReference(QObject* parent) const;
    /** @} */

    /**
     * @name Accessors for indirectly supported types.
     *
     * \throws utils::ValueError if the underlying value cannot be
     * converted into the indirectly supported type.
     * @{
     */
    /**
     * Convert the value to a member value of `Enum'.
     *
     * The underlying value must be a string and a valid representation
     * of one of the member values of `Enum'.
     *
     * \throws utils::ValueError if the value is invalid
     */
    template <typename Enum>
    Enum asEnum() const
    {
        return utils::enumFromString<Enum>(this->asString());
    }

    /**
     * Convert the value to a QColor.
     *
     * The unredlying type must be a String and a valid color-name.
     */
    QColor asColor() const;
    /** @} */

    /**
     * Helper method to get the object id entry of this value.
     *
     * This is so often used during deserializing WObjects that it's
     * worth moving here. Furthermore the object-id is needed before
     * the (unserializing) constructor bodies are executed.
     * This method assumes the Value is a Map, and it has an entry with
     * the key "id", which is an Integer.
     *
     * \throws utils::ValueError if any of the above assumptions
     * doesn't hold.
     */
    ObjectId getObjectId() const;

private:
    void throwIfIncompatibleValue(Type t) const;
    WObject* asResolvedReference(QObject* parent) const;

    std::unique_ptr<Data> data;
};

template <typename Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
Value::Value(Enum e)
    : Value(utils::enumToString(e))
{
}

template <typename T>
T* Value::asReference(QObject* parent) const
{
    auto* wobj = this->asResolvedReference(parent);

    if (!wobj)
        return nullptr;

    auto* obj = qobject_cast<T*>(wobj);

    if (obj == nullptr)
        throw utils::ValueError(fmt::format(
            "Failed to cast resolved reference {} to requested type {}", *wobj, T::staticMetaObject.className()));

    return obj;
}

struct Serializable
{
    /**
     * Serialize the object into an intermediate-representation.
     */
    virtual ir::Value serialize() const = 0;
};

} // namespace ir
} // namespace core
} // namespace warmonger

#endif // W_CORE_INTERMEDIATE_REPRESENTATION_H
