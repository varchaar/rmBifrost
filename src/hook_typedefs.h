#ifndef HOOK_TYPEDEFS_H
#define HOOK_TYPEDEFS_H

#include <QtCore/QObject>

inline const auto create_ep_fb_addr = reinterpret_cast<void*>(0x0090fed0);

struct point {
    int x;
    int y;
};

typedef void (*ScreenUpdateFunc)(QObject*, point start, point end, int, int, int);

inline const auto screen_update_func_addr = reinterpret_cast<void*>(0x0090b220);

#endif // HOOK_TYPEDEFS_H
