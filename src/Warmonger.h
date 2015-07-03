#ifndef WARMONGER_H
#define WARMONGER_H

#include "core/WorldLoader.h"
#include "ui/model/WorldList.h"

namespace warmonger {

class Warmonger :
    public QObject
{
    Q_OBJECT

public:
    Warmonger(QObject *parent = nullptr);
    ~Warmonger();

    int exec(int argc, char *argv[]);

private:
    void setupModels();
    void setupViews();

    core::WorldLoader *worldLoader;
    ui::model::WorldList *worldList;
};

}; // namespace warmonger

#endif // WAMNOGER_H
