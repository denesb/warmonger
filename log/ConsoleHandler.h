#ifndef LOG_CONSOLE_HANDLER_H
#define LOG_CONSOLE_HANDLER_H

#include <QTextStream>
#include "log/Handler.h"

namespace warmonger {
namespace log {

class ConsoleHandler :
    public Handler
{
public:
    ConsoleHandler();
    virtual ~ConsoleHandler();

private:
    void handle(const QString &msg) override;

    QTextStream stream;
};

} // namespace log
} // namespace warmonger

#endif // LOG_CONSOLE_HANDLER_H
