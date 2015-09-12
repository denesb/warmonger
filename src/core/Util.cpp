#include "core/Util.h"

namespace warmonger {
namespace core {

QString getClassName(const QString &fullClassName)
{
    QStringList classNameParts = fullClassName.split("::");
    return classNameParts.takeLast();
}

}; // namespace core
}; // namespace warmonger
