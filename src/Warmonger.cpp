#include "Warmonger.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    ui(nullptr)
{
    log::Logger::init();

    std::shared_ptr<log::ConsoleHandler> consoleHandler(new log::ConsoleHandler());
    consoleHandler->setLevel(log::Debug);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);

    this->ui = new ui::UserInterface(this);
}

Warmonger::~Warmonger()
{
    delete ui;
}
