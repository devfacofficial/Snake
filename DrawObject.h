#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

typedef struct DrawObject
{
    HANDLE Output;
    CONSOLE_SCREEN_BUFFER_INFO Info;
    bool (*PutChar)(struct DrawObject* this, int32_t x, int32_t y, int8_t ch);
    bool (*PutStr)(struct DrawObject* this, int32_t x, int32_t y, const char* str);
    bool (*GetCenter)(struct DrawObject* this, int32_t* centerX, int32_t* centerY);
} DrawObject;

DrawObject* DrawObject_New(void);
void DrawObject_Delete(DrawObject* this);
bool DrawObject_PutChar(DrawObject* this, int32_t x, int32_t y, int8_t ch);
bool DrawObject_PutStr(DrawObject* this, int32_t x, int32_t y, const char* str);
bool DrawObject_GetCenter(DrawObject* this, int32_t* centerX, int32_t* centerY);

#endif