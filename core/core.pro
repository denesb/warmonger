include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Armor.h \
    DamageType.h \
    Exception.h \
    EntityManager.h \
    Faction.h \
    Game.h \
    GameEntity.h \
    GameEntityPart.h \
    GameObject.h \
    Map.h \
    MapNode.h \
    Player.h \
    QJsonUtil.h \
    QVariantUtil.h \
    Settlement.h \
    SettlementType.h \
    TerrainType.h \
    Unit.h \
    UnitClass.h \
    UnitLevel.h \
    UnitType.h \
    Weapon.h \
    World.h \
    WorldSurface.h

SOURCES += \
    Armor.cpp \
    DamageType.cpp \
    EntityManager.cpp \
    Exception.cpp \
    Faction.cpp \
    Game.cpp \
    GameEntity.cpp \
    GameEntityPart.cpp \
    GameObject.cpp \
    Map.cpp \
    MapNode.cpp \
    Player.cpp \
    QJsonUtil.cpp \
    Settlement.cpp \
    SettlementType.cpp \
    TerrainType.cpp \
    Unit.cpp \
    UnitClass.cpp \
    UnitLevel.cpp \
    UnitType.cpp \
    Weapon.cpp \
    World.cpp \
    WorldSurface.cpp
