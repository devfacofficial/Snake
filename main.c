#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include "DrawObject.h"
#include "Wall.h"
#include "Food.h"
#include "Snake.h"

void ShowErrorMsg(uint32_t line, const int8_t* func);

//-----------------------------------------------------------------------------
//! @brief   エラーメッセージ表示
//! @details 標準エラー出力にエラーメッセージを表示する
//! @return  無し
//!
void ShowErrorMsg(
    uint32_t       line,                //!< [I]行番号(__LINE__)
    const int8_t* func)                 //!< [I]関数名(__func__)
{
    int8_t msg[256];
    snprintf((char*)msg, sizeof(msg), "Error: %s() failed at line %d.\n", func, line);
    fputs((char*)msg, stderr);
}

//-----------------------------------------------------------------------------
//! @brief   メイン処理
//! @details スネークゲームを実行する
//! @return  0:正常終了  0以外:異常終了
//!
int main(void)
{
    DrawObject* drawObject = NULL;
    Wall* wall = NULL;
    Food* food = NULL;
    Snake* snake = NULL;

    // 初期化
    drawObject = DrawObject_New();
    if(drawObject == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    wall = Wall_New(drawObject,20,20);
    if(wall == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    food = Food_New(drawObject,20,20, 1, 2);
    if(food == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    snake = Snake_New(drawObject, 20, 20, 1, 2);
    if(food == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    // ゲーム処理
    while(1)
    {
        // 壁表示
        wall->Show(wall);

        // えさ表示
        food->Arrival(food);
        food->Draw(food);

        // へび初期化
        snake->Init(snake);

        while(snake->IsCollisionWall == false)
        {
            // へび移動
            snake->Move(snake);

            // 障害物または体に衝突
            if(snake->IsCollisionWall == true)
            {
                drawObject->PutStr(drawObject,1, 9,"                    ");
                drawObject->PutStr(drawObject,1,10,"     GAME OVER!     ");
                drawObject->PutStr(drawObject,1,11,"    HIT ANY KEY!    ");
                drawObject->PutStr(drawObject,1,12,"                    ");
                while(_getch())
                {
                    break;
                }
            }
            else
            {
                Sleep(250);
            }
        }
    }
    
END:
    if(wall != NULL)Wall_Delete(wall);
    if(drawObject != NULL)DrawObject_Delete(drawObject);
    return 0;
}
