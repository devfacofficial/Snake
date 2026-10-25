#include "Food.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-----------------------------------------------------------------------------
//! @brief   コンストラクタ
//! @details エサオブジェクト生成
//! @return  エサオブジェクト / 生成失敗時NULLを返す
//!
Food* Food_New(
    DrawObject* drawObj,                //!< [I]描画オブジェクト
    int32_t     width,                  //!< [I]幅
    int32_t     height,                 //!< [I]高さ
    int32_t     ofstx,                  //!< [I]エサ位置のオフセットX
    int32_t     ofsty)                  //!< [I]エサ位置のオフセットY
{
    if (drawObj == NULL || width <= 0 || height <= 0)
    {
        return NULL;
    }

    Food* this = (Food*)malloc(sizeof(Food));
    if (this == NULL )
    {
        return NULL;
    }

    GameObj_Init(&this->Base);
    this->Width       = width;
    this->Height      = height;
    this->OfstX       = ofstx;
    this->OfstY       = ofsty;
    this->X           = -1;
    this->Y           = -1;
    this->DrawObj     = drawObj;
    this->Arrival     = Food_Arrival;
    this->Base.Draw   = Food_Draw;
    this->Base.Delete = Food_Delete;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details エサオブジェクト解放
//! @return  無し
//!
void Food_Delete(
    GameObj* obj)                       //!< [I]親オブジェクト
{
    Food* this = (Food*)obj;
    if (this == NULL)
    {
        return;
    }
    free(this);
}


//-----------------------------------------------------------------------------
//! @brief   エサ登場
//! @details エサをランダムな位置に設定する
//! @return  無し
//!
void Food_Arrival(
    GameObj* obj)                       //!< [I]親オブジェクトポインタ
{
    Food* this = (Food*)obj;

    if(this == NULL)
    {
        return;
    }

    static int randomSeeded = 0;
    if (!randomSeeded)
    {
        srand((unsigned)time(NULL));
        randomSeeded = 1;
    }

    // 壁とスコアの表示を考慮し、X+1, Y+2 する
    this->X = (rand() % this->Width)  + this->OfstX;
    this->Y = (rand() % this->Height) + this->OfstY;
}

//-----------------------------------------------------------------------------
//! @brief   エサ表示
//! @details X,Y座標にエサを表示する
//! @return  無し
//!
void Food_Draw(
    GameObj* obj)                       //!< [I]親オブジェクトポインタ
{
    Food* this = (Food*)obj;
    if ( (this == NULL) || 
         (this->DrawObj == NULL) || 
         (this->DrawObj->Output == INVALID_HANDLE_VALUE) )
    {
        return;
    }

    // エサ座標未設定のため表示しない
    if( (this->X == -1) || (this->Y == -1) )
    {
        return;
    }

    // エサ表示
    this->DrawObj->PutChar(this->DrawObj, this->X, this->Y, '$');
}
