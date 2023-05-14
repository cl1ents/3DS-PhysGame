#pragma once
#include <3ds.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include "render.hpp"

namespace physics {
    extern cpSpace* space;
    extern cpBody* ball;

    void setUp();
    void step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch);
}