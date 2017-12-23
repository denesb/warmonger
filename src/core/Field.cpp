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

#include <QMetaEnum>

#include "core/WObject.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

Field::Field(QObject* parent)
    : QObject(parent)
{
}

Field::Field(const QString& name, Type type, QObject* parent)
    : QObject(parent)
    , name(name)
    , type(type)
{
}

void Field::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void Field::setType(Type type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

const std::size_t FieldValue::bufSize{std::max({sizeof(int),
    sizeof(double),
    sizeof(QString),
    sizeof(FieldValue::Reference),
    sizeof(FieldValue::List),
    sizeof(FieldValue::Map),
    sizeof(FieldValue::ExternalValue)})};

FieldValue::FieldValue()
    : buf(new char[FieldValue::bufSize])
    , state(State::Null)
{
}

FieldValue::FieldValue(int integer)
    : buf(new char[FieldValue::bufSize])
    , state(State::Integer)
{
    *reinterpret_cast<int*>(this->buf.get()) = integer;
}

FieldValue::FieldValue(double real)
    : buf(new char[FieldValue::bufSize])
    , state(State::Real)
{
    *reinterpret_cast<double*>(this->buf.get()) = real;
}

FieldValue::FieldValue(QString string)
    : buf(new char[FieldValue::bufSize])
    , state(State::String)
{
    new (this->buf.get()) QString(std::move(string));
}

FieldValue::FieldValue(Reference reference)
    : buf(new char[FieldValue::bufSize])
    , state(State::Reference)
{
    *reinterpret_cast<Reference*>(this->buf.get()) = reference;
}

FieldValue::FieldValue(List list)
    : buf(new char[FieldValue::bufSize])
    , state(State::List)
{
    new (this->buf.get()) List(std::move(list));
}

FieldValue::FieldValue(Map map)
    : buf(new char[FieldValue::bufSize])
    , state(State::Map)
{
    new (this->buf.get()) Map(std::move(map));
}

FieldValue::FieldValue(ExternalValue externalValue)
    : buf(new char[FieldValue::bufSize])
    , state(State::External)
{
    new (this->buf.get()) ExternalValue(std::move(externalValue));
}

FieldValue::~FieldValue()
{
    this->destroy();
}

FieldValue::FieldValue(const FieldValue& other)
    : buf(new char[FieldValue::bufSize])
    , state(other.state)
{
    switch (other.state)
    {
        case State::Null:
            break;

        case State::Integer:
        case State::Real:
        case State::Reference:
            memcpy(this->buf.get(), other.buf.get(), bufSize);
            break;

        case State::String:
            new (this->buf.get()) QString(*reinterpret_cast<QString*>(other.buf.get()));
            break;

        case State::List:
            new (this->buf.get()) List(*reinterpret_cast<List*>(other.buf.get()));
            break;

        case State::Map:
            new (this->buf.get()) Map(*reinterpret_cast<Map*>(other.buf.get()));
            break;

        case State::External:
            new (this->buf.get()) ExternalValue(*reinterpret_cast<ExternalValue*>(other.buf.get()));
            break;
    }
}

FieldValue& FieldValue::operator=(const FieldValue& other)
{
    const auto sameState = this->state == other.state;
    if (sameState)
        this->destroy();

    this->state = other.state;

    switch (this->state)
    {
        case State::Null:
            break;

        case State::Integer:
        case State::Real:
        case State::Reference:
            memcpy(this->buf.get(), other.buf.get(), bufSize);
            break;

        case State::String:
            if (sameState)
                *reinterpret_cast<QString*>(this->buf.get()) = *reinterpret_cast<QString*>(other.buf.get());
            else
                new (this->buf.get()) QString(*reinterpret_cast<QString*>(other.buf.get()));
            break;

        case State::List:
            if (sameState)
                *reinterpret_cast<List*>(this->buf.get()) = *reinterpret_cast<List*>(other.buf.get());
            else
                new (this->buf.get()) List(*reinterpret_cast<List*>(other.buf.get()));
            break;

        case State::Map:
            if (sameState)
                *reinterpret_cast<Map*>(this->buf.get()) = *reinterpret_cast<Map*>(other.buf.get());
            else
                new (this->buf.get()) Map(*reinterpret_cast<Map*>(other.buf.get()));
            break;

        case State::External:
            *reinterpret_cast<ExternalValue*>(this->buf.get()) = *reinterpret_cast<ExternalValue*>(other.buf.get());
            break;
    }

    return *this;
}

FieldValue::FieldValue(FieldValue&& other)
    : buf(std::move(other.buf))
    , state(other.state)
{
    other.state = State::Null;
}

FieldValue& FieldValue::operator=(FieldValue&& other)
{
    this->destroy();

    this->buf = std::move(other.buf);
    this->state = other.state;

    other.state = State::Null;

    return *this;
}

FieldValue& FieldValue::operator=(int integer)
{
    if (this->state == State::Integer)
        this->destroy();

    this->state = State::Integer;
    *reinterpret_cast<int*>(this->buf.get()) = integer;

    return *this;
}

FieldValue& FieldValue::operator=(double real)
{
    if (this->state == State::Real)
        this->destroy();

    this->state = State::Real;
    *reinterpret_cast<double*>(this->buf.get()) = real;

    return *this;
}

FieldValue& FieldValue::operator=(QString string)
{
    if (this->state == State::String)
    {
        *reinterpret_cast<QString*>(this->buf.get()) = std::move(string);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) QString(std::move(string));
    }

    this->state = State::String;

    return *this;
}

FieldValue& FieldValue::operator=(Reference reference)
{
    if (this->state != State::Reference)
        this->destroy();

    *reinterpret_cast<Reference*>(this->buf.get()) = reference;
    this->state = State::Reference;

    return *this;
}

FieldValue& FieldValue::operator=(List list)
{
    if (this->state == State::List)
    {
        *reinterpret_cast<List*>(this->buf.get()) = std::move(list);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) List(std::move(list));
    }

    this->state = State::List;

    return *this;
}

FieldValue& FieldValue::operator=(Map map)
{
    if (this->state == State::Map)
    {
        *reinterpret_cast<Map*>(this->buf.get()) = std::move(map);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) Map(std::move(map));
    }

    this->state = State::Map;

    return *this;
}

FieldValue& FieldValue::operator=(ExternalValue externalValue)
{
    if (this->state == State::External)
    {
        *reinterpret_cast<ExternalValue*>(this->buf.get()) = std::move(externalValue);
    }
    else
    {
        this->destroy();
        new (this->buf.get()) ExternalValue(std::move(externalValue));
    }

    this->state = State::External;

    return *this;
}

int& FieldValue::asInteger()
{
    if (this->state == State::Integer)
        return *reinterpret_cast<int*>(this->buf.get());

    throw utils::ValueError("Underlying value is not integer");
}

double& FieldValue::asReal()
{
    if (this->state == State::Real)
        return *reinterpret_cast<double*>(this->buf.get());

    throw utils::ValueError("Underlying value is not real");
}

QString& FieldValue::asString()
{
    if (this->state == State::String)
        return *reinterpret_cast<QString*>(this->buf.get());

    throw utils::ValueError("Underlying value is not string");
}

FieldValue::Reference& FieldValue::asReference()
{
    if (this->state == State::Reference)
        return *reinterpret_cast<Reference*>(this->buf.get());

    throw utils::ValueError("Underlying value is not reference");
}

FieldValue::List& FieldValue::asList()
{
    if (this->state == State::List)
        return *reinterpret_cast<List*>(this->buf.get());

    throw utils::ValueError("Underlying value is not list");
}

FieldValue::Map& FieldValue::asMap()
{
    if (this->state == State::Map)
        return *reinterpret_cast<Map*>(this->buf.get());

    throw utils::ValueError("Underlying value is not map");
}

FieldValue::ExternalValue& FieldValue::asExternalValue()
{
    if (this->state == State::External)
        return *reinterpret_cast<ExternalValue*>(this->buf.get());

    throw utils::ValueError("Underlying value is not external value");
}

const int& FieldValue::asInteger() const
{
    if (this->state == State::Integer)
        return *reinterpret_cast<int*>(this->buf.get());

    throw utils::ValueError("Underlying value is not integer");
}

const double& FieldValue::asReal() const
{
    if (this->state == State::Real)
        return *reinterpret_cast<double*>(this->buf.get());

    throw utils::ValueError("Underlying value is not real");
}

const QString& FieldValue::asString() const
{
    if (this->state == State::String)
        return *reinterpret_cast<QString*>(this->buf.get());

    throw utils::ValueError("Underlying value is not string");
}

const FieldValue::Reference& FieldValue::asReference() const
{
    if (this->state == State::Reference)
        return *reinterpret_cast<Reference*>(this->buf.get());

    throw utils::ValueError("Underlying value is not reference");
}

const FieldValue::List& FieldValue::asList() const
{
    if (this->state == State::List)
        return *reinterpret_cast<List*>(this->buf.get());

    throw utils::ValueError("Underlying value is not list");
}
const FieldValue::Map& FieldValue::asMap() const
{
    if (this->state == State::Map)
        return *reinterpret_cast<Map*>(this->buf.get());

    throw utils::ValueError("Underlying value is not map");
}

const FieldValue::ExternalValue& FieldValue::asExternalValue() const
{
    if (this->state == State::External)
        return *reinterpret_cast<ExternalValue*>(this->buf.get());

    throw utils::ValueError("Underlying value is not external value");
}

QVariant FieldValue::toQVariant() const
{
    switch (this->state)
    {
        case State::Null:
        {
            return QVariant();
        }

        case State::Integer:
        {
            return QVariant(this->asInteger());
        }

        case State::Real:
        {
            return QVariant(this->asReal());
        }

        case State::Reference:
        {
            return QVariant::fromValue(this->asReference());
        }

        case State::String:
        {
            return QVariant(this->asString());
        }

        case State::List:
        {
            QVariantList qvlist;
            const auto& list = this->asList();

            for (auto& element : list)
            {
                qvlist.push_back(element.toQVariant());
            }

            return qvlist;
        }

        case State::Map:
        {
            QVariantMap qvmap;
            const auto& map = this->asMap();

            for (auto& element : map)
            {
                qvmap[element.first] = QVariant(element.second.toQVariant());
            }

            return qvmap;
        }

        case State::External:
        {
            return asExternalValue().materialize().toQVariant();
        }
    }

    throw utils::ValueError("Invalid state");
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

void FieldValue::set(ExternalValue externalValue)
{
    *this = std::move(externalValue);
}

int& FieldValue::makeInteger()
{
    if (this->state == State::Integer)
    {
        return *reinterpret_cast<int*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::Integer;
        return *(new (this->buf.get()) int);
    }
}

double& FieldValue::makeReal()
{
    if (this->state == State::Real)
    {
        return *reinterpret_cast<double*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::Real;
        return *(new (this->buf.get()) double);
    }
}

QString& FieldValue::makeString()
{
    if (this->state == State::String)
    {
        return *reinterpret_cast<QString*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::String;
        return *(new (this->buf.get()) QString);
    }
}

FieldValue::Reference& FieldValue::makeReference()
{
    if (this->state == State::Reference)
    {
        return *reinterpret_cast<Reference*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::Reference;
        return *(new (this->buf.get()) Reference);
    }
}

FieldValue::List& FieldValue::makeList()
{
    if (this->state == State::List)
    {
        return *reinterpret_cast<List*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::List;
        return *(new (this->buf.get()) List);
    }
}

FieldValue::Map& FieldValue::makeMap()
{
    if (this->state == State::Map)
    {
        return *reinterpret_cast<Map*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::Map;
        return *(new (this->buf.get()) Map);
    }
}

FieldValue::ExternalValue& FieldValue::makeExternalValue()
{
    if (this->state == State::External)
    {
        return *reinterpret_cast<ExternalValue*>(this->buf.get());
    }
    else
    {
        this->destroy();
        this->state = State::External;
        return *(new (this->buf.get()) ExternalValue);
    }
}

Field::Type FieldValue::getType() const
{
    switch (this->state)
    {
        case State::Integer:
            return Field::Type::Integer;
        case State::Real:
            return Field::Type::Real;
        case State::Reference:
            return Field::Type::Reference;
        case State::String:
            return Field::Type::String;
        case State::List:
            return Field::Type::List;
        case State::Map:
            return Field::Type::Map;
        default:
            return static_cast<Field::Type>(-1);
    }
}

void FieldValue::destroy()
{
    switch (this->state)
    {
        case State::Null:
            break;

        case State::Integer:
        case State::Real:
        case State::Reference:
            break;

        case State::String:
            reinterpret_cast<QString*>(this->buf.get())->~QString();
            break;

        case State::List:
            reinterpret_cast<List*>(this->buf.get())->~vector<FieldValue>();
            break;

        case State::Map:
            reinterpret_cast<Map*>(this->buf.get())->~map<QString, FieldValue>();
            break;

        case State::External:
            reinterpret_cast<ExternalValue*>(this->buf.get())->~ExternalValue();
            break;
    }

    this->state = State::Null;
}

bool operator==(const FieldValue& a, const FieldValue& b)
{
    if (a.getState() != b.getState())
        return false;

    switch (a.getState())
    {
        case FieldValue::State::Null:
            return true;
        case FieldValue::State::Integer:
            return a.asInteger() == b.asInteger();
        case FieldValue::State::Real:
            return a.asReal() == b.asReal();
        case FieldValue::State::String:
            return a.asString() == b.asString();
        case FieldValue::State::Reference:
            return a.asReference() == b.asReference();
        case FieldValue::State::List:
            return a.asList() == b.asList();
        case FieldValue::State::Map:
            return a.asMap() == b.asMap();
        case FieldValue::State::External:
            return a.asExternalValue() == b.asExternalValue();
    }

    return false;
}

bool operator<(const FieldValue& a, const FieldValue& b)
{
    if (a.getState() != b.getState())
        throw utils::ValueError("Cannot compare fields of different states");

    switch (a.getState())
    {
        case FieldValue::State::Null:
            return false;
        case FieldValue::State::Integer:
            return a.asInteger() < b.asInteger();
        case FieldValue::State::Real:
            return a.asReal() < b.asReal();
        case FieldValue::State::String:
            return a.asString() < b.asString();
        case FieldValue::State::Reference:
            return a.asReference() < b.asReference();
        case FieldValue::State::List:
            return a.asList() < b.asList();
        case FieldValue::State::Map:
            return a.asMap() < b.asMap();
        case FieldValue::State::External:
            return false;
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, Field::Type t)
{
    const QMetaEnum metaEnum{QMetaEnum::fromType<Field::Type>()};
    auto str = metaEnum.valueToKey(static_cast<int>(t));
    if (!str)
        throw utils::ValueError("Invalid value: " + QString::number(static_cast<int>(t)) + " for enum Field::Type");
    os << str;
    return os;
}

std::ostream& operator<<(std::ostream& os, FieldValue::State s)
{
    switch (s)
    {
        case FieldValue::State::Null:
            os << "Null";
            break;
        case FieldValue::State::Integer:
            os << "Integer";
            break;
        case FieldValue::State::Real:
            os << "Real";
            break;
        case FieldValue::State::String:
            os << "String";
            break;
        case FieldValue::State::Reference:
            os << "Reference";
            break;
        case FieldValue::State::List:
            os << "List";
            break;
        case FieldValue::State::Map:
            os << "Map";
            break;
        case FieldValue::State::External:
            os << "External";
            break;
    }
    return os;
}

} // namespace core
} // namespace warmonger
