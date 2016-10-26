#ifndef W_CORE_BANNER_H
#define W_CORE_BANNER_H

#include <vector>

#include <QObject>

#include "core/Civilization.h"

namespace warmonger {
namespace core {

/**
 * A faction banner
 */
class Banner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)

public:
    explicit Banner(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    /**
     * Get the civilizations for this banner
     *
     * Banners can be restricted to certain civilizations. If the list is empty
     * the banner is not restricted to any civilizations.
     *
     * @returns const std::vector<Civilizations*>& the civilizations
     */
    const std::vector<Civilization*>& getCivilizations() const
    {
        return this->civilizations;
    }

    QVariantList readCivilizations() const;

    void setCivilizations(const std::vector<Civilization*>& civilizations);

signals:
    void displayNameChanged();
    void civilizationsChanged();

private:
    QString displayName;
    std::vector<Civilization*> civilizations;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_BANNER_H
