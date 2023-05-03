#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <math.h>
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include <chrono>

#include "render.hpp"
#include "physics.hpp"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

using namespace std::chrono;

int main(int argc, char **argv)
{
    gfxInitDefault();

    render::setUp();

    consoleInit(GFX_TOP, NULL);

    touchPosition touch;

    // SET UP SPACE!
    physics::setUp();
    // Main loop
    
    printf("Chipmunk %s\n", cpVersionString);
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    srand(ms.count());
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
            cpBodySetVelocity(physics::ball, cpvmult(cpvsub(click, physics::ball->p), 60));
        }
        if (kDown & KEY_A)
        {
            cpBodySetVelocity(physics::ball, cpv(0,0));
            cpBodySetAngularVelocity(physics::ball, 0);
            cpBodySetPosition(physics::ball, cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
        }

        cpSpaceStep(physics::space, 1.0/60);
        render::offset = physics::ball->p-cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

        // RENDER
        render::renderFrame(1.0/60);
	}

	C2D_Fini();

    C3D_Fini();

    cpSpaceFree(physics::space);

    gfxExit();
	return 0;
}