SRC = $$PWD

INCLUDEPATH += $$SRC

HEADERS += \
    $$SRC/Warmonger.h \
    $$SRC/core/JsonUtil.hpp \
    $$SRC/core/WorldMeta.h \
    $$SRC/core/WorldItem.h \
    $$SRC/core/World.h \
    $$SRC/core/WorldLoader.h \
    $$SRC/core/TerrainType.h \
    $$SRC/core/UnitClass.h \
    $$SRC/core/DamageType.h \
    $$SRC/core/Weapon.h \
    $$SRC/core/Armor.h \
    $$SRC/core/UnitType.h \
    $$SRC/core/Faction.h \
    $$SRC/ui/model/WorldList.h

SOURCES += \
    $$SRC/main.cpp \
    $$SRC/Warmonger.cpp \
    $$SRC/core/WorldItem.cpp \
    $$SRC/core/WorldMeta.cpp \
    $$SRC/core/World.cpp \
    $$SRC/core/WorldLoader.cpp \
    $$SRC/core/TerrainType.cpp \
    $$SRC/core/UnitClass.cpp \
    $$SRC/core/DamageType.cpp \
    $$SRC/core/Weapon.cpp \
    $$SRC/core/Armor.cpp \
    $$SRC/core/UnitType.cpp \
    $$SRC/core/Faction.cpp \
    $$SRC/ui/model/WorldList.cpp

# Add more folders to ship with the application, here
qml.source = ui/qml
qml.target =

worlds.source = ../worlds
worlds.target = 

DEPLOYMENTFOLDERS = qml worlds

# Please do not modify the following two lines. Required for deployment.
include($$SRC/ui/qtquick2applicationviewer.pri)
qtcAddDeployment()
