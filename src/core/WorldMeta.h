#include <QObject>
#include <QString>
#include <QJsonObject>

#ifndef CORE_WORLD_META_H
#define CORE_WORLD_META_H

namespace core {

class WorldMeta :
    public QObject
{
    Q_OBJECT

public:
    WorldMeta(
		const QString &name,
		const QString &displayName,
		const QString &description,
		const QString &path,
		QObject *parent = nullptr
	);
	WorldMeta(const QJsonObject &obj, QObject *parent = nullptr);

    QString getName() const;
    QString getDisplayName() const;
    QString getDescription() const;
    QString getPath() const;

	QJsonObject toJson() const;
private:
    QString name;
    QString displayName;
    QString description;
    QString path;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
