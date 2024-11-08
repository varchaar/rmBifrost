#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>


enum refresh_type {
    MONOCHROME,
    MONOCHROME_PENCIL,
    COLOR_FAST,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_CONTENT,
    FULL
};

constexpr auto SCREEN_WIDTH = 1620;
constexpr auto SCREEN_HEIGHT = 2160;

constexpr auto ENV_DEBUG = "BIFROST_DEBUG";



#endif // CONSTANTS_H