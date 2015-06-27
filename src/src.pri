SRC = $$PWD

INCLUDEPATH += $$SRC

HEADERS += \
    $$SRC/core/GameEngine.h \
	$$SRC/core/WorldItem.h \
    $$SRC/core/World.h \
	$$SRC/core/TerrainType.h \
	$$SRC/core/UnitClass.h

SOURCES += \
    $$SRC/Warmonger.cpp \
    $$SRC/core/GameEngine.cpp \
	$$SRC/core/WorldItem.cpp \
    $$SRC/core/World.cpp \
	$$SRC/core/TerrainType.cpp \
	$$SRC/core/UnitClass.cpp

# Add more folders to ship with the application, here
qml.source = ui/qml
qml.target =

worlds.source = ../worlds
worlds.target = 

DEPLOYMENTFOLDERS = qml worlds

# Please do not modify the following two lines. Required for deployment.
include($$SRC/ui/qtquick2applicationviewer.pri)
qtcAddDeployment()
