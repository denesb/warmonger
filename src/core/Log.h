#ifndef CORE_LOG_H
#define CORE_LOG_H

#include <QDebug>
#include <QLoggingCategory>

namespace warmonger {
namespace core {

Q_DECLARE_LOGGING_CATEGORY(root)
Q_DECLARE_LOGGING_CATEGORY(json)

} // namespace core
} // namespace warmonger

#endif // CORE_LOG_H
