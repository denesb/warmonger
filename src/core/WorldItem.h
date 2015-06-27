#ifndef CORE_WORLD_ELEMENT_H
#define CORE_WORLD_ELEMENT_H

#include <QString>
#include <QObject>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

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

	virtual WorldItem * getWorldItem(const QString &className, const QString &objectName) const = 0;

	virtual void fromJson(const QJsonObject &obj);
	virtual QJsonObject toJson() const;

	template<class P>
	QList<P *> propertyListFromJson(const QJsonArray &array)
	{
		QList<P *> list;

		for (const QJsonValue item : array)
		{
			P *property = new P(this);
			property->fromJson(item.toObject());
			list.append(property);
		}

		return std::move(list);
	}
	
private:
	QString displayName;
};

}; // namespace core

#endif // CORE_WORLD_ELEMENT_H
