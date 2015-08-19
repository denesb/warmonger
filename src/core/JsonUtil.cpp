#include <QFile>

#include "core/JsonUtil.hpp"

static const QString module("core.JsonUtil");

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
        Exception e(Exception::FileOpenFailed, {path});
        wError(module) << e.getMessage();
        throw e;
    }

    QByteArray jsonData = jsonFile.readAll();

    jsonFile.close();

    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(jsonData, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        Exception e(Exception::JsonParse, {path, parseError.errorString()});
        wError(module) << e.getMessage();
        throw e;
    }

    wInfo("core.JsonUtil") << "Loaded Json document from " << path;

    return std::move(doc);
}

void core::saveJsonDocument(const QString &path, const QJsonDocument &doc)
{
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        Exception e(Exception::FileOpenFailed, {path});
        wError(module) << e.getMessage();
        throw e;
    }

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

    if (jsonFile.write(jsonData) == -1)
    {
        Exception e(Exception::FileWriteFailed, {path});
        wError(module) << e.getMessage();
        throw e;
    }
    
    wInfo("core.JsonUtil") << "Saved Json document to " << path;
}

