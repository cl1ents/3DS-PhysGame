#define sign(x) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
#pragma once
#include <3ds.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

class Player {
    float maxSpeed;
    float acceleration;
    float fallOff;
    float jumpPower;
    public:
        cpBody *body;
        cpShape *Hitbox;
        void step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch, circlePosition cpad);

        Player(cpVect pos, cpVect size);
};