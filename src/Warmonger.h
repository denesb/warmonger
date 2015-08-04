#ifndef WARMONGER_H
#define WARMONGER_H

#include <QGuiApplication>

#include "ui/UserInterface.h"

namespace warmonger {

class Warmonger :
    public QGuiApplication
{
    Q_OBJECT

public:
    Warmonger(int argc, char *argv[]);
    ~Warmonger();

private:
    ui::UserInterface *ui;
};

} // namespace warmonger

#endif // WARMONGER_H
