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

#include "core/IntermediateRepresentation.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace warmonger {
namespace core {
namespace ir {

static Reference serializeReference(core::WObject* obj);
static QObject* findObjectTreeNode(QObject* obj, const QString& className);

std::ostream& operator<<(std::ostream& os, Type t)
{
    switch (t)
    {
        case Type::Null:
            os << "Null";
            break;
        case Type::Boolean:
            os << "Boolean";
            break;
        case Type::Integer:
            os << "Integer";
            break;
        case Type::Real:
            os << "Real";
            break;
        case Type::String:
            os << "String";
            break;
        case Type::Reference:
            os << "Reference";
            break;
        case Type::List:
            os << "List";
            break;
        case Type::Map:
            os << "List";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Reference& ref)
{
    os << "{ parent: " << ref.parentClassName << ", object: " << ref.objectClassName << ", id: " << ref.id << "}";
    return os;
}

struct Data
{
    Data(bool boolean)
        : type(Type::Boolean)
        , boolean(boolean)
    {
    }

    Data(int integer)
        : type(Type::Integer)
        , integer(integer)
    {
    }

    Data(double real)
        : type(Type::Real)
        , real(real)
    {
    }

    Data(QString string)
        : type(Type::String)
    {
        new (&this->string) QString(std::move(string));
    }

    Data(Reference reference)
        : type(Type::Reference)
    {
        new (&this->reference) Reference(std::move(reference));
    }

    Data(std::vector<Value> list)
        : type(Type::List)
    {
        new (&this->list) std::vector<Value>(std::move(list));
    }

    Data(std::unordered_map<QString, Value> map)
        : type(Type::Map)
    {
        new (&this->map) std::unordered_map<QString, Value>(std::move(map));
    }

    ~Data()
    {
        switch (this->type)
        {
            case Type::Null:
            case Type::Boolean:
            case Type::Integer:
            case Type::Real:
                break;
            case Type::String:
                this->string.~QString();
                break;
            case Type::Reference:
                this->reference.~Reference();
                break;
            case Type::List:
                this->list.~vector<Value>();
                break;
            case Type::Map:
                this->map.~unordered_map<QString, Value>();
                break;
        }
    }

    Type type = Type::Null;
    // FIXME: Use std::variant<> after the switch to C++17
    union
    {
        bool boolean;
        int integer;
        double real;
        QString string;
        Reference reference;
        std::vector<Value> list;
        std::unordered_map<QString, Value> map;
    };
};

Value::Value()
{
}

Value::Value(bool boolean)
    : data(std::make_unique<Data>(boolean))
{
}

Value::Value(int integer)
    : data(std::make_unique<Data>(integer))
{
}

Value::Value(double real)
    : data(std::make_unique<Data>(real))
{
}

Value::Value(QString string)
    : data(std::make_unique<Data>(std::move(string)))
{
}

Value::Value(WObject* reference)
    : data(std::make_unique<Data>(serializeReference(reference)))
{
}

Value::Value(Reference reference)
    : data(std::make_unique<Data>(std::move(reference)))
{
}

Value::Value(std::vector<Value> list)
    : data(std::make_unique<Data>(std::move(list)))
{
}

Value::Value(std::unordered_map<QString, Value> map)
    : data(std::make_unique<Data>(std::move(map)))
{
}

Value::Value(QColor color)
    : Value(color.name())
{
}

// Only because the definition of Data is not available in the header.
Value::~Value()
{
}

Value::Value(Value&& other)
    : data(std::move(other.data))
{
}

Value& Value::operator=(Value&& v)
{
    this->data = std::move(v.data);
    return *this;
}

Type Value::getType() const
{
    if (this->data)
        return this->data->type;
    return Type::Null;
}

bool Value::asBoolean() const
{
    this->throwIfIncompatibleValue(Type::Boolean);
    return this->data->boolean;
}

int Value::asInteger() const
{
    this->throwIfIncompatibleValue(Type::Integer);
    if (this->getType() == Type::Integer)
        return this->data->integer;
    else
        return this->data->real;
}

double Value::asReal() const
{
    this->throwIfIncompatibleValue(Type::Real);
    if (this->getType() == Type::Integer)
        return this->data->integer;
    else
        return this->data->real;
}

const QString& Value::asString() const &
{
    this->throwIfIncompatibleValue(Type::String);
    return this->data->string;
}

QString Value::asString() &&
{
    this->throwIfIncompatibleValue(Type::String);
    return std::move(this->data->string);
}

const Reference& Value::asReference() const &
{
    this->throwIfIncompatibleValue(Type::Reference);
    return this->data->reference;
}

Reference Value::asReference() &&
{
    this->throwIfIncompatibleValue(Type::Reference);
    return std::move(this->data->reference);
}

const std::vector<Value>& Value::asList() const &
{
    this->throwIfIncompatibleValue(Type::List);
    return this->data->list;
}

std::vector<Value> Value::asList() &&
{
    this->throwIfIncompatibleValue(Type::List);
    return std::move(this->data->list);
}

const std::unordered_map<QString, Value>& Value::asMap() const &
{
    this->throwIfIncompatibleValue(Type::Map);
    return this->data->map;
}

std::unordered_map<QString, Value> Value::asMap() &&
{
    this->throwIfIncompatibleValue(Type::Map);
    return std::move(this->data->map);
}

QColor Value::asColor() const
{
    this->throwIfIncompatibleValue(Type::String);
    auto c = QColor(this->data->string);
    if (!c.isValid())
        throw utils::ValueError(fmt::format("Incompatible value: {} is not a valid color name", this->data->string));

    return c;
}

ObjectId Value::getObjectId() const
{
    this->throwIfIncompatibleValue(Type::Map);
    return ObjectId(this->data->map["id"].asInteger());
}

void Value::throwIfIncompatibleValue(Type t) const
{
    switch (t)
    {
        case Type::Boolean:
        case Type::String:
        case Type::Reference:
        case Type::List:
        case Type::Map:
            if (t != this->getType())
                throw utils::ValueError(fmt::format("Incompatible value: expected {} but got {}", t, this->getType()));

            break;

        case Type::Integer:
            if (this->getType() != Type::Integer && this->getType() != Type::Real)
                throw utils::ValueError(fmt::format("Incompatible value: expected {} but got {}", t, this->getType()));

            if (this->getType() == Type::Real &&
                static_cast<double>(static_cast<int>(this->data->real)) != this->data->real)
                throw utils::ValueError(fmt::format("Incompatible value: expected {} but stored value is invalid", t));

            break;

        case Type::Real:
            if (this->getType() != Type::Integer && this->getType() != Type::Real)
                throw utils::ValueError(fmt::format("Incompatible value: expected {} but got {}", t, this->getType()));

            break;

        case Type::Null:
            break;
    }
}

WObject* Value::asResolvedReference(QObject* parent) const
{
    if (!parent)
        throw utils::ValueError("Cannot resolve reference with null parent");

    auto& ref = this->asReference();

    if (ref.id == core::ObjectId::Invalid)
        return nullptr;

    auto* root = findObjectTreeNode(parent, ref.parentClassName);

    if (root == nullptr)
        throw utils::ValueError(
            fmt::format("Failed to unserialize reference {}: no parent found with required class", ref));

    auto children{root->findChildren<core::WObject*>()};
    auto it = std::find_if(children.cbegin(), children.cend(), [&ref](auto child) {
        return ref.objectClassName == child->metaObject()->className() && ref.id == child->getId();
    });

    if (it == children.cend())
        throw utils::ValueError(
            fmt::format("Failed to unserialize reference {}: no matching object found in parent {}", ref, *root));

    return *it;
}

static Reference serializeReference(core::WObject* obj)
{
    if (!obj)
        return {QString(), QString(), ObjectId::Invalid};

    const QMetaObject* metaObj = obj->metaObject();

    QObject* root = core::getObjectTreeRoot(obj);

    if (root == nullptr)
        return {QString(), QString(metaObj->className()), obj->getId()};

    const QMetaObject* rootMetaObj = root->metaObject();

    return {QString(rootMetaObj->className()), QString(metaObj->className()), obj->getId()};
}

static QObject* findObjectTreeNode(QObject* obj, const QString& className)
{
    if (obj == nullptr)
        return nullptr;
    else if (obj->metaObject()->className() == className)
        return obj;
    else
        return findObjectTreeNode(obj->parent(), className);
}

} // namespace ir
} // namespace core
} // namespace warmonger
