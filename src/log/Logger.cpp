#include <functional>
#include "log/Logger.h"

using namespace warmonger::log;

QMap<QString, Logger*> Logger::loggers = QMap<QString, Logger*>();

void Logger::init()
{
    Logger *root = new Logger("root", nullptr);

    Logger::loggers["root"] = root;
    Logger::loggers[""] = root;
    Logger::loggers[QString()] = root;
}

Logger * Logger::get(const QString &name)
{
    QMap<QString, Logger *>::iterator it = Logger::loggers.find(name);
    if (it != Logger::loggers.end())
    {
        return it.value();
    }

    Logger *parent;
    int i = name.lastIndexOf('.');

    if (i == -1)
    {
        parent = Logger::loggers["root"];
    }
    else
    {
        parent = Logger::get(name.mid(0, i));
    }

    Logger *logger = new Logger(name, parent);
    Logger::loggers.insert(name, logger);
    return logger;
}

void Logger::addHandler(const std::shared_ptr<Handler> &handler)
{
    this->handlers << handler;
}

void Logger::removeHandler(const std::shared_ptr<Handler> &handler)
{
    this->handlers.removeOne(handler);
}

void Logger::logRecord(const LogRecord &record) const
{
    if (this->handlers.empty())
    {
        if (this->parent != nullptr)
        {
            this->parent->logRecord(record);
        }
    }
    else
    {
        for (std::shared_ptr<Handler> handler : this->handlers)
        {
            *handler << record;
        }
    }
}

Logger::Logger(const QString &name, Logger *parent) :
    name(name),
    parent(parent),
    handlers()
{
}

Logger::~Logger()
{
}
