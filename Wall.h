#ifndef WALL_H
#define WALL_H

#include <stdint.h>
#include "DrawObject.h"
#include "GameObj.h"

//-----------------------------------------------------------------------------
//! @brief   壁
//! @details 壁を描画する
//!
typedef struct Wall
{
    GameObj Base;           //!< 親オブジェクト
    int32_t Width;          //!< 画面幅
    int32_t Height;         //!< 画面高さ
    DrawObject* DrawObj;    //!< 描画オブジェクト
} Wall;

Wall* Wall_New(DrawObject* drawObj,int32_t width, int32_t height);
void  Wall_Delete(GameObj* obj);
void  Wall_Draw(GameObj* obj);

#endif
