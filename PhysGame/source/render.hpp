#pragma once
#include <citro2d.h>
#include <vector>

#define TOP_SCREEN_WIDTH  400
#define BOTTOM_SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

namespace render {
    extern cpVect offset;
    extern C3D_RenderTarget* bottom;
    extern C3D_RenderTarget* top;

    struct shapeUserData {
        u32 color;
    };

    struct polyUserData : render::shapeUserData {
        std::vector<double> poly;
    };

    render::shapeUserData *getRenderData(cpShape *shape);

    bool isShapeOnscreen(cpShape *shape);
    void drawShape(cpShape *shape, void *v = nullptr);

    void setUp();
    void renderFrame(float deltaTime);
}