#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>

#include "core/Map.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT

public:
    ApplicationContext(QObject *parent);
    ~ApplicationContext();

    Q_INVOKABLE QVariantList mapList();

    Q_INVOKABLE QObject * openMap(QString mapName);
    Q_INVOKABLE void closeMap(QObject *map);

private:
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
