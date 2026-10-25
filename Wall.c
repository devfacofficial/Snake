#include "Wall.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
//! @brief   コンストラクタ
//! @details 壁オブジェクト生成
//! @return  壁オブジェクト / 生成失敗時NULLを返す
//!
Wall* Wall_New(
    DrawObject* drawObj,                //!< [I]thisポインタ
    int32_t     width,                  //!< [I]幅
    int32_t     height)                 //!< [I]高さ
{
    if(drawObj == NULL)
    {
        return NULL;
    }

    Wall* this = (Wall*)malloc(sizeof(Wall));
    if (this == NULL )
    {
        return NULL;
    }

    GameObj_Init(&this->Base);

    // 壁の幅/高さを設定　フロアを囲うため、+2している
    this->Width       = width + 2;
    this->Height      = height + 2;
    this->DrawObj     = drawObj;
    this->Base.Draw   = Wall_Draw;
    this->Base.Delete = Wall_Delete;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details 壁オブジェクト解放
//! @return  無し
//!
void Wall_Delete(
    GameObj* obj)                       //!< [I]親オブジェクトポインタ
{
    Wall* this = (Wall*)obj;
    if (this == NULL)
    {
        return;
    }
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   壁表示
//! @details １行開けて、Width / Height 範囲で壁を描画する
//! @return  無し
//!
void Wall_Draw(
    GameObj* obj)                       //!< [I]親オブジェクトポインタ
{
    Wall* this = (Wall*)obj;
    if ( (this == NULL) || 
         (this->DrawObj == NULL) || 
         (this->DrawObj->Output == INVALID_HANDLE_VALUE) )
    {
        return;
    }

    // 画面をクリアする
    if (system("cls") != 0)
    {
        fputs("Failed to execute cls.\n", stderr);
        return;
    }

    // 壁を描画する
    // 先頭行はスコア描画のため、１行目から描画する
    for (int32_t y = 1; y < (this->Height+1); y++)
    {
        for (int32_t x = 0; x < this->Width; x++)
        {
            if ( (y == 1) || (y == this->Height    ) ||
                 (x == 0) || (x == this->Width  - 1)    )
            {
                this->DrawObj->PutChar(this->DrawObj, x, y, '#');
            }
            else
            {
                this->DrawObj->PutChar(this->DrawObj, x, y, ' ');
            }   
        }
    }
}