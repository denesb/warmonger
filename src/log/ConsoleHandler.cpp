#include <iostream>

#include "log/ConsoleHandler.h"

using namespace warmonger::log;

ConsoleHandler::ConsoleHandler() :
    Handler(),
    stream(stdout)
{
}

ConsoleHandler::~ConsoleHandler()
{
}

void ConsoleHandler::handle(const QString &msg)
{
    this->stream << msg << endl;
}
