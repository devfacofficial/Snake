#include "GameObj.h"

//-----------------------------------------------------------------------------
//! @brief   初期化
//! @details 無し
//! @return  無し
//!
void GameObj_Init(
    GameObj* this)                      //!< [I]オブジェクトポインタ
{
    this->Update = GameObj_Update;
    this->Draw   = GameObj_Draw;
    this->Delete = GameObj_Delete;
}

//-----------------------------------------------------------------------------
//! @brief   更新処理
//! @details 無し
//! @return  無し
//!
void GameObj_Update(
    GameObj* this)                      //!< [I]オブジェクトポインタ
{
    (void)this;
}

//-----------------------------------------------------------------------------
//! @brief   描画処理
//! @details 無し
//! @return  無し
//!
void GameObj_Draw(
    GameObj* this)                      //!< [I]オブジェクトポインタ
{
    (void)this;
}

//-----------------------------------------------------------------------------
//! @brief   解放処理
//! @details 無し
//! @return  無し
//!
void GameObj_Delete(
    GameObj* this)                      //!< [I]オブジェクトポインタ
{
    (void)this;
}
