#include "core/Exception.h"

using namespace warmonger::core;

const QString ExceptionContext::template_str = QString("%1:%2 in function %3");

ExceptionContext::ExceptionContext(const QString &file, const QString &func, int line) :
    file(file),
    func(func),
    line(line)
{
    int pos = this->file.lastIndexOf("src");

    if (pos != -1)
    {
        this->file = this->file.mid(pos + 4);
    }
}

QString ExceptionContext::toStr() const
{
    return ExceptionContext::template_str.arg(this->file).arg(this->line).arg(this->func);
}

QString ExceptionContext::getFile() const
{
    return this->file;
}

QString ExceptionContext::getFunc() const
{
    return this->func;
}

int ExceptionContext::getLine() const
{
    return this->line;
}

const QMap<Exception::ErrorCode, QString> Exception::messageDefinitions = {
    std::make_pair(Exception::General, QStringLiteral("General exception")),
    std::make_pair(Exception::NullPointer, QStringLiteral("Null pointer exception")),
    std::make_pair(Exception::IO, QStringLiteral("IO error")),
    std::make_pair(Exception::FileNotFound, QStringLiteral("File %1 not found")),
    std::make_pair(Exception::CannotOpenFile, QStringLiteral("Cannot open file %1")),
    std::make_pair(Exception::CannotReadFile, QStringLiteral("Cannot read file %1")),
    std::make_pair(Exception::Serialization, QStringLiteral("Serialization error")),
    std::make_pair(Exception::JsonParse, QStringLiteral("Json parse error: %1")),
    std::make_pair(Exception::UnresolvedReference, QStringLiteral("Unable to resolve reference to (%1)%2"))
};

Exception::Exception(ErrorCode code, const ExceptionContext &context, const QStringList &args) :
    code(code),
    args(args),
    context(context)
{
}

Exception::~Exception()
{
}

Exception::ErrorCode Exception::getErrorCode() const
{
    return this->code;
}

ExceptionContext Exception::getContext() const
{
    return this->context;
}

QString Exception::getMessage() const
{
    QString msg = Exception::messageDefinitions[this->code];
    for (const QString str : this->args)
    {
        msg = msg.arg(str);
    }

    return msg;
}

QString Exception::getMessageWithContext() const
{
    QString msg = this->getMessage();

    msg += QStringLiteral(" in ") + this->context.toStr();

    return msg;
}
