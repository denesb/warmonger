#include <QFile>

#include "core/JsonUtil.h"

static const QString category("core.JsonUtil");

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
        wError(category) << "Failed to open Json document from path " << path;
        throw Exception(Exception::FileIO, "File open failed");
    }

    QByteArray jsonData = jsonFile.readAll();

    jsonFile.close();

    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(jsonData, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        wError(category) << "Parse of Json document " << path << "failed: "
            << parseError.errorString();
        throw Exception(Exception::JsonParse);
    }

    wInfo("core.JsonUtil") << "Loaded Json document from " << path;

    return std::move(doc);
}

void core::saveJsonDocument(const QString &path, const QJsonDocument &doc)
{
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        wError(category) << "Failed to open Json document from path " << path;
        throw Exception(Exception::FileIO, "File open failed");
    }

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    if (jsonFile.write(jsonData) == -1)
    {
        wError(category) << "Failed to write Json document " << path;
        throw Exception(Exception::FileIO, "Failed to write file");
    }
    
    wInfo(category) << "Saved Json document to " << path;
}

