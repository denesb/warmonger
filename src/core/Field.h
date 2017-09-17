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
    Q_PROPERTY(FieldType* type READ getType NOTIFY typeChanged)

public:
    /**
     * The type id of a field-type.
     *
     * This enum contains the supported types for component fields.
     */
    enum class TypeId
    {
        Integer,
        Real,
        String,
        Reference,
        List,
        Map
    };
    Q_ENUM(TypeId)

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
    Field(const QString& name, std::unique_ptr<FieldType>&& type, QObject* parent = nullptr);

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
    FieldType* getType() const
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
    void setType(std::unique_ptr<FieldType>&& type);

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
    FieldType* type;
};

/**
 * FieldType interface.
 */
class FieldType : public QObject
{
public:
    using QObject::QObject;

    virtual Field::TypeId id() const = 0;
};

namespace FieldTypes {

/**
 * Integral numeric type.
 *
 * Corresponds to int.
 */
class Integer : public FieldType
{
public:
    using FieldType::FieldType;

    Field::TypeId id() const override
    {
        return Field::TypeId::Integer;
    }
};

/**
 * Real numeric type.
 *
 * Corresponds to double.
 */
class Real : public FieldType
{
public:
    using FieldType::FieldType;

    Field::TypeId id() const override
    {
        return Field::TypeId::Real;
    }
};

/**
 * String.
 *
 * Corresponds to QString.
 */
class String : public FieldType
{
public:
    using FieldType::FieldType;

    Field::TypeId id() const override
    {
        return Field::TypeId::String;
    }
};

/**
 * Reference to a FieldType
 *
 * Corresponds to a FieldType*.
 */
class Reference : public FieldType
{
public:
    using FieldType::FieldType;

    Field::TypeId id() const override
    {
        return Field::TypeId::Reference;
    }
};

/**
 * List of elements.
 *
 * Corresponds to a QVariantList
 */
class List : public FieldType
{
public:
    List(std::unique_ptr<FieldType>&& valueType);

    Field::TypeId id() const override
    {
        return Field::TypeId::List;
    }

    FieldType* getValueType() const
    {
        return valueType;
    }

private:
    FieldType* valueType;
};

/**
 * Associative container mapping string keys to values.
 *
 * Corresponds to a QVariantMap
 */
class Map : public FieldType
{
public:
    Map(std::unique_ptr<FieldType>&& valueType);

    Field::TypeId id() const override
    {
        return Field::TypeId::Map;
    }

    FieldType* getValueType() const
    {
        return valueType;
    }

private:
    FieldType* valueType;
};

} // namspace FieldTypes

class WObject;

/**
 * Type-erased union-like holder of a field value.
 *
 * Field types from Field::TypeId are mapped to C++ types as follows:
 *
 * Field::TypeId            | C++ type
 * ------------------------ | -----------------------------
 * Field::TypeId::Integer   | int
 * Field::TypeId::Real      | double
 * Field::TypeId::String    | QString
 * Field::TypeId::Reference | WObject*
 * Field::TypeId::List      | std::vector<FieldValue>
 * Field::TypeId::Map       | std::map<QString, FieldValue>
 *
 * Constructor overloads and assignment operator overloads are provided for
 * each supported type to conveniently construct or assign any supported
 * values. For non-trivial types move constructors and move assignment
 * operators are also provided.
 * To access the stored value use the _as_ (asInteger(), asReal(), etc.)
 * methods, they have both modifiable and const versions.
 * A default constructed FieldValue will have a null value, isNull() will
 * return _true_. Null fields don't have a type yet, so calling getTypeId()
 * will return a random value.
 * To prepare the storage of a certain value use the _make_ (makeInteger(),
 * makeReal(), etc.) methods. These will prepare a default-constructed value
 * with the provided type and return a modifiable pointer to it.
 */
class FieldValue
{
public:
    using Reference = WObject*;
    using List = std::vector<FieldValue>;
    using Map = std::map<QString, FieldValue>;

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
    /** @} */

    ~FieldValue();

    FieldValue(const FieldValue& other);
    FieldValue& operator=(const FieldValue& other);

    FieldValue(FieldValue&& other);
    FieldValue& operator=(FieldValue&& other);

    FieldValue& operator=(int integer);
    FieldValue& operator=(double real);
    FieldValue& operator=(const QString& string);
    FieldValue& operator=(QString&& string);
    FieldValue& operator=(Reference reference);
    FieldValue& operator=(const List& list);
    FieldValue& operator=(List&& list);
    FieldValue& operator=(const Map& map);
    FieldValue& operator=(Map&& map);

    /**
     * @name Accessors.
     *
     * Will return a reference to the underlying value or will
     * throw utils::ValueError if the value is not of ther requested
     * type.
     * @{
     */
    int& asInteger();
    double& asReal();
    QString& asString();
    Reference& asReference();
    List& asList();
    Map& asMap();
    /** @} */

    /**
     * @name Const accessors.
     *
     * Will return a const reference to the underlying value or will
     * throw utils::ValueError if the value is not of ther requested
     * type.
     * @{
     */
    const int& asInteger() const;
    const double& asReal() const;
    const QString& asString() const;
    const Reference& asReference() const;
    const List& asList() const;
    const Map& asMap() const;
    /** @} */

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
    /** @} */

    Field::TypeId getTypeId() const
    {
        return this->typeId;
    }

    bool isNull() const
    {
        return this->null;
    }

    /**
     * @name Type query methods.
     * @{
     */

    bool isInteger() const
    {
        return !this->null && this->typeId == Field::TypeId::Integer;
    }

    bool isReal() const
    {
        return !this->null && this->typeId == Field::TypeId::Real;
    }

    bool isString() const
    {
        return !this->null && this->typeId == Field::TypeId::String;
    }

    bool isReference() const
    {
        return !this->null && this->typeId == Field::TypeId::Reference;
    }

    bool isList() const
    {
        return !this->null && this->typeId == Field::TypeId::List;
    }

    bool isMap() const
    {
        return !this->null && this->typeId == Field::TypeId::Map;
    }

    /** @} */

    /**
     * Equivalent to FieldValue::isNull().
     */
    bool operator!() const
    {
        return this->null;
    }

private:
    void destroy();

    static const std::size_t bufSize;

    std::unique_ptr<char[]> buf;
    Field::TypeId typeId;
    bool null;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H