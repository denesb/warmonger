SRC = $$PWD

INCLUDEPATH += $$SRC

HEADERS += \
    $$SRC/core/GameEngine.h \
    $$SRC/core/WorldMeta.h \
    $$SRC/core/World.h

SOURCES += \
    $$SRC/Warmonger.cpp \
    $$SRC/core/GameEngine.cpp \
    $$SRC/core/WorldMeta.cpp \
    $$SRC/core/World.cpp

# Add more folders to ship with the application, here
qml.source = ui/qml
qml.target =

worlds.source = ../worlds
worlds.target = 

DEPLOYMENTFOLDERS = qml worlds

# Please do not modify the following two lines. Required for deployment.
include($$SRC/ui/qtquick2applicationviewer.pri)
qtcAddDeployment()
