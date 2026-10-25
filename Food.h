#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>
#include "DrawObject.h"
#include "GameObj.h"

//-----------------------------------------------------------------------------
//! @brief   エサ
//! @details エサの生成／消去を行う
//!
typedef struct Food
{
    GameObj Base;           //!< 親オブジェクト
    int32_t Width;          //!< 画面幅
    int32_t Height;         //!< 画面高さ
    int32_t X;              //!< エサX
    int32_t Y;              //!< エサY
    int32_t OfstX;          //!< エサ描画位置オフセットX
    int32_t OfstY;          //!< エサ描画位置オフセットY
    DrawObject* DrawObj;    //!< 描画オブジェクト
} Food;

Food* Food_New(DrawObject* drawObj, int32_t width, int32_t height, int32_t ofstx, int32_t ofsty);
void  Food_Delete(GameObj* obj);
void  Food_Update(GameObj* obj);
void  Food_Draw(GameObj* obj);

#endif
