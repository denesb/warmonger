## Dependencies

### Build

 * C++17 compliant C++ compiler
 * CMake 3.8.2
 * Qt 5.5 (QtCore and QtQuick)
 * KF5Archive
 * lua 5.3
 * fmt

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
    sudo apt install cmake qtbase5-dev qtdeclarative5-dev libkf5archive-dev liblua5.3-dev libfmt-dev
    sudo apt install doxygen graphviz clang-format

### Install dependencies on Fedora
    sudo dnf install cmake qt5-qtbase-devel qt5-qtdeclarative-devel qt5-qtquickcontrols2 kf5-karchive-devel lua-devel fmt-devel
    sudo dnf install doxygen graphviz clang
