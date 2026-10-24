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

    DrawObject* this = malloc(sizeof *this);
    if (this == NULL)
    {
        return NULL;
    }

    if (!GetConsoleScreenBufferInfo(output, &this->Info))
    {
        free(this);
        return NULL;
    }

    this->IsCursorHidden = false;
    this->Output    = output;
    this->PutChar   = DrawObject_PutChar;
    this->PutStr    = DrawObject_PutStr;
    this->GetCenter = DrawObject_GetCenter;
    this->HideCursor = DrawObject_HideCursor;
    this->RestoreCursor = DrawObject_RestoreCursor;
    return this;
}

//-----------------------------------------------------------------------------
//! @brief   描画オブジェクトを解放する
//!
void DrawObject_Delete(DrawObject* this)
{
    if (this != NULL && this->IsCursorHidden)
    {
        DrawObject_RestoreCursor(this);
    }
    free(this);
}

//-----------------------------------------------------------------------------
//! @brief   カーソルを非表示にする
//! @return  成功時true
//!
bool DrawObject_HideCursor(DrawObject* this)
{
    if (this == NULL)
    {
        return false;
    }
    if (this->IsCursorHidden)
    {
        return true;
    }
    if (!GetConsoleCursorInfo(this->Output, &this->CursorInfo))
    {
        return false;
    }

    CONSOLE_CURSOR_INFO hiddenCursorInfo = this->CursorInfo;
    hiddenCursorInfo.bVisible = FALSE;
    if (!SetConsoleCursorInfo(this->Output, &hiddenCursorInfo))
    {
        return false;
    }

    this->IsCursorHidden = true;
    return true;
}

//-----------------------------------------------------------------------------
//! @brief   カーソルを非表示にする前の状態に戻す
//! @return  成功時true
//!
bool DrawObject_RestoreCursor(DrawObject* this)
{
    if (this == NULL)
    {
        return false;
    }
    if (!this->IsCursorHidden)
    {
        return true;
    }
    if (!SetConsoleCursorInfo(this->Output, &this->CursorInfo))
    {
        return false;
    }

    this->IsCursorHidden = false;
    return true;
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
//! @brief   指定位置に文字列を描画する
//! @details 無し
//! @return  成功時true
//!
bool DrawObject_PutStr(
    DrawObject* this,
    int32_t x,
    int32_t y,
    const char* str)
{
    if (this == NULL)
    {
        return false;
    }

    COORD position = { (SHORT)x, (SHORT)y };
    if (!SetConsoleCursorPosition(this->Output, position))
    {
        return false;
    }

    DWORD written = 0;
    DWORD length = (DWORD)strlen(str);
    return WriteConsoleA(this->Output, str, length, &written, NULL) && (written == 1);
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
