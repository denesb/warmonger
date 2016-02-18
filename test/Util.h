#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <memory>
#include <string>
#include <ostream>

#include <QString>

#include "core/World.h"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s);

std::unique_ptr<core::World> makeWorld();

#endif // TEST_UTIL_H
