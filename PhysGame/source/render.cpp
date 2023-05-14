#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>

#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include <tuple>
#include <vector>

#include "render.hpp"
#include "physics.hpp"
#include "hsv.cpp"
#include "Del.hpp"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

using Coord = double;
using Point = std::vector<Coord>;
using N = uint32_t;

using namespace render;

const u32 bgcolor = C2D_Color32f(1,1,1,1);

cpBB screen = cpBBNewForExtents(cpv(SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0), SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0);

cpVect render::offset = cpv(0,10);
C3D_RenderTarget *render::bottom;

render::shapeUserData *render::getRenderData(cpShape *shape)
{
    render::shapeUserData *data = (render::shapeUserData *)cpShapeGetUserData(shape);

    if (data == nullptr)
    {
        cpShapeType type = shape->klass->type;

        if (type == CP_POLY_SHAPE)
        {
            render::polyUserData *newData = new render::polyUserData();

            std::vector<double> coords = {};

            int vertCount = cpPolyShapeGetCount(shape);
            
            for(int i=0; i<vertCount; i++)
            {
                cpVect point = cpPolyShapeGetVert(shape, i);
                coords.push_back(point.x);
                coords.push_back(point.y);
            }

            delaunator::Delaunator d(coords);

            newData->poly = {};

            for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
                newData->poly.push_back(d.coords[2 * d.triangles[i]]); //tx0
                newData->poly.push_back(d.coords[2 * d.triangles[i] + 1]); //ty0

                newData->poly.push_back(d.coords[2 * d.triangles[i + 1]]); //tx1
                newData->poly.push_back(d.coords[2 * d.triangles[i + 1] + 1]); //ty1
                
                newData->poly.push_back(d.coords[2 * d.triangles[i + 2]]); //tx2
                newData->poly.push_back(d.coords[2 * d.triangles[i + 2] + 1]); //ty2
            }

            cpShapeSetUserData(shape, newData);
            printf("Set poly data, %p\n", newData);

            data = (render::shapeUserData *)newData;
        }
        else
        {
            render::shapeUserData *newData = new render::shapeUserData();

            cpShapeSetUserData(shape, newData);
            printf("Set shape data, %p\n", newData);

            data = (render::shapeUserData *)newData;
        }

        data->color = C2D_Color32f((rand()%1000)/1000.0f, (rand()%1000)/1000.0f, (rand()%1000)/1000.0f, 1);
    }

    return (render::shapeUserData *)data;
}

bool render::isShapeOnscreen(cpShape *shape)
{
    screen = cpBBNewForExtents(cpv(SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0)+render::offset, SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0);
    return cpBBIntersects(screen, cpShapeGetBB(shape));
}

void render::drawShape(cpShape *shape, void *v) {
    if (!render::isShapeOnscreen(shape)) return;
    
    render::shapeUserData *data = getRenderData(shape);

    cpBody *body = cpShapeGetBody(shape);
    // rgb colorRGB = hsv2rgb({(ms.count()/50)%360, .8, .8});
    u32 color = data->color; //C2D_Color32f(colorRGB.r,colorRGB.g,colorRGB.b,1);

    cpShapeType type = shape->klass->type;

    if (type == CP_CIRCLE_SHAPE) {
        cpCircleShape* circle = (cpCircleShape *)shape;
        cpVect pos = cpBodyLocalToWorld(body, circle->c) - render::offset;
        cpVect lineEnd = cpvadd(cpBodyLocalToWorld(body, cpv(0, cpCircleShapeGetRadius(shape))), cpvsub(pos, body->p));
        C2D_DrawCircleSolid(pos.x, pos.y, .5, cpCircleShapeGetRadius(shape), color);
        C2D_DrawLine(pos.x, pos.y, color, lineEnd.x, lineEnd.y, bgcolor, 1, .5);
    }
    else if (type == CP_SEGMENT_SHAPE) 
    {
        cpVect A = cpBodyLocalToWorld(body, cpSegmentShapeGetA(shape)) - render::offset;
        cpVect B = cpBodyLocalToWorld(body, cpSegmentShapeGetB(shape)) - render::offset;

        int radius = cpSegmentShapeGetRadius(shape);

        C2D_DrawCircleSolid(A.x, A.y, .5, radius, color);
        C2D_DrawCircleSolid(B.x, B.y, .5, radius, color);

        C2D_DrawLine(A.x, A.y, color, B.x, B.y, color, cpSegmentShapeGetRadius(shape)*2, .5);
    }
    else if (type == CP_POLY_SHAPE) 
    {
        render::polyUserData *polyData = (render::polyUserData *)data;

        for (std::size_t i=0; i < polyData->poly.size(); i+=6) {
            cpVect a = cpBodyLocalToWorld(body, cpv(polyData->poly.at(i), polyData->poly.at(i+1))) - render::offset,
                   b = cpBodyLocalToWorld(body, cpv(polyData->poly.at(i+2), polyData->poly.at(i+3))) - render::offset,
                   c = cpBodyLocalToWorld(body, cpv(polyData->poly.at(i+4), polyData->poly.at(i+5))) - render::offset;

            C2D_DrawTriangle(
                a.x,
                a.y,
                color,
                b.x,
                b.y,
                color,
                c.x,
                c.y,
                color,
                .5
            );
        }
    }
}

void render::setUp()
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    render::bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

void render::renderFrame(float deltaTime)
{   
    cpVect target = physics::ball->p-cpv(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    render::offset = render::offset+(target-render::offset)*.3;
    
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(render::bottom, bgcolor);
    C2D_SceneBegin(render::bottom);

    cpSpaceEachShape(physics::space, render::drawShape, NULL); 

    C3D_FrameEnd(0);
}