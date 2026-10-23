#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * ARRAY(Type) は、Type 型の値を格納する動的配列とイテレータを定義します。
 * 要素型を定義した後に呼び出してください。
 *
 * 例:
 *   typedef struct Pos { int X; int Y; } Pos;
 *   ARRAY(Pos);
 *
 * これにより PosArray / PosArrayIterator 型と、次の関数が使えます。
 *   PosArray_New()                  配列を作成（失敗時は NULL）
 *   array->Push(array, &value)      値を末尾に追加（成功時 true）
 *   array->Clear(array)             要素領域を解放し、空の配列にする
 *   PosArray_Delete(array)          配列と要素領域を解放
 *
 * Push は value が指す値を配列内にコピーします。value の領域自体は
 * 配列には保存されません。配列を使い終えたら Delete してください。
 * Clear の後も配列オブジェクトは引き続き使用できます。
 *
 * 反復処理の例:
 *   PosArrayIterator it = PosArray_Iterator(array);
 *   const Pos* value;
 *   while ((value = it.Next(&it)) != NULL)
 *   {
 *       // value は現在の要素を指す
 *   }
 *
 * Next は次の要素へのポインタを返し、終端または無効なイテレータでは
 * NULL を返します。返されたポインタは読み取り専用です。反復中は配列を
 * Clear/Delete したり、Push で再確保が起こる可能性のある変更をしないで
 * ください。
 */
#define ARRAY(Type) \
    typedef struct Type##Array Type##Array; \
    typedef struct Type##ArrayIterator Type##ArrayIterator; \
    static inline bool Type##Array##_Push(Type##Array* array, Type* value); \
    static inline void Type##Array##_Clear(Type##Array* array); \
    static inline Type* Type##ArrayIterator##_Next(Type##ArrayIterator* iterator); \
    \
    struct Type##Array \
    { \
        Type* Data; \
        size_t Length; \
        size_t Capacity; \
        bool (*Push)(Type##Array* this, Type* value); \
        void (*Clear)(Type##Array* this); \
    }; \
    \
    typedef struct Type##ArrayIterator \
    { \
        Type##Array* Array; \
        size_t Index; \
        Type* (*Next)(Type##ArrayIterator* this); \
    } Type##ArrayIterator; \
    \
    static inline Type##Array* Type##Array##_New(void) \
    { \
        Type##Array* array = malloc(sizeof(*array)); \
        if (array == NULL) \
        { \
            return NULL; \
        } \
        array->Data = NULL; \
        array->Length = 0; \
        array->Capacity = 0; \
        array->Push = Type##Array##_Push; \
        array->Clear = Type##Array##_Clear; \
        array->Clear(array); \
        return array; \
    } \
    \
    static inline void Type##Array##_Delete(Type##Array* array) \
    { \
        if (array != NULL) \
        { \
            Type##Array##_Clear(array); \
            free(array); \
        } \
    } \
    \
    static inline bool Type##Array##_Push(Type##Array* array, Type* value) \
    { \
        if (array == NULL || array->Length == SIZE_MAX) \
        { \
            return false; \
        } \
        if (array->Length == array->Capacity) \
        { \
            size_t newCapacity = array->Capacity == 0 ? 4 : array->Capacity * 2; \
            if (newCapacity < array->Capacity || newCapacity > SIZE_MAX / sizeof(Type)) \
            { \
                return false; \
            } \
            Type* newData = realloc(array->Data, newCapacity * sizeof(Type)); \
            if (newData == NULL) \
            { \
                return false; \
            } \
            array->Data = newData; \
            array->Capacity = newCapacity; \
        } \
        array->Data[array->Length++] = *value; \
        return true; \
    } \
    \
    static inline void Type##Array##_Clear(Type##Array* array) \
    { \
        if (array != NULL) \
        { \
            free(array->Data); \
            array->Data = NULL; \
            array->Length = 0; \
            array->Capacity = 0; \
        } \
    } \
    \
    static inline Type##ArrayIterator Type##Array##_Iterator(Type##Array* array) \
    { \
        Type##ArrayIterator iterator = { array, 0, Type##ArrayIterator##_Next }; \
        return iterator; \
    } \
    \
    static inline Type* Type##ArrayIterator##_Next(Type##ArrayIterator* iterator) \
    { \
        if (iterator == NULL || iterator->Array == NULL || \
            iterator->Index >= iterator->Array->Length) \
        { \
            return NULL; \
        } \
        return &iterator->Array->Data[iterator->Index++]; \
    }
#endif
