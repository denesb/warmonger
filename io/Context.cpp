#include <algorithm>

#include "core/World.h"
#include "io/Context.h"

namespace warmonger {
namespace io {

Context::Context()
    : injectFn()
    , objectsByType()
{
}

Context::Context(std::function<void(const QString&, const QString&, Context&)> injectFn)
    : injectFn(injectFn)
    , objectsByType()
{
}

void Context::add(QObject* object)
{
    const QMetaObject* metaObject = object->metaObject();
    const QString className = metaObject->className();

    wDebug << "Added `" << className << "' object `" << object << "'";

    QMap<QString, QObject*>& objects = this->objectsByType[className];
    objects[object->objectName()] = object;
}

QObject* Context::getObject(const QString& className, const QString& objectName) const
{
    const QMap<QString, QObject*> objects = this->objectsByType[className];

    QObject* object{nullptr};
    if (objects.contains(objectName))
    {
        object = objects[objectName];
    }
    else
    {
        wWarning << "Object `" << className << "' with objectName `" << objectName << "' not found";
    }

    return object;
}

void addWorldToContext(Context& ctx, core::World* world)
{
    ctx.add(world);

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

    const std::vector<core::Banner*> bs = world->getBanners();
    std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

    const std::vector<core::Civilization*> cs = world->getCivilizations();
    std::for_each(cs.cbegin(), cs.cend(), [&](core::Civilization* o) { ctx.add(o); });

    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });
}

} // namespace io
} // namespace warmonger
