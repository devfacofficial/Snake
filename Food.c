#include "Food.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-----------------------------------------------------------------------------
//! @brief   コンストラクタ
//! @details えさオブジェクト生成
//! @return  えさオブジェクト / 生成失敗時NULLを返す
//!
Food* Food_New(
    DrawObject* drawObj,                //!< [I]描画オブジェクト
    int32_t     width,                  //!< [I]幅
    int32_t     height,                 //!< [I]高さ
    int32_t     ofstx,                  //!< [I]えさ位置のオフセットX
    int32_t     ofsty)                  //!< [I]えさ位置のオフセットY
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

    this->Width     = width;
    this->Height    = height;
    this->OfstX     = ofstx;
    this->OfstY     = ofsty;
    this->X         = -1;
    this->Y         = -1;
    this->DrawObj   = drawObj;
    this->Arrival   = Food_Arrival;
    this->Draw      = Food_Draw;
    this->Disappear = Food_Disappear;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details えさオブジェクト解放
//! @return  無し
//!
void Food_Delete(
    Food* this)
{
    if (this == NULL)
    {
        return;
    }
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   フード登場
//! @details フードをランダムな位置に設定する
//! @return  無し
//!
void Food_Arrival(
    Food* this)                         //!< [I]thisポインタ
{
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
//! @brief   フード表示
//! @details X,Y座標にフードを表示する
//! @return  無し
//!
void Food_Draw(
    Food* this)                         //!< [I]thisポインタ
{
    if ( (this == NULL) || 
         (this->DrawObj == NULL) || 
         (this->DrawObj->Output == INVALID_HANDLE_VALUE) )
    {
        return;
    }

    // えさ座標未設定のため表示しない
    if( (this->X == -1) || (this->Y == -1) )
    {
        return;
    }

    // えさ表示
    this->DrawObj->PutChar(this->DrawObj, this->X, this->Y, '$');
}

//-----------------------------------------------------------------------------
//! @brief   フード消失
//! @details フードを消す
//! @return  無し
//!
void Food_Disappear(
    Food* this)                         //!< [I]thisポインタ
{
    if ( (this == NULL) || 
         (this->DrawObj == NULL) || 
         (this->DrawObj->Output == INVALID_HANDLE_VALUE) )
    {
        return;
    }

    // えさ消去
    this->DrawObj->PutChar(this->DrawObj, this->X, this->Y, ' ');
    this->X = -1;
    this->Y = -1;
}
