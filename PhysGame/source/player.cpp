#include <math.h>

#include "player.hpp"

#include "render.hpp"
#include "physics.hpp"

void playerVelocityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt) 
{
    cpBodyUpdateVelocity(body, gravity, damping, dt);
    body->w = 0;
    body->a = 0;
}

Player::Player(cpVect pos, cpVect size) {
    // Configuation
    fallOff = 15;
    acceleration = 20000;
    maxSpeed = 500;

    // Creating physics object
    body = cpBodyNew(1.0f, 1.0f);
    cpBodySetVelocityUpdateFunc(body, playerVelocityFunc);
    cpBodySetPosition(body, pos);
    { // Hitbox creation
        cpVect verts[] = boxVerts(size.x, size.y);

        Hitbox = cpPolyShapeNew(body, 4, verts, cpTransformIdentity, 1.0);
        cpShapeSetElasticity(Hitbox, 0);
	    cpShapeSetFriction(Hitbox, 0);
        cpShapeSetDensity(Hitbox, 1);
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

    //cpBodySetVelocity(body, cpv(cpad.dx*deltaTime*acceleration, body->v.y));
    float goal = 0;
    
    body->v = cpv(body->v.x*(1-fmin(fallOff*deltaTime, 1)), body->v.y);

    if (abs(cpad.dx) > 10)
        goal = cpad.dx/154;

/*
    if (goal > 0)
        body->v = body->v + cpv(fmin(goal*acceleration*deltaTime, (maxSpeed-body->v.x)), 0);
    else
        body->v = body->v + cpv(fmax(goal*acceleration*deltaTime, ((-maxSpeed)-body->v.x)), 0);
*/

    body->v = cpv(goal*maxSpeed, body->v.y); // Set velocity directly (too harsh)
    
    printf("vel = %.3f %.3f\n", body->v.x, body->v.y);
}