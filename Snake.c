#include "Snake.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

//-----------------------------------------------------------------------------
//! @brief   コンストラクタ
//! @details へびオブジェクト生成
//! @return  へびオブジェクト / 生成失敗時NULLを返す
//!
Snake* Snake_New(
    DrawObject* drawObj,                //!< [I]描画オブジェクト
    int32_t     width,                  //!< [I]幅
    int32_t     height,                 //!< [I]高さ
    int32_t     ofstx,                  //!< [I]へび位置のオフセットX
    int32_t     ofsty)                  //!< [I]へび位置のオフセットY
{
    if (drawObj == NULL || width <= 0 || height <= 0)
    {
        return NULL;
    }

    Snake* this = (Snake*)malloc(sizeof(Snake));
    if (this == NULL )
    {
        return NULL;
    }

    this->Body = PosArray_New();
    if (this->Body == NULL)
    {
        free(this);
        return NULL;
    }

    this->Width     = width;
    this->Height    = height;
    this->OfstX     = ofstx;
    this->OfstY     = ofsty;
    this->X         = -1;
    this->Y         = -1;
    this->DrawObj   = drawObj;
    this->Move      = Snake_Move;
    this->Draw      = Snake_Draw;

    // へびの初期状態を設定
    int32_t x = (this->Width  / 2) + this->OfstX;  
    int32_t y = (this->Height / 2) + this->OfstY;
    Snake_AddBody(this, x, y);
    x--;
    Snake_AddBody(this, x, y);
    x--;
    Snake_AddBody(this, x, y);
    x--;
    Snake_AddBody(this, x, y);
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details へびオブジェクト解放
//! @return  無し
//!
void Snake_Delete(
    Snake* this)
{
    if (this == NULL)
    {
        return;
    }

    PosArray_Delete(this->Body);
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   へびの胴体を末尾に追加
//! @return  true:追加成功 false:追加失敗
//!
bool Snake_AddBody(
    Snake* this,
    int32_t x,
    int32_t y)
{
    if (this == NULL || this->Body == NULL)
    {
        return false;
    }

    Pos position = { x, y };
    return this->Body->Push(this->Body, &position);
}

//-----------------------------------------------------------------------------
//! @brief   へび登場
//! @details へびを頭の方角に移動
//! @return  無し
//!
void Snake_Move(
    Snake* this)                         //!< [I]thisポインタ
{
    (void)this;
}

//-----------------------------------------------------------------------------
//! @brief   へび表示
//! @details X,Y座標にフードを表示する
//! @return  無し
//!
void Snake_Draw(
    Snake* this)                         //!< [I]thisポインタ
{
    bool isTop = true;
    if ( (this == NULL) || 
         (this->DrawObj == NULL) || 
         (this->DrawObj->Output == INVALID_HANDLE_VALUE) )
    {
        return;
    }

    PosArrayIterator iterator = PosArray_Iterator(this->Body);
    const Pos* position;
    while ((position = iterator.Next(&iterator)) != NULL)
    {
        if(isTop == true)
        {
            this->DrawObj->PutChar(this->DrawObj, position->X, position->Y, '@');
            isTop = false;
        }
        else
        {
            this->DrawObj->PutChar(this->DrawObj, position->X, position->Y, '+');
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   フード消失
//! @details フードを消す
//! @return  無し
//!
void Snake_Disappear(
    Snake* this)                         //!< [I]thisポインタ
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
