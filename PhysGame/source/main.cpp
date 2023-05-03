#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <math.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include "render.hpp"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

int main(int argc, char **argv)
{
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_TOP, NULL);

    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    touchPosition touch;

    // SET UP SPACE!

    cpSpace *space = cpSpaceNew();
	cpSpaceSetIterations(space, 20);

    cpSpaceSetGravity(space, cpv(0, 1000.0f));
    cpBody *ball = cpSpaceAddBody(space, cpBodyNew(1.0f, 1.0f));
    cpBodySetPosition(ball, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    {
        cpShape *shape = cpSpaceAddShape(space, cpCircleShapeNew(ball, 15.0f, cpvzero));
    	cpShapeSetElasticity(shape, .8f);
	    cpShapeSetFriction(shape, 1.0f);
        cpShapeSetDensity(shape, .2);
    }

    cpBody *platform = cpSpaceAddBody(space, cpBodyNewStatic());
    cpBodySetPosition(platform, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT-25.0));
    {
        float h = 50;
        float w = SCREEN_WIDTH;
        cpVect verts[] = {
            cpv(-w/2.0f,-h/2.0f),
            cpv(-w/2.0f, h/2.0f),
            cpv( w/2.0f, h/2.0f),
            cpv( w/2.0f,-h/2.0f),
            cpv(-w/2.0f,-h/2.0f),
        };
        cpShape *shape = cpSpaceAddShape(space, cpPolyShapeNew(platform, 4, verts, cpTransformIdentity, 0.0));
        cpShapeSetElasticity(shape, 0.1f);
	    cpShapeSetFriction(shape, 0.5f);
        cpShapeSetDensity(shape, 1.0f);
    }

    cpBody *box = cpSpaceAddBody(space, cpBodyNew(1.0f, 1.0f));
    cpBodySetPosition(box, cpv(SCREEN_WIDTH/3, SCREEN_HEIGHT/2));
    {
        float h = 100;
        float w = 100;
        cpVect verts[] = {
            cpv(-w/2.0f,-h/2.0f),
            cpv(-w/2.0f, h/2.0f),
            cpv( w/2.0f, h/2.0f),
            cpv( w/2.0f,-h/2.0f),
            cpv(-w/2.0f,-h/2.0f),
        };
        cpShape *shape = cpSpaceAddShape(space, cpPolyShapeNew(box, 4, verts, cpTransformIdentity, 0.0));
        cpShapeSetElasticity(shape, .8f);
	    cpShapeSetFriction(shape, 1.0f);
        cpShapeSetDensity(shape, .2);
    }

    cpBody *border = cpSpaceAddBody(space, cpBodyNewStatic());
    // cpBodySetPosition(platform, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT-25.0));
    {
        cpVect offset = cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        float radius = 500;
        float h = SCREEN_HEIGHT+radius*2;
        float w = SCREEN_WIDTH+radius*2;
        cpVect verts[] = {
            cpv(-w/2.0f,-h/2.0f),
            cpv(-w/2.0f, h/2.0f),
            cpv( w/2.0f, h/2.0f),
            cpv( w/2.0f,-h/2.0f),
            cpv(-w/2.0f,-h/2.0f),
        };
        
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

    // Main loop
    
    printf("Chipmunk %s\n", cpVersionString);
    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break;
        
        // Physics

        if (kDown & KEY_TOUCH)
        {
            hidTouchRead(&touch);

            cpVect click = cpv(touch.px, touch.py)+render::offset;
            cpBodySetVelocity(ball, cpvmult(cpvsub(click, ball->p), 60));
        }
        if (kDown & KEY_A)
        {
            cpBodySetVelocity(ball, cpv(0,0));
            cpBodySetAngularVelocity(ball, 0);
            cpBodySetPosition(ball, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
        }

        cpSpaceStep(space, 1.0/60);
        render::offset = ball->p-cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

        // RENDER
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(bottom, C2D_Color32f(1, 1, 1, 1));
        C2D_SceneBegin(bottom);

        cpSpaceEachShape(space, render::drawShape, NULL);

        C3D_FrameEnd(0);
	}

	C2D_Fini();

    C3D_Fini();

    cpSpaceFree(space);

    gfxExit();
	return 0;
}