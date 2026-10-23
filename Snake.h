#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stdint.h>
#include "Array.h"
#include "DrawObject.h"

typedef struct Pos
{
    int32_t X;
    int32_t Y;
}Pos;
ARRAY(Pos);

typedef enum SNAKE_DIRECTION
{
    UP    = 8,
    DOWN  = 2,
    LEFT  = 4,
    RIGHT = 6
} SNAKE_DIRECTION;

typedef struct Snake
{
    int32_t         Width;
    int32_t         Height;
    int32_t         X;
    int32_t         Y;
    int32_t         OfstX;
    int32_t         OfstY;
    SNAKE_DIRECTION Direction;
    bool            IsCollisionWall;
    bool            IsCollisionFood;
    DrawObject*     DrawObj;
    PosArray*       Body;
    void (*Move)(struct Snake* this);
} Snake;

Snake* Snake_New(DrawObject* drawObj, int32_t width, int32_t height, int32_t ofstx, int32_t ofsty);
void   Snake_Delete(Snake* this);
void   Snake_Move(Snake* this);
void   Snake_ChangeDirection(Snake* this);
bool   Snake_Collision(Snake* this);
bool   Snake_AddBody(Snake* this, int32_t x, int32_t y);
bool   Snake_GetNextHeadPos(Snake* this, Pos* pNextHead);
#endif
