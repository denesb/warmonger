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
first line of code where mods are first-world content and there is no
favoured built-in content.

Warmonger is a hexagon-based wargame, with action happens is happening on the
campaign map and battles being resolved on the battle map.

See the [design overview](docs/OVERVIEW.md) for more details.

## Dependencies
* C++14 compliant C++ compiler
* boost 1.58 (boost::log and boost::optional)
* Qt 5.5 (QtCore and QtQuick)
* KF5Archive

Version numbers refer to the minimum supported version of each dependency.
