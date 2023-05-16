#pragma once
#include <3ds.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

class Player {
    public:
        cpBody *body;
        cpShape *Hitbox;
        void step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch, circlePosition cpad);

        Player(cpVect pos, cpVect size);
};