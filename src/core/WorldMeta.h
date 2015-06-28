#ifndef CORE_WORLD_META_H
#define CORE_WORLD_META_H

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace core {

class WorldMeta :
    public QObject
{
    Q_OBJECT

public:
	WorldMeta(QObject *parent = nullptr);

    QString getObjectName() const;

    QString getDisplayName() const;
	void setDisplayName(const QString &displayName);

	QString getDescription() const;
	void setDescription(const QString &description);

	static WorldMeta * newFromJson(const QJsonObject &obj, QObject *parent = nullptr);
	void fromJson(const QJsonObject &obj);
	QJsonObject toJson() const;
private:
    QString name;
    QString displayName;
    QString description;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
