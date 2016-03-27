include(../common.pri)

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    Armor.h \
    Army.h \
    CampaignMap.h \
    Civilization.h \
    DamageType.h \
    Exception.h \
    Faction.h \
    Game.h \
    MapNode.h \
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
    Army.cpp \
    CampaignMap.cpp \
    Civilization.cpp \
    DamageType.cpp \
    Exception.cpp \
    Faction.cpp \
    Game.cpp \
    MapNode.cpp \
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
