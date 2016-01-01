#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "log/LogStream.h"

namespace warmonger {
namespace core {

/**
 * The father of all exceptions.
 */
class GameException
{
public:
    static const QString name;

    explicit GameException(const QString &message = QString());

    QString getMessage() const;

private:
    const QString message;
};

/**
 * Failed to resolve a reference in the GameObject tree.
 *
 * This occures when parsing Json document representing a GameEntity.
 */
class UnresolvedReferenceError :
    public GameException
{
public:
    static const QString name;

    explicit UnresolvedReferenceError(const QString &message = QString());
};

/**
 * Failed to extract value from QVariant, enclosed value has wrong type.
 */
class QVariantTypeError :
    public GameException
{
public:
    static const QString name;

    explicit QVariantTypeError(const QString &message = QString());
};

/**
 * Invalid value error.
 *
 * A parameter has an invalid value. This occurs when parameters
 * come from "outside", e.g. files, scripts, network.
 */
class ValueError :
    public GameException
{
public:
    static const QString name;

    explicit ValueError(const QString &message = QString());
};

/**
 * I/O Errors.
 */
class IOError :
    public GameException
{
public:
    static const QString name;

    explicit IOError(const QString &message = QString());
};

/**
 * Json parse error.
 */
class JsonParseError :
    public GameException
{
public:
    static const QString name;

    explicit JsonParseError(const QString &message = QString());
};

/**
 * The father of all errors related to game rule violations.
 *
 * All methods checking game rules before performing a game action
 * will throw a child of this class when a game rule violation is
 * detected.
 */
class GameRuleViolationError :
    public GameException
{
public:
    static const QString name;

    explicit GameRuleViolationError(const QString &message = QString());
};

/**
 * A rule related to unit recruitment was violated.
 */
class UnitRecruitError :
    public GameRuleViolationError
{
public:
    static const QString name;

    explicit UnitRecruitError(const QString &message = QString());
};

/**
 * A rule related to map editing was violated.
 */
class MapEditingError :
    public GameRuleViolationError
{
public:
    static const QString name;

    explicit MapEditingError(const QString &message = QString());
};

template <typename T>
void logException(
    log::LogStream &stream,
    const T &e
)
{
    static const QString prefix("warmonger::core::");
    stream <<  prefix << T::name << "<" << e.getMessage() << ">";
}

log::LogStream& operator<<(log::LogStream &stream, const GameException &e);
log::LogStream& operator<<(log::LogStream &stream, const UnresolvedReferenceError &e);
log::LogStream& operator<<(log::LogStream &stream, const QVariantTypeError &e);
log::LogStream& operator<<(log::LogStream &stream, const ValueError &e);
log::LogStream& operator<<(log::LogStream &stream, const IOError &e);
log::LogStream& operator<<(log::LogStream &stream, const JsonParseError &e);
log::LogStream& operator<<(log::LogStream &stream, const GameRuleViolationError &e);
log::LogStream& operator<<(log::LogStream &stream, const UnitRecruitError &e);

} // core
} // warmonger

#endif // CORE_EXCEPTION_H
