#include <QFile>

#include "core/JsonUtil.h"

static const QString loggerName("core.JsonUtil");

using namespace warmonger;

QJsonObject core::sizeToJson(const QSize &size)
{
    QJsonObject obj;

    obj["width"] = size.width();
    obj["height"] = size.height();

    return std::move(obj);
}

QJsonDocument core::loadJsonDocument(const QString &path)
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

    return std::move(doc);
}

void core::saveJsonDocument(const QString &path, const QJsonDocument &doc)
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

