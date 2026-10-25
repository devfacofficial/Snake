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
 * これにより PosArray / PosIterator 型と、次の関数が使えます。
 *   <PosArray用関数>
 *     ・PosArray_New()                  配列を作成（失敗時は NULL）
 *     ・PosArray_Delete(array)          配列と要素領域を解放
 *     ・array->Push(array, &value)      値を末尾に追加（成功時 true）
 *     ・array->Clear(array)             要素領域を解放し、空の配列にする
 *   <PosIterator用関数>
 *     ・PosArray_Iterator(array)        イテレーターを作成
 *     ・iterator->Next(iterator)        次の要素を取り出す
 *
 * イテレータの使用例:
 *   PosIterator it = PosIterator(array);
 *   const Pos* value;
 *   while ((value = it.Next(&it)) != NULL)
 *   {
 *       // value は現在の要素を指す
 *   }
 *
 */
#define ARRAY(Type) \
    typedef struct Type##Array \
    { \
        Type*  Data; \
        size_t Length; \
        size_t Capacity; \
        bool (*Push)(struct Type##Array* this, Type* value); \
        void (*Clear)(struct Type##Array* this); \
    }Type##Array; \
    static inline bool Type##Array##_Push(Type##Array* this, Type* value); \
    static inline void Type##Array##_Clear(Type##Array* this); \
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
        array->Push  = Type##Array##_Push; \
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
    typedef struct Type##Iterator \
    { \
        Type##Array* Array; \
        size_t Index; \
        Type* (*Next)(struct Type##Iterator* this); \
    } Type##Iterator; \
    static inline Type* Type##Iterator_Next(Type##Iterator* this); \
    \
    static inline Type##Iterator Type##Array_Iterator(Type##Array* array) \
    { \
        Type##Iterator iterator = { array, 0, Type##Iterator##_Next }; \
        return iterator; \
    } \
    \
    static inline Type* Type##Iterator##_Next(Type##Iterator* iterator) \
    { \
        if (iterator == NULL || iterator->Array == NULL) \
        { \
            return NULL; \
        }\
        else if (iterator->Index >= iterator->Array->Length) \
        { \
            return NULL; \
        } \
        return &iterator->Array->Data[iterator->Index++]; \
    }
#endif
    