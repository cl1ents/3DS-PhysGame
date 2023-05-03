#pragma once
#include <vector>

namespace render {
    extern cpVect offset;

    struct shapeUserData {
        u32 color;
    };

    struct polyUserData : render::shapeUserData {
        std::vector<double> poly;
    };

    render::shapeUserData *getRenderData(cpShape *shape);

    bool isShapeOnscreen(cpShape *shape);
    void drawShape(cpShape *shape, void *v = nullptr);
}