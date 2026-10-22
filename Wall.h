#ifndef WALL_H
#define WALL_H

#include <stdint.h>
#include "DrawObject.h"

typedef struct Wall
{
    int32_t Width;
    int32_t Height;
    DrawObject* DrawObj;
    void (*Show)(struct Wall* this);
} Wall;

Wall* Wall_New(DrawObject* drawObj,int32_t width, int32_t height);
void  Wall_Delete(Wall* this);
void  Wall_Show(Wall* this);

#endif
