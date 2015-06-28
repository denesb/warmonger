#ifndef CORE_WORLD_ELEMENT_H
#define CORE_WORLD_ELEMENT_H

#include <QString>
#include <QObject>
#include <QJsonObject>

namespace core {

class WorldItem :
	public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName)

public:
	WorldItem(QObject *parent = nullptr);
	virtual ~WorldItem();

	QString getDisplayName() const;
	void setDisplayName(const QString &displayName);

	bool isRootWorldItem();

	virtual void fromJson(const QJsonObject &obj);
	virtual QJsonObject toJson() const;
	
private:
	QString displayName;
};

}; // namespace core

#endif // CORE_WORLD_ELEMENT_H
