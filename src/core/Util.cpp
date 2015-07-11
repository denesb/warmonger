#include <QTextStream>

#include "core/Util.h"

namespace warmonger {
namespace core {

QString pos2str(const QPoint &pos)
{
    static const QString strTemplate("%1,%2");

    return std::move(strTemplate.arg(pos.x()).arg(pos.y()));
}

QPoint str2pos(QString str)
{
    int x, y;
    char comma;
    QTextStream s(&str);

    s >> x >> comma >> y;

    return std::move(QPoint(x, y));
}


}; // namespace core
}; // namespace warmonger
