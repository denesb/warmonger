include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Armor.h \
    DamageType.h \
    Exception.h \
    Faction.h \
    Game.h \
    Map.h \
    MapNode.h \
    Player.h \
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
    Exception.cpp \
    Faction.cpp \
    Game.cpp \
    Map.cpp \
    MapNode.cpp \
    Player.cpp \
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
