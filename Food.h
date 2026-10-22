#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>
#include "DrawObject.h"

typedef struct Food
{
    int32_t Width;
    int32_t Height;
    int32_t X;
    int32_t Y;
    DrawObject* DrawObj;
    void (*Arrival)(struct Food* this);
    void (*Draw)(struct Food* this);
    void (*Disappear)(struct Food* this);
} Food;

Food* Food_New(DrawObject* drawObj, int32_t width, int32_t height);
void  Food_Delete(Food* this);
void  Food_Arrival(Food* this);
void  Food_Draw(Food* this);
void  Food_Disappear(Food* this);

#endif
