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

    this->Width           = width;
    this->Height          = height;
    this->OfstX           = ofstx;
    this->OfstY           = ofsty;
    this->X               = -1;
    this->Y               = -1;
    this->Direction       = RIGHT;
    this->IsCollisionFood = false;
    this->IsCollisionWall = false;
    this->DrawObj         = drawObj;
    this->Move            = Snake_Move;

    // へびの初期状態を設定
    int32_t x = 4 + this->OfstX;
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
    bool isTop = true;
    PosArrayIterator iterator = PosArray_Iterator(this->Body);
    Pos* position;
    Pos  bfPosition;
    Pos  nowPosition;

    // 衝突判定
    if( Snake_Collision(this) == true)
    {
        return;
    }

    while ((position = iterator.Next(&iterator)) != NULL)
    {
        if(isTop == true)
        {
            bfPosition  = *position;
            switch(this->Direction)
            {
            case RIGHT:
                position->X += 1;
                break;
            case LEFT:
                position->X -= 1;
                break;
            case UP:
                position->Y += 1;
                break;
            case DOWN:
                position->Y -= 1;
                break;
            default:
            }

            this->DrawObj->PutChar(this->DrawObj, position->X, position->Y, '@');
            isTop = false;
        }
        else
        {
            nowPosition = *position;
            *position   = bfPosition;
            bfPosition  = nowPosition;
            this->DrawObj->PutChar(this->DrawObj, position->X, position->Y, '+');
        }
    }
    this->DrawObj->PutChar(this->DrawObj, bfPosition.X, bfPosition.Y, ' ');
}

//-----------------------------------------------------------------------------
//! @brief   衝突判定
//! @details 壁、体との衝突を判定する
//! @return  無し
//!
bool Snake_Collision(Snake* this)
{
    bool isTop = true;
    PosArrayIterator iterator = PosArray_Iterator(this->Body);
    Pos* position;
    Pos  headPosition;

    while ((position = iterator.Next(&iterator)) != NULL)
    {
        if(this->IsCollisionWall == true)
        {
            break;
        }

        // 壁衝突判定
        if(isTop == true)
        {
            headPosition = *position;
            switch(this->Direction)
            {
            case RIGHT:
                headPosition.X += 1;
                if(headPosition.X >= (this->Width + this->OfstX - 1))
                {
                    this->IsCollisionWall = true;
                }
                break;
            case LEFT:
                headPosition.X -= 1;
                if(headPosition.X <= this->OfstX)
                {
                    this->IsCollisionWall = true;
                }
                break;
            case UP:
                headPosition.Y += 1;
                if(headPosition.Y >= (this->Height + this->OfstY - 1))
                {
                    this->IsCollisionWall = true;
                }
                break;
            case DOWN:
                headPosition.Y -= 1;
                if(headPosition.Y <= this->OfstY)
                {
                    this->IsCollisionWall = true;
                }
                break;
            }
        }
        // 体衝突判定
        else
        {
            if( (position->X == headPosition.X) && (position->Y == headPosition.Y) )
            {
                this->IsCollisionWall = true;
            }
        }
    }
    return this->IsCollisionWall;
}