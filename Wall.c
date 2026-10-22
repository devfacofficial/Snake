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
    DrawObject* drawObj)
{
    Wall* this = (Wall*)malloc(sizeof(Wall));
    if (this == NULL || drawObj == NULL)
    {
        return NULL;
    }

    this->Width   = 22;
    this->Height  = 22;
    this->DrawObj = drawObj;
    this->Show = Wall_Show;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   デストラクタ
//! @details 壁オブジェクト解放
//! @return  無し
//!
void Wall_Delete(
    Wall* this)
{
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
void Wall_Show(Wall* this)
{
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
 