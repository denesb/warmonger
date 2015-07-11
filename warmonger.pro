CONFIG += C++11
QT += qml quick

include(src/src.pri)

# Please do not modify the following two lines. Required for deployment.
include(deploy.pri)
qtcAddDeployment()
