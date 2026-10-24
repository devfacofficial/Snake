#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

//-----------------------------------------------------------------------------
//! @brief   描画オブジェクト
//! @details 文字を描画する
//!
typedef struct DrawObject
{
    HANDLE Output;                      //!< 標準入力ハンドラ
    CONSOLE_SCREEN_BUFFER_INFO Info;    //!< コンソール情報
    CONSOLE_CURSOR_INFO CursorInfo;     //!< カーソル情報
    bool IsCursorHidden;                //!< カーソル非表示中
    bool (*PutChar)(struct DrawObject* this, int32_t x, int32_t y, int8_t ch);          //!< 指定位置に1文字描画する
    bool (*PutStr)(struct DrawObject* this, int32_t x, int32_t y, const char* str);     //!< 指定位置に文字列を描画する
    bool (*GetCenter)(struct DrawObject* this, int32_t* centerX, int32_t* centerY);     //!< 表示領域の中心座標を取得する
    bool (*HideCursor)(struct DrawObject* this);                                        //!< カーソルを非表示にする
    bool (*RestoreCursor)(struct DrawObject* this);                                     //!< カーソルを非表示にする前の状態に戻す
} DrawObject;

DrawObject* DrawObject_New(void);
void DrawObject_Delete(DrawObject* this);
bool DrawObject_PutChar(DrawObject* this, int32_t x, int32_t y, int8_t ch);
bool DrawObject_PutStr(DrawObject* this, int32_t x, int32_t y, const char* str);
bool DrawObject_GetCenter(DrawObject* this, int32_t* centerX, int32_t* centerY);
bool DrawObject_HideCursor(DrawObject* this);
bool DrawObject_RestoreCursor(DrawObject* this);

#endif