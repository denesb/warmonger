#include "io/Exception.h"

using namespace warmonger;
using namespace warmonger::io;

std::ostream& operator<<(std::ostream &os, const UnresolvedReferenceError &e)
{
    os << "<UnresolvedReferenceError " << e.getMessage().toStdString() << ">";
    return os;
}
