#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include <QColor>
#include <QObject>
#include <QString>

#include "core/Banner.h"
#include "core/Civilization.h"

namespace warmonger {
namespace core {

class Faction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QColor primaryColor READ getPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor secondaryColor READ getPrimaryColor NOTIFY secondaryColorChanged)
    Q_PROPERTY(Civilization* civilization READ getCivilization NOTIFY civilizationChanged)

public:
    explicit Faction(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    const QColor& getPrimaryColor() const
    {
        return this->primaryColor;
    }

    void setPrimaryColor(const QColor& primaryColor);

    const QColor& getSecondaryColor() const
    {
        return this->secondaryColor;
    }

    void setSecondaryColor(const QColor& secondaryColor);

    Banner* getBanner() const
    {
        return this->banner;
    }

    void setBanner(Banner* banner);

    Civilization* getCivilization() const
    {
        return this->civilization;
    }

    void setCivilization(Civilization* civilization);

signals:
    void displayNameChanged();
    void primaryColorChanged();
    void secondaryColorChanged();
    void bannerChanged();
    void civilizationChanged();

private:
    QString displayName;
    QColor primaryColor;
    QColor secondaryColor;
    Banner* banner;
    Civilization* civilization;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
