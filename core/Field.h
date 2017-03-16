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
        Dictionary
    };
    Q_ENUM(TypeId)

    /**
     * Constructs an empty CampaignMap.
     *
     * \param parent the parent QObject.
     */
    Field(QObject* parent = nullptr);

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
class Dictionary : public FieldType
{
public:
    Dictionary(std::unique_ptr<FieldType>&& valueType);

    Field::TypeId id() const override
    {
        return Field::TypeId::Dictionary;
    }

    FieldType* getValueType() const
    {
        return valueType;
    }

private:
    FieldType* valueType;
};

} // namspace FieldTypes

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
