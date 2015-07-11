#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <QString>
#include <QPoint>

namespace warmonger {
namespace core {

QString pos2str(const QPoint &pos);
QPoint str2pos(QString str);

}; // namespace core
}; // namespace warmonger

#endif // CORE_UTIL_H
