/** \file
 * Field.
 *
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
#ifndef W_CORE_FIELD_H
#define W_CORE_FIELD_H

#include <memory>

#include <QObject>
#include <QString>
#include <QVariant>

namespace warmonger {
namespace core {

class FieldType;

/**
 * Component-type field.
 *
 * Contains the name and type of a field.
 */
class Field : public QObject
{
    Q_OBJECT

    /**
     * The name of the field.
     */
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)

    /**
     * The type of the field.
     */
    Q_PROPERTY(Field::Type type READ getType NOTIFY typeChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<QObject>()
            .visitMember("name", &Field::getName, &Field::setName)
            .visitMember("type", &Field::getType, &Field::setType)
            .template visitConstructor<QObject*>("parent");
    }

    /**
     * The type id of a field-type.
     *
     * This enum contains the supported types for component fields.
     * TODO: Add boolean type
     */
    enum class Type
    {
        Integer,
        Real,
        String,
        Reference,
        List,
        Map
    };
    Q_ENUM(Type)

    /**
     * Constructs an empty Field.
     *
     * \param parent the parent QObject.
     */
    Field(QObject* parent = nullptr);

    /**
     * Constructs a Field.
     *
     * \param parent the parent QObject.
     */
    Field(const QString& name, Type type, QObject* parent = nullptr);

    /**
     * Get the name.
     *
     * \returns the name
     */
    QString getName() const
    {
        return this->name;
    }

    /**
     * Set the name.
     *
     * Will emit Field::nameChanged() if the new value is different than the
     * current one.
     *
     * \param name the name
     */
    void setName(const QString& name);

    /**
     * Get the type.
     *
     * \returns the type
     */
    Type getType() const
    {
        return this->type;
    }

    /**
     * Set the type.
     *
     * Will emit Field::typeChanged() if the new value is different than the
     * current one.
     * The Field takes ownership of the type.
     *
     * \param name the name
     */
    void setType(Type type);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the type changes.
     */
    void typeChanged();

private:
    QString name;
    Type type;
};

class WObject;
class ExternalValue;

/**
 * Type-erased union-like holder of a field value.
 *
 * FieldType has the following distinct states:
 *
 * State     | State enum       | Will return true
 * -----------------------------------------------
 * null      | State::Null      | isNull()
 * external  | State::External  | isExternal()
 * integer   | State::Integer   | isInteger()
 * real      | State::Real      | isReal()
 * string    | State::String    | isString()
 * reference | State::Reference | isReference()
 * list      | State::List      | isList()
 * map       | State::Map       | isMap()
 *
 * Constructor overloads and assignment operator overloads are provided for
 * each supported state to conveniently construct or assign any supported
 * values. For non-trivial types move constructors and move assignment
 * operators are also provided.
 * To access the stored value use the _as_ (asInteger(), asReal(), etc.)
 * methods, they have both modifiable and const versions. Before calling an
 * _as_ method make sure the FieldValue is in the correct state. E.g. for
 * the call to asInteger() to succeed FieldValue has to be in the Integer
 * state, i.e. it has to hold an integer value.
 * A default constructed FieldValue will be in the Null state, it won't
 * contain any value. Null fields don't have a type yet, so calling getType()
 * will return a random value.
 * To prepare the storage of a certain value use the _make_ (makeInteger(),
 * makeReal(), etc.) methods. These will prepare a default-constructed value
 * with the provided type and return a modifiable pointer to it.
 * Assigning to a FieldValue is *always* a valid operation. The new value
 * will override the old one.
 *
 * Field types from Field::Type are mapped to C++ types as follows:
 *
 * Field::Type            | C++ type
 * -----------------------| -----------------------------
 * Field::Type::Integer   | int
 * Field::Type::Real      | double
 * Field::Type::String    | QString
 * Field::Type::Reference | WObject*
 * Field::Type::List      | std::vector<FieldValue>
 * Field::Type::Map       | std::map<QString, FieldValue>
 *
 */
class FieldValue
{
public:
    enum class State : int8_t
    {
        Null,
        External,
        Integer,
        Real,
        String,
        Reference,
        List,
        Map,
    };
    using Reference = WObject*;
    using List = std::vector<FieldValue>;
    using Map = std::map<QString, FieldValue>;

    /**
     * A value that resides in an external buffer.
     *
     * Can be materialized into the FieldValue's internal buffer on-demand.
     * This can be an expensive operations as it might involve copying possibly
     * deeply nested data structures.
     * Typical examples are containers and objects owned and stored inside
     * script-engines.
     * The primary user of this interface will be world-rules implementations,
     * script bindinds. ExternalValue makes it possible for them to keep values
     * mostly used by the rules in their native environments, only copying them
     * across when necessary.
     */
    class ExternalValue
    {
    public:
        struct Impl
        {
            virtual FieldValue materialize() const = 0;
        };

        ExternalValue() = default;

        ExternalValue(std::shared_ptr<Impl> impl)
            : impl(impl)
        {
        }

        /**
         * Materialize the value into a FieldValue.
         *
         * This is a possibly expensive operation as it can involve copying a
         * possibly deeply nested data-structure and unserializing it into
         * FieldValues.
         *
         * \returns The current snapshot of the value, unserialized into
         *      a FieldValue
         */
        FieldValue materialize() const
        {
            return this->impl->materialize();
        }

        Impl* getImpl()
        {
            return this->impl.get();
        }

    private:
        std::shared_ptr<Impl> impl;
    };

    template <typename T, typename... Args>
    static FieldValue makeExternal(Args&&... args)
    {
        return FieldValue(ExternalValue(std::make_shared<T>(std::forward<Args>(args)...)));
    }

    /**
     * Construct a null field value.
     */
    FieldValue();

    /**
     * @name Construct directly from a value.
     *
     * @{
     */
    FieldValue(int integer);
    FieldValue(double real);
    FieldValue(QString string);
    FieldValue(Reference reference);
    FieldValue(List list);
    FieldValue(Map map);
    FieldValue(ExternalValue externalValue);
    /** @} */

    ~FieldValue();

    FieldValue(const FieldValue& other);
    FieldValue& operator=(const FieldValue& other);

    FieldValue(FieldValue&& other);
    FieldValue& operator=(FieldValue&& other);

    FieldValue& operator=(int integer);
    FieldValue& operator=(double real);
    FieldValue& operator=(QString string);
    FieldValue& operator=(Reference reference);
    FieldValue& operator=(List list);
    FieldValue& operator=(Map map);
    FieldValue& operator=(ExternalValue externalValue);

    /**
     * @name Accessors.
     *
     * Will return a reference to the underlying value or will
     * throw utils::ValueError if the value is not of the requested
     * type.
     * @{
     */
    int& asInteger();
    double& asReal();
    QString& asString();
    Reference& asReference();
    List& asList();
    Map& asMap();
    ExternalValue& asExternalValue();
    /** @} */

    /**
     * @name Const accessors.
     *
     * Will return a const reference to the underlying value or will
     * throw utils::ValueError if the value is not of the requested
     * type.
     * @{
     */
    const int& asInteger() const;
    const double& asReal() const;
    const QString& asString() const;
    const Reference& asReference() const;
    const List& asList() const;
    const Map& asMap() const;
    const ExternalValue& asExternalValue() const;
    /** @} */

    /**
     * Serialize the FieldValue into a QVariant.
     *
     * If the FieldValue holds an external value, it will be
     * materialized and the resulting value will be serialized into
     * a QVariant.
     */
    QVariant toQVariant() const;

    /**
     * @name Set the underlying value.
     *
     * Will change the type of the FieldValue to the type of the value
     * set.
     *
     * @{
     */
    void set(int integer);
    void set(double real);
    void set(QString string);
    void set(Reference reference);
    void set(List list);
    void set(Map map);
    void set(ExternalValue externalValue);
    /** @} */

    /**
     * @name Change the underlying type.
     *
     * A reference is returned to the default initialized value.
     * Note that this will erase any previously stored value.
     * @{
     */
    int& makeInteger();
    double& makeReal();
    QString& makeString();
    WObject*& makeReference();
    List& makeList();
    Map& makeMap();
    ExternalValue& makeExternalValue();
    /** @} */

    /**
     * Get the state of the FieldValue;
     */
    State getState() const
    {
        return this->state;
    }

    /**
     * Get the type of the stored value.
     *
     * Only returns meaningful value if the FieldValue is not null or
     * external.
     */
    Field::Type getType() const;

    /**
     * @name State query methods.
     * @{
     */

    bool isNull() const
    {
        return this->state == State::Null;
    }

    bool isExternal() const
    {
        return this->state == State::External;
    }

    bool isInteger() const
    {
        return this->state == State::Integer;
    }

    bool isReal() const
    {
        return this->state == State::Real;
    }

    bool isString() const
    {
        return this->state == State::String;
    }

    bool isReference() const
    {
        return this->state == State::Reference;
    }

    bool isList() const
    {
        return this->state == State::List;
    }

    bool isMap() const
    {
        return this->state == State::Map;
    }

    /** @} */

    /**
     * Equivalent to FieldValue::isNull().
     */
    bool operator!() const
    {
        return this->state == State::Null;
    }

    operator int() const
    {
        return this->asInteger();
    }

    operator double() const
    {
        return this->asReal();
    }

    operator const QString&() const
    {
        return this->asString();
    }

    operator Reference() const
    {
        return this->asReference();
    }

    operator const List&() const
    {
        return this->asList();
    }

    operator const Map&() const
    {
        return this->asMap();
    }

    operator const ExternalValue&() const
    {
        return this->asExternalValue();
    }

private:
    void destroy();

    static const std::size_t bufSize;

    std::unique_ptr<char[]> buf;
    State state = State::Null;
};

bool operator==(const FieldValue& a, const FieldValue& b);

/**
 * Less compare FieldValues
 *
 * Not defined for Null and External states and for state mismatch.
 */
bool operator<(const FieldValue& a, const FieldValue& b);

std::ostream& operator<<(std::ostream& os, Field::Type t);
std::ostream& operator<<(std::ostream& os, FieldValue::State s);

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
