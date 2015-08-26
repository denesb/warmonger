#ifndef WARMONGER_H
#define WARMONGER_H

#include <QGuiApplication>

#include "ui/qtquick2applicationviewer.h"
#include "ui/ApplicationContext.h"

namespace warmonger {

class Warmonger :
    public QGuiApplication
{
    Q_OBJECT

public:
    Warmonger(int argc, char *argv[]);
    ~Warmonger();

private:
    void readSettings();
    void initLogger();
    void initUi();

    QtQuick2ApplicationViewer viewer;
    ui::ApplicationContext *ctx;
};

} // namespace warmonger

#endif // WARMONGER_H
