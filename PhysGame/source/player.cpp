#include "player.hpp"

#include "render.hpp"
#include "physics.hpp"

Player::Player(cpVect pos, cpVect size) {
    // Creating physics object
    body = cpBodyNew(1.0f, 1.0f);
    cpBodySetPosition(body, pos);
    { // Hitbox creation
        cpVect verts[] = boxVerts(size.x, size.y);

        Hitbox = cpPolyShapeNew(body, 4, verts, cpTransformIdentity, 1.0);
        cpShapeSetElasticity(Hitbox, 0.0f);
	    cpShapeSetFriction(Hitbox, 0.2f);
        cpShapeSetDensity(Hitbox, .2);
    }

    cpSpaceAddBody(physics::space, body);
    cpSpaceAddShape(physics::space, Hitbox);
}

void Player::step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch, circlePosition cpad)
{
    if (kDown & KEY_SELECT)
    {
        cpBodySetVelocity(body, cpv(0,0));
        cpBodySetPosition(body, cpv(0,0));
    }

    if (abs(cpad.dx) > 10) {
        cpBodySetForce(body, cpv(cpad.dx*1000, 0));
        printf("cpad = %d %d\n", cpad.dx, cpad.dy);
    }
    body->w = 0;
    body->a = 0;
}