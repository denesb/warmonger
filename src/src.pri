# Don't add any other folders!
INCLUDEPATH += src

#
# All file list's are separated into groups by their module.
# Please keep the lists sorted!
#

# log
HEADERS += \
    src/log/ConsoleHandler.h \
    src/log/Handler.h \
    src/log/Filter.h \
    src/log/Formatter.h \
    src/log/Logger.h \
    src/log/LogLevel.h \
    src/log/LogRecord.h \
    src/log/LogStream.h

# core
HEADERS += \
    src/core/Armor.h \
    src/core/DamageType.h \
    src/core/Exception.h \
    src/core/EntityManager.h \
    src/core/Faction.h \
    src/core/Game.h \
    src/core/GameEntity.h \
    src/core/GameEntityPart.h \
    src/core/GameObject.h \
    src/core/Map.h \
    src/core/MapNode.h \
    src/core/Player.h \
    src/core/QJsonUtil.h \
    src/core/QVariantUtil.h \
    src/core/Settlement.h \
    src/core/SettlementType.h \
    src/core/TerrainType.h \
    src/core/Unit.h \
    src/core/UnitClass.h \
    src/core/UnitLevel.h \
    src/core/UnitType.h \
    src/core/Weapon.h \
    src/core/World.h \
    src/core/WorldSurface.h

# ui
HEADERS += \
    src/ui/qtquick2applicationviewer.h \
    src/ui/ApplicationContext.h \
    src/ui/GameMap.h \
    src/ui/MiniMap.h \
    src/ui/MapDrawer.h \
    src/ui/MapPreview.h \
    src/ui/MapUtil.h \
    src/ui/UnitRecruit.h

# app
SOURCES += \
    src/Main.cpp

# log
SOURCES += \
    src/log/ConsoleHandler.cpp \
    src/log/Handler.cpp \
    src/log/Filter.cpp \
    src/log/Formatter.cpp \
    src/log/Logger.cpp \
    src/log/LogRecord.cpp \
    src/log/LogStream.cpp

# core
SOURCES += \
    src/core/Armor.cpp \
    src/core/DamageType.cpp \
    src/core/EntityManager.cpp \
    src/core/Exception.cpp \
    src/core/Faction.cpp \
    src/core/Game.cpp \
    src/core/GameEntity.cpp \
    src/core/GameEntityPart.cpp \
    src/core/GameObject.cpp \
    src/core/Map.cpp \
    src/core/MapNode.cpp \
    src/core/Player.cpp \
    src/core/QJsonUtil.cpp \
    src/core/Settlement.cpp \
    src/core/SettlementType.cpp \
    src/core/TerrainType.cpp \
    src/core/Unit.cpp \
    src/core/UnitClass.cpp \
    src/core/UnitLevel.cpp \
    src/core/UnitType.cpp \
    src/core/Weapon.cpp \
    src/core/World.cpp \
    src/core/WorldSurface.cpp

# ui
SOURCES += \
    src/ui/qtquick2applicationviewer.cpp \
    src/ui/ApplicationContext.cpp \
    src/ui/GameMap.cpp \
    src/ui/MapDrawer.cpp \
    src/ui/MiniMap.cpp \
    src/ui/MapPreview.cpp \
    src/ui/MapUtil.cpp \
    src/ui/UnitRecruit.cpp

# qml
OTHER_FILES += \
    src/ui/qml/EditableMap.qml \
    src/ui/qml/Game.qml \
    src/ui/qml/GameItemInfo.qml \
    src/ui/qml/GameSetup.qml \
    src/ui/qml/InfoBar.qml \
    src/ui/qml/ListEditGroup.qml \
    src/ui/qml/Main.qml \
    src/ui/qml/MapDetails.qml \
    src/ui/qml/MapEditor.qml \
    src/ui/qml/MapItemInfo.qml \
    src/ui/qml/MapItemSelector.qml \
    src/ui/qml/MapList.qml \
    src/ui/qml/MapNodeEdit.qml \
    src/ui/qml/NewGame.qml \
    src/ui/qml/PointBar.qml \
    src/ui/qml/SettlementEdit.qml \
    src/ui/qml/TextEditGroup.qml \
    src/ui/qml/UnitEdit.qml

# deplyment
qml.source = src/ui/qml
qml.target =

worlds.source = worlds
worlds.target =

DEPLOYMENTFOLDERS = qml worlds
