#include <map>

#include "utils/Logging.h"

namespace warmonger {
namespace utils {

std::string trim(const char * fileName)
{
    return std::string(fileName);
}

static void qtMessageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg);

void initLogging()
{
    qInstallMessageHandler(qtMessageHandler);
}

static void qtMessageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    switch(type)
    {
        case QtDebugMsg:
            BOOST_LOG_TRIVIAL(debug) << trim(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtInfoMsg:
            BOOST_LOG_TRIVIAL(info) << trim(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtWarningMsg:
            BOOST_LOG_TRIVIAL(warning) << trim(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtCriticalMsg:
            BOOST_LOG_TRIVIAL(error) << trim(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtFatalMsg:
            BOOST_LOG_TRIVIAL(fatal) << trim(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;
    }
}

} // namespace utils
} // namespace warmonger
