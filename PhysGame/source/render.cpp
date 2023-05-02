#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>

#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk.h>

#include <tuple>
#include <vector>

#include "render.hpp"
#include "Del.hpp"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

using Coord = double;
using Point = std::vector<Coord>;
using N = uint32_t;

using namespace render;

void render::drawShape(cpShape *shape, void *data) {
    cpBody *body = cpShapeGetBody(shape);
    u32 color = C2D_Color32f(0,0,0,1);
    u32 bgcolor = C2D_Color32f(1,1,1,1);

    cpShapeType type = shape->klass->type;

    if (type == CP_CIRCLE_SHAPE) {
        cpCircleShape* circle = (cpCircleShape *)shape;
        cpVect pos = cpBodyLocalToWorld(body, circle->c);
        cpVect lineEnd = cpvadd(cpBodyLocalToWorld(body, cpv(0, cpCircleShapeGetRadius(shape))), cpvsub(pos, body->p));
        C2D_DrawCircleSolid(body->p.x, body->p.y, .5, cpCircleShapeGetRadius(shape), C2D_Color32f(0,0,0,1));
        C2D_DrawLine(pos.x, pos.y, color, lineEnd.x, lineEnd.y, bgcolor, 1, .5);
    }
    else if (type == CP_SEGMENT_SHAPE) 
    {
        cpVect A = cpBodyLocalToWorld(body, cpSegmentShapeGetA(shape));
        cpVect B = cpBodyLocalToWorld(body, cpSegmentShapeGetB(shape));
        C2D_DrawLine(body->p.x+A.x, body->p.y+A.y, color, body->p.x+B.x, body->p.x+B.y, color, cpSegmentShapeGetRadius(shape), .5);
    }
    else if (type == CP_POLY_SHAPE) 
    {
        //return;
        
        std::vector<double> coords = {};

        int vertCount = cpPolyShapeGetCount(shape);
        
        for(int i=0; i<vertCount; i++)
        {
            cpVect point = cpBodyLocalToWorld(body, cpPolyShapeGetVert(shape, i));
            coords.push_back(point.x);
            coords.push_back(point.y);
        }

        delaunator::Delaunator d(coords);

        for(std::size_t i = 0; i < d.triangles.size(); i+=3) {
            C2D_DrawTriangle(d.coords[2 * d.triangles[i]],        //tx0
                d.coords[2 * d.triangles[i] + 1],    //ty0
                color,
                d.coords[2 * d.triangles[i + 1]],    //tx1
                d.coords[2 * d.triangles[i + 1] + 1],//ty1
                color,
                d.coords[2 * d.triangles[i + 2]],    //tx2
                d.coords[2 * d.triangles[i + 2] + 1], //ty2
                color,
                .5
            );
        }
        
    }
}