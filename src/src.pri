SRC = $$PWD

INCLUDEPATH += $$SRC

#HEADERS += 

SOURCES += \
           $$SRC/main.cpp

# Add more folders to ship with the application, here
qml.source = qml/warmonger
qml.target = qml
DEPLOYMENTFOLDERS = qml

# Please do not modify the following two lines. Required for deployment.
include($$SRC/qtquick2applicationviewer.pri)
qtcAddDeployment()
