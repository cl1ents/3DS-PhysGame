#include <3ds.h>
#include <stdio.h>
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

    consoleInit(GFX_BOTTOM, NULL);

    touchPosition touch;

    // SET UP SPACE!
    physics::setUp();
    // Main loop
    
    printf("Chipmunk %s\n", cpVersionString);
    printf("Inf = %d\n", INFINITY);
    printf("Test1 = %d\n", 1e300);
    printf("Test2 = %d\n", 1e300*1e300);
    printf("Test3 = %d\n", __builtin_inff());
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    srand(ms.count());

    float deltaTime = 1.0/60;
    while (aptMainLoop())
    {
        hidScanInput();

        u32 kUp = hidKeysUp();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        u32 kRepeat = hidKeysDownRepeat();

        if (kDown & KEY_START) break;
        if (kHeld & KEY_TOUCH)
        {
            hidTouchRead(&touch);
        }

        // PHYSICS
        physics::step(deltaTime, kUp, kDown, kHeld, kRepeat, touch);

        // RENDER
        render::renderFrame(deltaTime);
	}

	C2D_Fini();

    C3D_Fini();

    cpSpaceFree(physics::space);

    gfxExit();
	return 0;
}