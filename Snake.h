#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stdint.h>
#include "Array.h"
#include "DrawObject.h"

//-----------------------------------------------------------------------------
//! @brief   座標
//! @details 座標を保持する
//!
typedef struct Pos
{
    int32_t X;                          //!< X座標
    int32_t Y;                          //!< Y座標
}Pos;
ARRAY(Pos);

//-----------------------------------------------------------------------------
//! @brief   ヘビの進行方向
//! @details ヘビの進行方向を示す
//!
typedef enum SNAKE_DIRECTION
{
    UP    = 8,                          //!< 上
    DOWN  = 2,                          //!< 下
    LEFT  = 4,                          //!< 左
    RIGHT = 6                           //!< 右
} SNAKE_DIRECTION;

//-----------------------------------------------------------------------------
//! @brief   ヘビ
//! @details ヘビの生成／消去を行う
//!
typedef struct Snake
{
    int32_t         Width;              //!< 画面幅
    int32_t         Height;             //!< 画面高さ
    int32_t         OfstX;              //!< ヘビ描画位置オフセットX
    int32_t         OfstY;              //!< ヘビ描画位置オフセットY
    SNAKE_DIRECTION Direction;          //!< ヘビ進行方向
    bool            IsCollisionWall;    //!< 壁衝突
    DrawObject*     DrawObj;            //!< 描画オブジェクト
    PosArray*       Body;               //!< ヘビ本体
    void (*Init)(struct Snake* this);                           //!< 初期化
    void (*Move)(struct Snake* this);                           //!< 移動
    bool (*AddBody)(struct Snake* this, int32_t x, int32_t y);  //!< ヘビ本体追加
} Snake;

Snake* Snake_New(DrawObject* drawObj, int32_t width, int32_t height, int32_t ofstx, int32_t ofsty);
void   Snake_Delete(Snake* this);
void   Snake_Init(Snake* this);
void   Snake_Move(Snake* this);
void   Snake_ChangeDirection(Snake* this);
bool   Snake_Collision(Snake* this);
bool   Snake_AddBody(Snake* this, int32_t x, int32_t y);
bool   Snake_GetNextHeadPos(Snake* this, Pos* pNextHead);
#endif
