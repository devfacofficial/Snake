#ifndef FOOD_H
#define FOOD_H

#include <stdint.h>
#include "DrawObject.h"

//-----------------------------------------------------------------------------
//! @brief   エサ
//! @details エサの生成／消去を行う
//!
typedef struct Food
{
    int32_t Width;          //!< 画面幅
    int32_t Height;         //!< 画面高さ
    int32_t X;              //!< エサX
    int32_t Y;              //!< エサY
    int32_t OfstX;          //!< エサ描画位置オフセットX
    int32_t OfstY;          //!< エサ描画位置オフセットY
    DrawObject* DrawObj;    //!< 描画オブジェクト
    void (*Arrival)(struct Food* this);     //!< エサ登場
    void (*Draw)(struct Food* this);        //!< エサ表示
    void (*Disappear)(struct Food* this);   //!< エサ消失
} Food;

Food* Food_New(DrawObject* drawObj, int32_t width, int32_t height, int32_t ofstx, int32_t ofsty);
void  Food_Delete(Food* this);
void  Food_Arrival(Food* this);
void  Food_Draw(Food* this);
void  Food_Disappear(Food* this);

#endif
