#include "physics.hpp"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

cpSpace* physics::space;
Player* physics::player;

void physics::setUp()
{
    physics::space = cpSpaceNew();
    cpSpace *space = physics::space;

	cpSpaceSetIterations(space, 20);

    cpSpaceSetGravity(space, cpv(0, 1000.0f));

    /*
    cpBody *ball = cpSpaceAddBody(space, cpBodyNew(1.0f, 1.0f));
    cpBodySetPosition(ball, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    {
        cpShape *shape = cpSpaceAddShape(space, cpCircleShapeNew(ball, 15.0f, cpvzero));
    	cpShapeSetElasticity(shape, .8f);
	    cpShapeSetFriction(shape, 1.0f);
        cpShapeSetDensity(shape, .2);
    }
    physics::ball = ball;
    */

   physics::player = new Player(cpv(0,0), cpv(16, 20));

    cpBody *platform = cpSpaceAddBody(space, cpBodyNewStatic());
    cpBodySetPosition(platform, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT-25.0));
    {
        float h = 50;
        float w = 10000;
        cpVect verts[] = boxVerts(w, h);
        cpShape *shape = cpSpaceAddShape(space, cpPolyShapeNew(platform, 4, verts, cpTransformIdentity, 0.0));
        cpShapeSetElasticity(shape, 0.1f);
	    cpShapeSetFriction(shape, 2.0f);
        cpShapeSetDensity(shape, 1.0f);
    }

    cpBody *box = cpSpaceAddBody(space, cpBodyNew(1.0f, 1.0f));
    cpBodySetPosition(box, cpv(SCREEN_WIDTH/3, SCREEN_HEIGHT/2));
    {
        float h = 100;
        float w = 100;
        cpVect verts[] = boxVerts(w, h);
        cpShape *shape = cpSpaceAddShape(space, cpPolyShapeNew(box, 4, verts, cpTransformIdentity, 0.0));
        cpShapeSetElasticity(shape, .8f);
	    cpShapeSetFriction(shape, 1.0f);
        cpShapeSetDensity(shape, .1);
    }

/*
    cpBody *border = cpSpaceAddBody(space, cpBodyNewStatic());
    // cpBodySetPosition(platform, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT-25.0));
    {
        cpVect offset = cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        float radius = 10;
        float h = SCREEN_HEIGHT+radius*2;
        float w = SCREEN_WIDTH+radius*2;
        cpVect verts[] = boxVerts(w, h);
        
        for(int i=0; i<(sizeof(verts)/sizeof(cpVect) - 1); i++)
        {
		    cpVect a = verts[i], b = verts[i+1];
            cpShape *shape = cpSegmentShapeNew(border, cpvadd(a, offset), cpvadd(b, offset), radius);
            cpShapeSetElasticity(shape, 1.0f);
            cpShapeSetFriction(shape, 1.0f);
            cpShapeSetDensity(shape, .5);
            cpSpaceAddShape(space, shape);
        }
    }
*/
};

cpVect click;
circlePosition cpad;
void physics::step(float deltaTime, u32 kUp, u32 kDown, u32 kHeld, u32 kRepeat, touchPosition touch)
{
    /*
    if (kHeld & KEY_TOUCH)
    {
        click = cpv(touch.px, touch.py)+render::offset;
        cpBodySetVelocity(physics::ball, cpvmult(cpvsub(click, physics::ball->p), 60));
    }*/
    /*
    if (kDown & KEY_SELECT)
    {
        cpBodySetVelocity(physics::ball, cpv(0,0));
        cpBodySetAngularVelocity(physics::ball, 0);
        cpBodySetPosition(physics::ball, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    }

    hidCircleRead(&cpad);
    
    if (abs(cpad.dx) > 10) {
        physics::ball->w += cpad.dx*deltaTime*10;
    }
    printf("cpad = %d %d\n", cpad.dx, cpad.dy);
    physics::ball->w *= .8;
    */

    hidCircleRead(&cpad);
    physics::player->step(deltaTime, kUp, kDown, kHeld, kRepeat, touch, cpad);

    cpSpaceStep(physics::space, deltaTime);
}