#ifndef W_CORE_ARMY_H
#define W_CORE_ARMY_H

#include <vector>

#include "core/ArmyType.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Unit.h"

namespace warmonger {
namespace core {

/**
 * An army or a garrison.
 *
 * An army is a collection of units. The number of units in an army depends
 * current world. An army has a type, which allows for (among other) different
 * appearance or behavior for armies of different civilizations.
 *
 * \see warmonger::core::ArmyType
 * \see warmonger::core::Unit
 * \see warmonger::core::World
 */
class Army : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(ArmyType* type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode* mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction* owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QVariantList units READ readUnits NOTIFY unitsChanged)

public:
    /**
     * Constructs an empty Army object.
     *
     *\param parent the parent QObject.
     */
    explicit Army(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     *\returns the displayName
     */
    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    /**
     * Set the display-name.
     *
     * Will emit the signal Army::displayNameChanged() if the newly set value is
     * different than the current one.
     *
     * \param displayName the new displayName
     */
    void setDisplayName(const QString& displayName);

    /**
     * Get the army-type.
     *
     * \returns the army-type
     */
    ArmyType* getType() const
    {
        return this->type;
    }

    /**
     * Set the army-type.
     *
     * Will emit the signal Army::typeChanged() if the newly set value is
     * different than the current one.
     *
     * \param type the new army-type
     */
    void setType(ArmyType* type);

    /**
     * Get the map-node.
     *
     * \returns the map-node
     */
    MapNode* getMapNode() const
    {
        return this->mapNode;
    }

    /**
     * Set the map-node.
     *
     * Will emit the signal Army::mapNodeChanged() if the newly
     * set value is different than the current one.
     *
     * \param mapNode the new map-node
     */
    void setMapNode(MapNode* mapNode);

    /**
     * Get the owner.
     *
     * \return the owner
     */
    Faction* getOwner() const
    {
        return this->owner;
    }

    /**
     * Set the owner.
     *
     * Will emit the signal Army::ownerChanged() if the newly
     * set value is different than the current one.
     *
     * \param owner the new owner
     */
    void setOwner(Faction* owner);

    /**
     * Get the units.
     *
     * \returns the units
     */
    const std::vector<Unit*>& getUnits() const
    {
        return this->units;
    }

    /**
     * Set the units.
     *
     * Will emit the signal Army::unitsChanged() if the newly set value is
     * different than the current one.
     *
     * \param units the new units
     */
    void setUnits(const std::vector<Unit*>& units);

    /**
     * Get the units as a QVariantList.
     *
     * This function is used as a read function for the units property and is
     * not supposed to be called from C++ code. Use Army::getUnits() instead.
     *
     * \returns the units
     */
    QVariantList readUnits() const;

signals:
    /**
     * Emitted when the display-name changes.
     */
    void displayNameChanged();

    /**
     * Emitted when the army-type changes.
     */
    void typeChanged();

    /**
     * Emitted when the map-node changes.
     */
    void mapNodeChanged();

    /**
     * Emitted when the owner changes.
     */
    void ownerChanged();

    /**
     * Emitted when the units change.
     */
    void unitsChanged();

private:
    QString displayName;
    ArmyType* type;
    MapNode* mapNode;
    Faction* owner;
    std::vector<Unit*> units;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ARMY_H
