#include <stdio.h>
#include <stdint.h>
#include "DrawObject.h"
#include "Wall.h"

void ShowErrorMsg(uint32_t line, const int8_t* func);

void ShowErrorMsg(
    uint32_t       line,                //!< [I]行番号(__LINE__)
    const int8_t* func)                 //!< [I]関数名(__func__)
{
    int8_t msg[256];
    snprintf((char*)msg, sizeof(msg), "Error: %s() failed at line %d.\n", func, line);
    fputs((char*)msg, stderr);
}

int main(void)
{
    DrawObject* drawObject = NULL;
    Wall* wall = NULL;

    // 初期化
    drawObject = DrawObject_New();
    if(drawObject == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    wall = Wall_New(drawObject);
    if(wall == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    // 壁表示
    wall->Show(wall);

END:
    return 0;
}