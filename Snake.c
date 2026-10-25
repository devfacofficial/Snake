#include "Snake.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

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

    this->Width           = width;
    this->Height          = height;
    this->OfstX           = ofstx;
    this->OfstY           = ofsty;
    this->Direction       = RIGHT;
    this->IsCollisionWall = false;
    this->DrawObj         = drawObj;
    this->Body            = NULL;
    this->Init            = Snake_Init;
    this->Move            = Snake_Move;
    this->AddBody         = Snake_AddBody;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details へびオブジェクト解放
//! @return  無し
//!
void Snake_Delete(
    Snake* this)                        //!< [I]thisポインタ
{
    if (this == NULL)
    {
        return;
    }

    PosArray_Delete(this->Body);
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   初期化
//! @details 無し
//! @return  無し
//!
void Snake_Init(
    Snake* this)                        //!< [I]thisポインタ
{
    if (this == NULL)
    {
        return;
    }

    if(this->Body != NULL)
    {
        PosArray_Delete(this->Body);
        this->Body = NULL;
    }

    this->Body = PosArray_New();
    if (this->Body == NULL)
    {
        return;
    }

    this->Direction       = RIGHT;
    this->IsCollisionWall = false;

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
}

//-----------------------------------------------------------------------------
//! @brief   へびの胴体を末尾に追加
//! @details えさを食べたときは、x=-1, y=-1 を設定すること
//! @return  true:追加成功 false:追加失敗
//!
bool Snake_AddBody(
    Snake* this,                        //!< [I]thisポインタ
    int32_t x,                          //!< [I]追加位置X
    int32_t y)                          //!< [I]追加位置Y
{
    if (this == NULL || this->Body == NULL)
    {
        return false;
    }

    Pos position = { x, y };
    return this->Body->Push(this->Body, &position);
}

//-----------------------------------------------------------------------------
//! @brief   へび移動
//! @details へびを頭の方角に移動
//! @return  無し
//!
void Snake_Move(
    Snake* this)                         //!< [I]thisポインタ
{
    bool isTop = true;
    PosIterator iterator = PosArray_Iterator(this->Body);
    Pos* position;
    Pos  bfPosition;
    Pos  nowPosition;

    if (this == NULL)
    {
        return;
    }

    // 方向判定
    Snake_ChangeDirection(this);

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
            Snake_GetNextHeadPos(this, position);
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
    if( (bfPosition.X != -1) || (bfPosition.Y != -1) )
    {
        this->DrawObj->PutChar(this->DrawObj, bfPosition.X, bfPosition.Y, ' ');
    }
}

//-----------------------------------------------------------------------------
//! @brief   へび移動方向判定
//! @details キー入力に応じて方向を変更する
//! @return  無し
//!
void Snake_ChangeDirection(
    Snake* this)                        //!< [I]thisポインタ
{
    if (this == NULL || !_kbhit())
    {
        return;
    }

    int key = _getch();
    SNAKE_DIRECTION nextDirection;

    if (key == 0 || key == 0xE0)
    {
        switch (_getch())
        {
        case 72: nextDirection = UP;    break; // 上
        case 80: nextDirection = DOWN;  break; // 下
        case 75: nextDirection = LEFT;  break; // 左
        case 77: nextDirection = RIGHT; break; // 右
        default: return;
        }
    }
    else
    {
        switch (key)
        {
        case '8': nextDirection = UP;    break;
        case '2': nextDirection = DOWN;  break;
        case '4': nextDirection = LEFT;  break;
        case '6': nextDirection = RIGHT; break;
        default: return;
        }
    }

    // 進行方向と正反対には曲がれないようにする
    if ((this->Direction == UP    && nextDirection == DOWN)  ||
        (this->Direction == DOWN  && nextDirection == UP)    ||
        (this->Direction == LEFT  && nextDirection == RIGHT) ||
        (this->Direction == RIGHT && nextDirection == LEFT))
    {
        return;
    }

    this->Direction = nextDirection;
}

//-----------------------------------------------------------------------------
//! @brief   衝突判定
//! @details 壁、体との衝突を判定する
//! @return  無し
//!
bool Snake_Collision(
    Snake* this)                        //!< [I]thisポインタ
{
    bool isTop = true;
    PosIterator iterator = PosArray_Iterator(this->Body);
    Pos* position;
    Pos  headPosition;

    if (this == NULL)
    {
        return false;
    }

    Snake_GetNextHeadPos(this, &headPosition);
    while ((position = iterator.Next(&iterator)) != NULL)
    {
        if(this->IsCollisionWall == true)
        {
            break;
        }

        // 壁衝突判定
        if(isTop == true)
        {
            if( (headPosition.X > (this->Width + this->OfstX - 1))  ||
                (headPosition.X < (this->OfstX))                    ||
                (headPosition.Y > (this->Height + this->OfstY - 1)) ||
                (headPosition.Y < (this->OfstY))                       )
            {
                this->IsCollisionWall = true;
                break;
            }
            isTop = false;
        }
        // 体衝突判定
        else
        {
            if( (position->X == headPosition.X) && (position->Y == headPosition.Y) )
            {
                this->IsCollisionWall = true;
                break;
            }
        }
    }
    return this->IsCollisionWall;
}

//-----------------------------------------------------------------------------
//! @brief   移動後の頭の位置を取得
//! @details 無し
//! @return  true:成功  false:失敗
//!
bool Snake_GetNextHeadPos(
    Snake* this,                        //!< [I]thisポインタ
    Pos*   pNextHead)                   //!< [O]次回頭位置
{
    bool rslt = false;
    Pos nextHead = this->Body->Data[0];

    if( (this == NULL) || (pNextHead == NULL) )
    {
        return false;
    }

    switch(this->Direction)
    {
    case RIGHT:
        nextHead.X += 1;
        rslt = true;
        break;
    case LEFT:
        nextHead.X -= 1;
        rslt = true;
        break;
    case UP:
        nextHead.Y -= 1;
        rslt = true;
        break;
    case DOWN:
        nextHead.Y += 1;
        rslt = true;
        break;
    default:
        rslt = false;
    }

    if(rslt == true)
    {
        *pNextHead = nextHead;
    }
    return rslt;
}