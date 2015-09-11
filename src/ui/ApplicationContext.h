#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>

#include "core/Game.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList maps READ readMaps NOTIFY mapsChanged)

public:
    ApplicationContext(QObject *parent);
    ~ApplicationContext();

    void loadMaps();

    QVariantList readMaps() const;

    Q_INVOKABLE QObject * newGame(QObject *map);

signals:
    void mapsChanged();

public slots:
    void setCurrentMap(QObject *map);

private:
    QList<core::Map *> maps;
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
