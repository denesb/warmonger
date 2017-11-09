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

#include "core/Field.h"

#include "core/WObject.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

Field::Field(QObject* parent)
    : QObject(parent)
{
}

Field::Field(const QString& name, std::unique_ptr<FieldType>&& type, QObject* parent)
    : QObject(parent)
    , name(name)
    , type(type.release())
{
    this->type->setParent(this);
}

void Field::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void Field::setType(std::unique_ptr<FieldType>&& type)
{
    type->setParent(this);
    this->type = type.release();

    emit typeChanged();
}

namespace FieldTypes {

List::List(std::unique_ptr<FieldType>&& valueType)
    : valueType(valueType.release())
{
    this->valueType->setParent(this);
}

Map::Map(std::unique_ptr<FieldType>&& valueType)
    : valueType(valueType.release())
{
    this->valueType->setParent(this);
}

} // namespace FieldTypes

const std::size_t FieldValue::bufSize{std::max({sizeof(int),
    sizeof(double),
    sizeof(QString),
    sizeof(FieldValue::Reference),
    sizeof(FieldValue::List),
    sizeof(FieldValue::Map)})};

FieldValue::FieldValue()
    : buf(new char[FieldValue::bufSize])
    , null(true)
{
}

FieldValue::FieldValue(int integer)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::Integer)
    , null(false)
{
    *reinterpret_cast<int*>(this->buf.get()) = integer;
}

FieldValue::FieldValue(double real)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::Real)
    , null(false)
{
    *reinterpret_cast<double*>(this->buf.get()) = real;
}

FieldValue::FieldValue(QString string)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::String)
    , null(false)
{
    new (this->buf.get()) QString(std::move(string));
}

FieldValue::FieldValue(Reference reference)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::Reference)
    , null(false)
{
    *reinterpret_cast<Reference*>(this->buf.get()) = reference;
}

FieldValue::FieldValue(List list)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::List)
    , null(false)
{
    new (this->buf.get()) List(std::move(list));
}

FieldValue::FieldValue(Map map)
    : buf(new char[FieldValue::bufSize])
    , typeId(Field::TypeId::Map)
    , null(false)
{
    new (this->buf.get()) Map(std::move(map));
}

FieldValue::~FieldValue()
{
    this->destroy();
}

FieldValue::FieldValue(const FieldValue& other)
    : buf(new char[FieldValue::bufSize])
    , typeId(other.typeId)
    , null(other.null)
{
    switch (this->typeId)
    {
        case Field::TypeId::Integer:
        case Field::TypeId::Real:
        case Field::TypeId::Reference:
            memcpy(this->buf.get(), other.buf.get(), bufSize);
            break;

        case Field::TypeId::String:
            new (this->buf.get()) QString(*reinterpret_cast<QString*>(other.buf.get()));
            break;

        case Field::TypeId::List:
            new (this->buf.get()) List(*reinterpret_cast<List*>(other.buf.get()));
            break;

        case Field::TypeId::Map:
            new (this->buf.get()) Map(*reinterpret_cast<Map*>(other.buf.get()));
            break;
    }
}

FieldValue& FieldValue::operator=(const FieldValue& other)
{
    const bool sameType{!this->null && this->typeId != other.typeId};

    if (!sameType)
        this->destroy();

    this->typeId = other.typeId;
    this->null = other.null;

    if (this->null)
        return *this;

    switch (this->typeId)
    {
        case Field::TypeId::Integer:
        case Field::TypeId::Real:
        case Field::TypeId::Reference:
            memcpy(this->buf.get(), other.buf.get(), bufSize);
            break;

        case Field::TypeId::String:
            if (sameType)
                *reinterpret_cast<QString*>(this->buf.get()) = *reinterpret_cast<QString*>(other.buf.get());
            else
                new (this->buf.get()) QString(*reinterpret_cast<QString*>(other.buf.get()));
            break;

        case Field::TypeId::List:
            if (sameType)
                *reinterpret_cast<List*>(this->buf.get()) = *reinterpret_cast<List*>(other.buf.get());
            else
                new (this->buf.get()) List(*reinterpret_cast<List*>(other.buf.get()));
            break;

        case Field::TypeId::Map:
            if (sameType)
                *reinterpret_cast<Map*>(this->buf.get()) = *reinterpret_cast<Map*>(other.buf.get());
            else
                new (this->buf.get()) Map(*reinterpret_cast<Map*>(other.buf.get()));
            break;
    }

    return *this;
}

FieldValue::FieldValue(FieldValue&& other)
    : buf(std::move(other.buf))
    , typeId(other.typeId)
    , null(other.null)
{
    other.null = true;
}

FieldValue& FieldValue::operator=(FieldValue&& other)
{
    this->destroy();

    this->buf = std::move(other.buf);
    this->typeId = other.typeId;
    this->null = other.null;

    other.null = true;

    return *this;
}

FieldValue& FieldValue::operator=(int integer)
{
    if (!this->isInteger())
        this->destroy();

    this->null = false;
    this->typeId = Field::TypeId::Integer;
    *reinterpret_cast<int*>(this->buf.get()) = integer;

    return *this;
}

FieldValue& FieldValue::operator=(double real)
{
    if (!this->isReal())
        this->destroy();

    this->null = false;
    this->typeId = Field::TypeId::Real;
    *reinterpret_cast<double*>(this->buf.get()) = real;

    return *this;
}

FieldValue& FieldValue::operator=(const QString& string)
{
    if (this->isString())
    {
        *reinterpret_cast<QString*>(this->buf.get()) = string;
    }
    else
    {
        this->destroy();
        new (this->buf.get()) QString(string);
    }

    this->null = false;
    this->typeId = Field::TypeId::String;

    return *this;
}

FieldValue& FieldValue::operator=(QString&& string)
{
    if (this->isString())
    {
        *reinterpret_cast<QString*>(this->buf.get()) = std::move(string);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) QString(std::move(string));
    }

    this->null = false;
    this->typeId = Field::TypeId::String;

    return *this;
}

FieldValue& FieldValue::operator=(Reference reference)
{
    if (!this->isReference())
        this->destroy();

    this->null = false;
    this->typeId = Field::TypeId::Reference;
    *reinterpret_cast<Reference*>(this->buf.get()) = reference;

    return *this;
}

FieldValue& FieldValue::operator=(const List& list)
{
    if (this->isList())
    {
        *reinterpret_cast<List*>(this->buf.get()) = list;
    }
    else
    {
        this->destroy();
        new (this->buf.get()) List(list);
    }

    this->null = false;
    this->typeId = Field::TypeId::List;

    return *this;
}

FieldValue& FieldValue::operator=(List&& list)
{
    if (this->isList())
    {
        *reinterpret_cast<List*>(this->buf.get()) = std::move(list);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) List(std::move(list));
    }

    this->null = false;
    this->typeId = Field::TypeId::List;

    return *this;
}

FieldValue& FieldValue::operator=(const Map& map)
{
    if (this->isMap())
    {
        *reinterpret_cast<Map*>(this->buf.get()) = map;
    }
    else
    {
        this->destroy();
        new (this->buf.get()) Map(map);
    }

    this->null = false;
    this->typeId = Field::TypeId::Map;

    return *this;
}

FieldValue& FieldValue::operator=(Map&& map)
{
    if (this->isMap())
    {
        *reinterpret_cast<Map*>(this->buf.get()) = std::move(map);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) Map(std::move(map));
    }

    this->null = false;
    this->typeId = Field::TypeId::Map;

    return *this;
}

int& FieldValue::asInteger()
{
    if (this->isInteger())
        return *reinterpret_cast<int*>(this->buf.get());

    throw utils::ValueError("Underlying value is not integer");
}

double& FieldValue::asReal()
{
    if (this->isReal())
        return *reinterpret_cast<double*>(this->buf.get());

    throw utils::ValueError("Underlying value is not real");
}

QString& FieldValue::asString()
{
    if (this->isString())
        return *reinterpret_cast<QString*>(this->buf.get());

    throw utils::ValueError("Underlying value is not string");
}

FieldValue::Reference& FieldValue::asReference()
{
    if (this->isReference())
        return *reinterpret_cast<Reference*>(this->buf.get());

    throw utils::ValueError("Underlying value is not reference");
}

FieldValue::List& FieldValue::asList()
{
    if (this->isList())
        return *reinterpret_cast<List*>(this->buf.get());

    throw utils::ValueError("Underlying value is not list");
}

FieldValue::Map& FieldValue::asMap()
{
    if (this->isMap())
        return *reinterpret_cast<Map*>(this->buf.get());

    throw utils::ValueError("Underlying value is not map");
}

const int& FieldValue::asInteger() const
{
    if (this->isInteger())
        return *reinterpret_cast<int*>(this->buf.get());

    throw utils::ValueError("Underlying value is not integer");
}

const double& FieldValue::asReal() const
{
    if (this->isReal())
        return *reinterpret_cast<double*>(this->buf.get());

    throw utils::ValueError("Underlying value is not real");
}

const QString& FieldValue::asString() const
{
    if (this->isString())
        return *reinterpret_cast<QString*>(this->buf.get());

    throw utils::ValueError("Underlying value is not string");
}

const FieldValue::Reference& FieldValue::asReference() const
{
    if (this->isReference())
        return *reinterpret_cast<Reference*>(this->buf.get());

    throw utils::ValueError("Underlying value is not reference");
}

const FieldValue::List& FieldValue::asList() const
{
    if (this->isList())
        return *reinterpret_cast<List*>(this->buf.get());

    throw utils::ValueError("Underlying value is not list");
}
const FieldValue::Map& FieldValue::asMap() const
{
    if (this->isMap())
        return *reinterpret_cast<Map*>(this->buf.get());

    throw utils::ValueError("Underlying value is not map");
}

QVariant FieldValue::toQVariant() const
{
    switch (this->typeId)
    {
        case Field::TypeId::Integer:
        {
            return QVariant(this->asInteger());
        }

        case Field::TypeId::Real:
        {
            return QVariant(this->asReal());
        }

        case Field::TypeId::Reference:
        {
            return QVariant::fromValue(this->asReference());
        }

        case Field::TypeId::String:
        {
            return QVariant(this->asString());
        }

        case Field::TypeId::List:
        {
            QVariantList qvlist;
            const auto& list = this->asList();

            for (auto& element : list)
            {
                qvlist.push_back(element.toQVariant());
            }

            return qvlist;
        }

        case Field::TypeId::Map:
        {
            QVariantMap qvmap;
            const auto& map = this->asMap();

            for (auto& element : map)
            {
                qvmap[element.first] = QVariant(element.second.toQVariant());
            }

            return qvmap;
        }
    }

    throw utils::ValueError("Invalid field type");
}

void FieldValue::set(int integer)
{
    *this = integer;
}

void FieldValue::set(double real)
{
    *this = real;
}

void FieldValue::set(QString string)
{
    *this = std::move(string);
}

void FieldValue::set(WObject* reference)
{
    *this = reference;
}

void FieldValue::set(List list)
{
    *this = std::move(list);
}

void FieldValue::set(Map map)
{
    *this = std::move(map);
}

int& FieldValue::makeInteger()
{
    if (this->isInteger())
    {
        return *reinterpret_cast<int*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::Integer;
        return *(new (this->buf.get()) int);
    }
}

double& FieldValue::makeReal()
{
    if (this->isReal())
    {
        return *reinterpret_cast<double*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::Real;
        return *(new (this->buf.get()) double);
    }
}

QString& FieldValue::makeString()
{
    if (this->isString())
    {
        return *reinterpret_cast<QString*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::String;
        return *(new (this->buf.get()) QString);
    }
}

FieldValue::Reference& FieldValue::makeReference()
{
    if (this->isReference())
    {
        return *reinterpret_cast<Reference*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::Reference;
        return *(new (this->buf.get()) Reference);
    }
}

FieldValue::List& FieldValue::makeList()
{
    if (this->isList())
    {
        return *reinterpret_cast<List*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::List;
        return *(new (this->buf.get()) List);
    }
}

FieldValue::Map& FieldValue::makeMap()
{
    if (this->isMap())
    {
        return *reinterpret_cast<Map*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->null = false;
        this->typeId = Field::TypeId::Map;
        return *(new (this->buf.get()) Map);
    }
}

void FieldValue::destroy()
{
    if (this->null)
        return;

    switch (typeId)
    {
        case Field::TypeId::Integer:
        case Field::TypeId::Real:
        case Field::TypeId::Reference:
            break;

        case Field::TypeId::String:
            reinterpret_cast<QString*>(this->buf.get())->~QString();
            break;

        case Field::TypeId::List:
            reinterpret_cast<List*>(this->buf.get())->~vector<FieldValue>();
            break;

        case Field::TypeId::Map:
            reinterpret_cast<Map*>(this->buf.get())->~map<QString, FieldValue>();
            break;
    }

    this->null = true;
}

bool operator==(const FieldValue& a, const FieldValue& b)
{
    if (a.getTypeId() != b.getTypeId())
        return false;

    switch (a.getTypeId())
    {
        case Field::TypeId::Integer:
            return a.asInteger() == b.asInteger();
        case Field::TypeId::Real:
            return a.asReal() == b.asReal();
        case Field::TypeId::String:
            return a.asString() == b.asString();
        case Field::TypeId::Reference:
            return a.asReference() == b.asReference();
        case Field::TypeId::List:
            return a.asList() == b.asList();
        case Field::TypeId::Map:
            return a.asMap() == b.asMap();
    }

    return false;
}

bool operator<(const FieldValue& a, const FieldValue& b)
{
    if (a.getTypeId() != b.getTypeId())
        throw utils::ValueError("Cannot compare fields of different types");

    switch (a.getTypeId())
    {
        case Field::TypeId::Integer:
            return a.asInteger() < b.asInteger();
        case Field::TypeId::Real:
            return a.asReal() < b.asReal();
        case Field::TypeId::String:
            return a.asString() < b.asString();
        case Field::TypeId::Reference:
            return a.asReference() < b.asReference();
        case Field::TypeId::List:
            return a.asList() < b.asList();
        case Field::TypeId::Map:
            return a.asMap() < b.asMap();
    }

    return false;
}

} // namespace core
} // namespace warmonger
