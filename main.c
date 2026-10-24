#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include "DrawObject.h"
#include "Wall.h"
#include "Food.h"
#include "Snake.h"

#define SCREEN_WIDTH  40
#define SCREEN_HEIGHT 20

DrawObject* drawObject = NULL;
Wall* wall             = NULL;
Food* food             = NULL;
Snake* snake           = NULL;

void ShowErrorMsg(uint32_t line, const int8_t* func);
void PlayGame();
void ShowGameoverMsg();

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
//! @brief   ゲーム処理
//! @details ゲームを実行する
//! @return  無し
//!
void PlayGame()
{
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
                ShowGameoverMsg();
            }
            else
            {
                Sleep(250);
            }
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   ゲーム終了メッセージ表示
//! @details キーが入力されるまで待機
//! @return  無し
//!
void ShowGameoverMsg()
{
    char msg[SCREEN_WIDTH];
    int32_t x;

    strcpy(msg,"GAME OVER!");
    x = ( (SCREEN_WIDTH - strlen(msg)) / 2) + 1;
    drawObject->PutStr(drawObject,x,10,msg);

    strcpy(msg,"HIT ANY KEY!");
    x = ( (SCREEN_WIDTH - strlen(msg)) / 2) + 1;
    drawObject->PutStr(drawObject,x,11,msg);

    while(_getch())
    {
        break;
    }
}

//-----------------------------------------------------------------------------
//! @brief   メイン処理
//! @details スネークゲームを実行する
//! @return  0:正常終了  0以外:異常終了
//!
int main(void)
{
    // 初期化
    drawObject = DrawObject_New();
    if(drawObject == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    wall = Wall_New(drawObject,SCREEN_WIDTH,SCREEN_HEIGHT);
    if(wall == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    food = Food_New(drawObject, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 2);
    if(food == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    snake = Snake_New(drawObject, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 2);
    if(food == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }
    
    // ゲーム処理
    PlayGame();

END:
    if(wall != NULL)Wall_Delete(wall);
    if(drawObject != NULL)DrawObject_Delete(drawObject);
    return 0;
}
