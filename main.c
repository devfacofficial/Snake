#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include "DrawObject.h"
#include "Wall.h"
#include "Food.h"
#include "Snake.h"

#define SCREEN_WIDTH  20
#define SCREEN_HEIGHT 10

DrawObject* drawObject = NULL;
Wall* wall             = NULL;
Food* food             = NULL;
Snake* snake           = NULL;
int32_t score          = 0;
bool stopRequested     = false;

void ShowErrorMsg(uint32_t line, const int8_t* func);
void PlayGame();
bool IsEat();
void ShowFood();
void ShowGameoverMsg();
void ShowScore();
BOOL WINAPI HandleConsoleControl(DWORD controlType);
bool IsStopRequested(void);

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
//! @brief   Ctrl+c 割り込みハンドラ
//! @details Ctrl+c 押下時、フラグをONにする
//! @return  true: 割り込み有り  false:割り込み無し
//!
BOOL WINAPI HandleConsoleControl(DWORD controlType)
{
    if (controlType == CTRL_C_EVENT || controlType == CTRL_BREAK_EVENT)
    {
        stopRequested = true;
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
//! @brief   Ctrl+c 割り込み有り
//! @details 無し
//! @return  true: 割り込み有り  false:割り込み無し
//!
bool IsStopRequested(void)
{
    return stopRequested;
}

//-----------------------------------------------------------------------------
//! @brief   ゲーム処理
//! @details ゲームを実行する
//! @return  無し
//!
void PlayGame()
{
    while(!IsStopRequested())
    {
        // 壁表示
        wall->Show(wall);

        // へび初期化
        snake->Init(snake);

        // えさ表示
        ShowFood();

        // スコアクリア
        score = 0;
        ShowScore();

        while( (snake->IsCollisionWall == false) && (!IsStopRequested()) )
        {
            // へび移動
            food->Draw(food);
            snake->Move(snake);

            // 障害物または体に衝突
            if(snake->IsCollisionWall == true)
            {
                ShowGameoverMsg();
            }
            else if(IsEat() == true)
            {
                // スコア加算
                if( score < 99999 )
                {
                    score += 30;
                    ShowScore();
                }

                // えさ表示
                ShowFood();

                // へびの体を１個追加
                snake->AddBody(snake, -1, -1);
            }
            Sleep(250);
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
    int32_t y;

    strcpy(msg,"GAME OVER!");
    x = ( (SCREEN_WIDTH - strlen(msg)) / 2) + 1;
    y = SCREEN_HEIGHT / 2;
    drawObject->PutStr(drawObject,x,y,msg);
    for (int i = 0; i < 30 && !IsStopRequested(); i++)
    {
        Sleep(100);
    }
    if (IsStopRequested())
    {
        return;
    }

    // 3秒の待機中に入力されたキーを破棄する
    while (_kbhit())
    {
        int key = _getch();
        if (key == 0 || key == 0xE0)
        {
            (void)_getch();
        }
    }

    strcpy(msg,"HIT ANY KEY!");
    x = ( (SCREEN_WIDTH - strlen(msg)) / 2) + 1;
    drawObject->PutStr(drawObject,x,(y+1),msg);

    // 案内を表示してから押されたキーを待つ
    while (!_kbhit() && !IsStopRequested())
    {
        Sleep(50);
    }
    if (!IsStopRequested())
    {
        int key = _getch();
        if (key == 0 || key == 0xE0)
        {
            (void)_getch();
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   へびがえさを食べたか判定
//! @details 無し
//! @return  true:食べた  false:食べてない
//!
bool IsEat()
{
    return( (food->X == snake->Body->Data[0].X) && (food->Y == snake->Body->Data[0].Y) );
}

//-----------------------------------------------------------------------------
//! @brief   えさを表示
//! @details へびと壁以外の場所にえさを置く
//! @return  無し
//!
void ShowFood()
{
    int8_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
    PosArrayIterator iterator = PosArray_Iterator(snake->Body);
    Pos* position = NULL;

    // へびのいる位置を配列に格納
    memset(screen, 0, sizeof(screen));
    while((position = iterator.Next(&iterator)) != NULL)
    {
        if( (position->X == -1) || (position->Y == -1) )
        {
            continue;
        }
        screen[(position->Y - 2)][(position->X - 1)] = 1;
    }

    // えさの位置決め
    food->Arrival(food);

    // えさがへびと重なっていたら、空いてる場所に移動
    if(screen[food->Y - 2][food->X - 1] == 1)
    {
        for(int x=0; x<SCREEN_WIDTH; x++)
        {
            for(int y=0; y<SCREEN_HEIGHT; y++)
            {
                if(screen[y][x] == 0)
                {
                    food->X = x + 1;
                    food->Y = y + 2;
                }
            }
        }
    }

    food->Draw(food);
}

//-----------------------------------------------------------------------------
//! @brief   スコア表示
//! @details 無し
//! @return  無し
//!
void ShowScore()
{
    char msg[20];
    sprintf(msg,"Score:%d",score);
    drawObject->PutStr(drawObject,0,0,msg);
}

//-----------------------------------------------------------------------------
//! @brief   メイン処理
//! @details スネークゲームを実行する
//! @return  0:正常終了  0以外:異常終了
//!
int main(void)
{
    BOOL isRegistCtrlHandler = FALSE;

    // [Ctrl+c]割り込みハンドラを登録
    if (SetConsoleCtrlHandler(HandleConsoleControl, TRUE))
    {
        isRegistCtrlHandler = TRUE;
    }
    else
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

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
    if(snake == NULL)
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        goto END;
    }

    // カーソル消去
    if (!drawObject->HideCursor(drawObject))
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
    }
    
    // ゲーム処理
    PlayGame();

END:
    // カーソルを画面最下段に設定
    if (drawObject != NULL)
    {
        drawObject->PutChar(drawObject, 0, (SCREEN_HEIGHT + 2), ' ');
    }

    // [Ctrl+C]割り込み解除
    if(isRegistCtrlHandler)
    {
        if(!SetConsoleCtrlHandler(HandleConsoleControl, FALSE))
        {
            ShowErrorMsg(__LINE__, (const int8_t*)__func__);
        }
    }

    // カーソル消去を解除
    if (drawObject != NULL && !drawObject->RestoreCursor(drawObject))
    {
        ShowErrorMsg(__LINE__, (const int8_t*)__func__);
    }

    if(wall != NULL)Wall_Delete(wall);
    if(food != NULL)Food_Delete(food);
    if(snake != NULL)Snake_Delete(snake);
    if(drawObject != NULL)DrawObject_Delete(drawObject);
    return 0;
}
