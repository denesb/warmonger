#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <QString>

namespace warmonger {
namespace core {

/**
 * Generic Exception, the father of all exceptions.
 */
class Exception :
    std::exception
{
public:
    explicit Exception(const QString &message = QString());

    QString getMessage() const noexcept;

    const char * what() const noexcept override;

protected:
    const QString message;
};

/**
 * Failed to extract value from QVariant, enclosed value has wrong type.
 */
class QVariantTypeError :
    public Exception
{
public:
    explicit QVariantTypeError(const QString &message = QString());
};

/**
 * Invalid value error.
 *
 * A parameter has an invalid value. This occurs when parameters
 * come from "outside", e.g. files, scripts, network.
 */
class ValueError :
    public Exception
{
public:
    explicit ValueError(const QString &message = QString());
};

/**
 * The father of all errors related to game rule violations.
 *
 * All methods checking game rules before performing a game action
 * will throw a child of this class when a game rule violation is
 * detected.
 */
class GameRuleViolationError :
    public Exception
{
public:
    explicit GameRuleViolationError(const QString &message = QString());
};

/**
 * A rule related to unit recruitment was violated.
 */
class UnitRecruitError :
    public GameRuleViolationError
{
public:
    explicit UnitRecruitError(const QString &message = QString());
};

/**
 * A rule related to map editing was violated.
 */
class MapEditingError :
    public GameRuleViolationError
{
public:
    explicit MapEditingError(const QString &message = QString());
};

} // core
} // warmonger

#endif // CORE_EXCEPTION_H
