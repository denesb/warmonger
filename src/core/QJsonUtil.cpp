#include <QFile>

#include "core/QJsonUtil.h"

static const QString loggerName("core.QJsonUtil");

using namespace warmonger;
namespace warmonger {
namespace core {

QSize sizeFromJson(const QJsonObject &obj)
{
    return QSize(obj["width"].toInt(), obj["height"].toInt());
}

QJsonObject sizeToJson(const QSize &size)
{
    QJsonObject obj;

    obj["width"] = size.width();
    obj["height"] = size.height();

    return obj;
}

QJsonDocument loadJsonDocument(const QString &path)
{
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        wError(loggerName) << "Failed to open Json document from path " << path;
        throw IOError("Failed to open " + path);
    }

    QByteArray jsonData = jsonFile.readAll();

    jsonFile.close();

    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(jsonData, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        wError(loggerName) << "Parse of Json document " << path << " failed: "
            << parseError.errorString() << " at offset " << parseError.offset;
        throw JsonParseError(
            parseError.errorString() + " at " + parseError.offset
        );
    }

    wInfo(loggerName) << "Loaded Json document from " << path;

    return doc;
}

void saveJsonDocument(const QString &path, const QJsonDocument &doc)
{
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        wError(loggerName) << "Failed to open Json document from path " << path;
        throw IOError("Failed to open " + path);
    }

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    if (jsonFile.write(jsonData) == -1)
    {
        wError(loggerName) << "Failed to write Json document " << path;
        throw IOError("Failed to open " + path);
    }

    wInfo(loggerName) << "Saved Json document to " << path;
}

} // namespace core
} // namespace warmonger
