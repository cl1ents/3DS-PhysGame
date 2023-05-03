#pragma once
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

namespace physics {
    extern cpSpace* space;
    extern cpBody* ball;

    void setUp();
}