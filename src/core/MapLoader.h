#ifndef CORE_MAP_LOADER_H
#define CORE_MAP_LOADER_H

#include <QList>
#include <QMap>
#include <QStringList>
#include <QString>

namespace warmonger {
namespace core {

class Map;

class MapLoader :
    public QObject
{
    Q_OBJECT

public:
    MapLoader(QObject *parent);
    ~MapLoader();

    QStringList getSearchPath() const;
    void setSearchPath(const QStringList &searchPath);

    QList<Map *> getMapList() const;
    Map * loadMap(const QString &mapName);

signals:
    void searchPathChanged(const QStringList &searchPath);
    void mapListChanged(const QList<Map *> &mapList);

public slots:

private:
    void scanSearchPath();
    void loadMapList();

    QStringList searchPath;
    QMap<QString, QString> pathToMapMap;
    QList<Map*> mapList;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_LOADER_H
