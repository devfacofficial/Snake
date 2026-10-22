#include <stdint.h>
#include <stdlib.h>
#include <windows.h>
#include "DrawObject.h"

//-----------------------------------------------------------------------------
//! @brief   描画オブジェクトを作成する
//! @return  成功時はオブジェクトのポインタ、失敗時はNULL
//!
DrawObject* DrawObject_New(void)
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE || output == NULL)
    {
        return NULL;
    }

    DrawObject* drawObject = malloc(sizeof *drawObject);
    if (drawObject == NULL)
    {
        return NULL;
    }

    if (!GetConsoleScreenBufferInfo(output, &drawObject->Info))
    {
        free(drawObject);
        return NULL;
    }

    drawObject->Output = output;
    drawObject->PutChar = DrawObject_PutChar;
    drawObject->GetCenter = DrawObject_GetCenter;
    return drawObject;
}

//-----------------------------------------------------------------------------
//! @brief   描画オブジェクトを解放する
//!
void DrawObject_Delete(DrawObject* this)
{
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   指定位置に1文字描画する
//! @details 表示領域外の座標は描画せず、APIエラー時はfalseを返す
//! @return  成功時true
//!
bool DrawObject_PutChar(
    DrawObject* this,
    int32_t x,
    int32_t y,
    int8_t ch)
{
    if (this == NULL)
    {
        return false;
    }

    if (x < this->Info.srWindow.Left ||
        x > this->Info.srWindow.Right ||
        y < this->Info.srWindow.Top ||
        y > this->Info.srWindow.Bottom)
    {
        return true;
    }

    COORD position = { (SHORT)x, (SHORT)y };
    if (!SetConsoleCursorPosition(this->Output, position))
    {
        return false;
    }

    DWORD written = 0;
    return WriteConsoleA(this->Output, &ch, 1, &written, NULL) && (written == 1);
}

//-----------------------------------------------------------------------------
//! @brief   表示領域の中心座標を取得する
//! @return  成功時true
//!
bool DrawObject_GetCenter(
    DrawObject* this,
    int32_t* centerX,
    int32_t* centerY)
{
    if (this == NULL || centerX == NULL || centerY == NULL)
    {
        return false;
    }

    *centerX = this->Info.srWindow.Left +
        (this->Info.srWindow.Right -
         this->Info.srWindow.Left + 1) / 2;
    *centerY = this->Info.srWindow.Top +
        (this->Info.srWindow.Bottom -
         this->Info.srWindow.Top + 1) / 2;
    return true;
}
