#include "utils/Utils.h"

namespace warmonger {
namespace utils {

QString makeFileName(const QString& fileName, const QString& extension)
{
    return fileName + "." + extension;
}

} // namespace utils
} // namespace warmonger
