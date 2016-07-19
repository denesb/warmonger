#include <map>

#include "Version.h"
#include "utils/Logging.h"

namespace warmonger {
namespace utils {

std::string trimSrcFilePath(const char* fileName)
{
    const std::string path(fileName);
    if (path.compare(0, basePath.size(), basePath) == 0)
    {
        return path.substr(basePath.size() + 1);
    }
    else
    {
        return path;
    }
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
            BOOST_LOG_TRIVIAL(debug) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtInfoMsg:
            BOOST_LOG_TRIVIAL(info) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtWarningMsg:
            BOOST_LOG_TRIVIAL(warning) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtCriticalMsg:
            BOOST_LOG_TRIVIAL(error) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtFatalMsg:
            BOOST_LOG_TRIVIAL(fatal) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;
    }
}

} // namespace utils
} // namespace warmonger
