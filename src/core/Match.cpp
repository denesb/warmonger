#include "core/Match.h"
#include "core/World.h"
#include "core/Player."

using namespace core;

Match::Match(const World *world, Map *map, QObject *parent) :
	QObject(parent),
	world(world),
	map(map)
{
}

Match::Match(const QJsonObject &json, QObject *parent) :
	QObject(parent)
{
}
