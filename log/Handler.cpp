#include "log/Handler.h"

using namespace warmonger::log;

Handler::Handler() :
    lvl(Warning),
    formatter(new Formatter()),
    filters()
{
}

Handler::~Handler()
{
}

Handler& Handler::operator<<(const LogRecord &record)
{
    if (record.getLevel() < this->lvl)
        return *this;

    bool pass = true;
    for (std::shared_ptr<Filter> filter : this->filters)
        pass = pass && filter->filter(record);

    if (!pass)
        return *this;

    QString msg{this->formatter->format(record)};

    this->handle(msg);

    return *this;
}

void Handler::setLevel(LogLevel lvl)
{
    this->lvl = lvl;
}

void Handler::setFormatter(std::shared_ptr<Formatter> formatter)
{
    this->formatter = formatter;
}

void Handler::addFilter(std::shared_ptr<Filter> filter)
{
    this->filters << filter;
}

void Handler::removeFiler(std::shared_ptr<Filter> filter)
{
    this->filters.removeOne(filter);
}
