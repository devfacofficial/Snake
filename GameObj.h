#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdint.h>

//-----------------------------------------------------------------------------
//! @brief   ゲームオブジェクト基底クラス
//! @details Snake / Food / Wall の基底クラス
//!          多態性を実現するための関数ポインタを持つ
//!
typedef struct GameObj
{
    void (*Update)(struct GameObj* this);   //!< 更新処理
    void (*Draw)(struct GameObj* this);     //!< 描画処理
    void (*Delete)(struct GameObj* this);   //!< 解放処理
} GameObj;

void GameObj_Init(GameObj* this);
void GameObj_Update(GameObj* this);
void GameObj_Draw(GameObj* this);
void GameObj_Delete(GameObj* this);

#endif
