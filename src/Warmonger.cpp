#include "Warmonger.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "log/Formatter.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    ui(nullptr)
{
    log::Logger::init();

    const QString formatStr("%{level} {%{name}}: %{message}");
    std::shared_ptr<log::Formatter> formatter(new log::Formatter(formatStr));

    std::shared_ptr<log::ConsoleHandler> consoleHandler(new log::ConsoleHandler());
    consoleHandler->setLevel(log::Debug);
    consoleHandler->setFormatter(formatter);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);

    this->ui = new ui::UserInterface(this);
}

Warmonger::~Warmonger()
{
    delete ui;
}
