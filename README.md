# Warmonger

Warmonger is (will be) a turn based strategy game-engine inspired by
[Battle for Wesnoth](http://wesnoth.org),
[Heroes of Might and Magic](https://en.wikipedia.org/wiki/Heroes_of_Might_and_Magic),
the [Total War series](https://en.wikipedia.org/wiki/Total_War_(series)),
[Mount & Blade](https://en.wikipedia.org/wiki/Mount_%26_Blade) and some other
strategy games mixed with my own ideas.

The project is under development and is not functional yet. As it is a pet
project and I work on it in my free time, I have no idea when it will be
anywhere near ready to use.

## Why?

Mainly because I love to write code and I enjoy working on it. I also
want to experiment with my own ideas for the perfect turn-based strategy
game.

## How?

Warmonger is written with moddability and hackability in mind from the very
first line of code where mods are first-class and there is no favoured built-in
content.

Warmonger is a hexagon-based wargame, with action happens is happening on the
campaign map and battles being resolved on the battle map.

See the [design overview](doc/OVERVIEW.md) for more details.

## Contributing

Warmonger doesn't have a roadmap yet, mainly because I'm still making up my
mind about some of the essential design decisions. Nevertheless if you are
interested feel free to drop me a line at dns dot botond at gmail dot com.

## Dependencies

### Build

 * C++14 compliant C++ compiler (tested with gcc 5.4)
 * CMake
 * Qt 5.5 (QtCore and QtQuick)
 * KF5Archive
 * lua 5.3

### Format the code

 * clang-format (clang-tools)

### Generate documentation

 * doxygen 1.8
 * graphviz

Version numbers refer to the minimum supported version of each dependency.

### Other thirdparty libraries used (bundled)

 * [catch](https://github.com/philsquared/Catch)
 * [backward-cpp](https://github.com/bombela/backward-cpp)
 * [spdlog](https://github.com/gabime/spdlog)
 * [sol2](https://github.com/ThePhD/sol2)

Kudos to the authors and contributors of all this awesome, freely available stuff.

### Install dependencies on Ubuntu
    sudo apt install cmake qtbase5-dev qtdeclarative5-dev libkf5archive-dev liblua5.3-dev
    sudo apt install doxygen graphviz clang-format

### Install dependencies on Fedora
    sudo dnf install cmake qt5-qtbase-devel qt5-qtdeclarative-devel kf5-karchive-devel lua-devel
    sudo dnf install doxygen graphviz clang

## Other awesome strategy game projects
 * [Battle for Wesnoth](https://www.wesnoth.org/)
 * [0 A.D.](https://play0ad.com/)
 * [OpenRA](http://www.openra.net/)
 * [TripleA](http://www.triplea-game.org/)
 * [OpenAge](http://openage.sft.mx/)
 * [Wyrmsun](https://andrettin.github.io/)
 * [FreeCiv](http://www.freeciv.org/)
 * [FreeCol](http://www.freecol.org/)
 * [VCMI - HOMM 3 WOG recreated](http://vcmi.eu/)
