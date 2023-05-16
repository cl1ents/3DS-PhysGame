#define boxVerts(w, h) { cpv(-w/2.0f,-h/2.0f), cpv(-w/2.0f, h/2.0f), cpv( w/2.0f, h/2.0f), cpv( w/2.0f,-h/2.0f), cpv(-w/2.0f,-h/2.0f) }

#pragma once
#include <3ds.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include "player.hpp"
#include "render.hpp"

namespace physics {
    extern cpSpace* space;
    extern Player* player;

    void setUp();
    void step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch);
}