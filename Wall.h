#ifndef WALL_H
#define WALL_H

#include <stdint.h>
#include "DrawObject.h"

//-----------------------------------------------------------------------------
//! @brief   壁
//! @details 壁を描画する
//!
typedef struct Wall
{
    int32_t Width;          //!< 画面幅
    int32_t Height;         //!< 画面高さ
    DrawObject* DrawObj;    //!< 描画オブジェクト
    void (*Show)(struct Wall* this);    //!< 表示
} Wall;

Wall* Wall_New(DrawObject* drawObj,int32_t width, int32_t height);
void  Wall_Delete(Wall* this);
void  Wall_Show(Wall* this);

#endif
