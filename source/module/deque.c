/*
 *             Copyright (C) 2023-2023 ZZK. All rights reserved.
 * @Author: zzk
 * @Date: 2023-07-11
 * @LastEditors: zzk
 * @LastEditTime: 2023-07-12
 * @Description: 实现环形队列支持双端读取
 */
#include "deque.h"

/**
 * @brief: Deque初始化
 * @param deque_obj  sDeque* obj
 * @param data_pool  void* 数据缓存
 * @param max_len  uint16_t 最大长度
 * @return  0成功 其他失败
 * @note:
 */
int8_t DequeInit(sDeque* deque_obj, void* data_pool, uint16_t max_len)
{
    if((deque_obj == NULL) ||
       (data_pool == NULL) ||
       (max_len == 0U))
    { /* 参数错误 */
        return -1;
    }

    deque_obj->buffer = (uint8_t*)data_pool;
    deque_obj->size   = max_len;
    deque_obj->head   = 0U;
    deque_obj->tail   = 1U;
    deque_obj->count  = 0U;

    return 0U;
}

/**
 * @brief: Deque清空
 * @param deque_obj  sDeque* obj
 * @return  0成功 其他失败
 * @note: 
 */
int8_t DequeClear(sDeque* deque_obj)
{
    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U))
    { /* 参数错误 */
        return -1;
    }

    deque_obj->head   = 0U;
    deque_obj->tail   = 1U;
    deque_obj->count  = 0U;

    return 0U;
}

/**
 * @brief: 获取队列空闲大小
 * @param deque_obj  sDeque*
 * @return  空闲大小
 * @note:
 */
uint16_t DequeGetFreeSize(sDeque* deque_obj)
{
    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U))
    { /* 参数错误 */
        return 0U;
    }

    return deque_obj->size - deque_obj->count;
}

/**
 * @brief: 获取队列已用大小
 * @param deque_obj  sDeque*
 * @return  已用大小
 * @note:
 */
uint16_t DequeGetUsedSize(sDeque* deque_obj)
{
    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U))
    { /* 参数错误 */
        return 0U;
    }

    return deque_obj->count;
}

/**
 * @brief: 从前面放入一个字节
 * @param deque_obj  sDeque* obj
 * @param byte  uint8_t 字节数据
 * @return  无
 * @note: 内部调用不考虑失败
 */
static void DequePushFrontByte(sDeque* deque_obj, uint8_t byte)
{
    deque_obj->buffer[deque_obj->head] = byte;
    if(deque_obj->head == 0U)
    {
        deque_obj->head = deque_obj->size;
    }
    deque_obj->head--;
    deque_obj->count++;
}

/**
 * @brief: 从后面放入一个字节
 * @param deque_obj  sDeque* obj
 * @param byte  uint8_t 字节数据
 * @return  无
 * @note: 内部调用不考虑失败
 */
static void DequePushBackByte(sDeque* deque_obj, uint8_t byte)
{
    deque_obj->buffer[deque_obj->tail] = byte;
    deque_obj->tail++;
    deque_obj->count++;
    if(deque_obj->tail == deque_obj->size)
    {
        deque_obj->tail = 0U;
    }
}

/**
 * @brief: 从前面取一个字节
 * @param deque_obj  sDeque* obj
 * @return  返回字节
 * @note: 内部调用不考虑失败
 */
static uint8_t DequePopFrontByte(sDeque* deque_obj)
{
    uint8_t byte;

    deque_obj->head++;
    deque_obj->count--;
    if(deque_obj->head == deque_obj->size)
    {
        deque_obj->head = 0U;
    }
    byte = deque_obj->buffer[deque_obj->head];

    return byte;
}

/**
 * @brief: 从后面取一个字节
 * @param deque_obj  sDeque* obj
 * @return  返回字节
 * @note: 内部调用不考虑失败
 */
static uint8_t DequePopBackByte(sDeque* deque_obj)
{
    uint8_t byte;

    if(deque_obj->tail == 0U)
    {
        deque_obj->tail = deque_obj->size;
    }
    deque_obj->tail--;
    deque_obj->count--;
    byte = deque_obj->buffer[deque_obj->tail];

    return byte;
}

/**
 * @brief: 从前面添加多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param len  uint16_t 数据长度
 * @return  无
 * @note:
 */
int8_t DequePushFrontBytes(sDeque* deque_obj, void* bytes, uint16_t len)
{
    uint16_t i;
    uint8_t* push_data = (uint8_t*)bytes;

    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U) || 
       (len == 0U))
    { /* 参数错误 */
        return -1;
    }

    if(len > DequeGetFreeSize(deque_obj))
    { /* 装不下/参数错误 */
        return -1;
    }
    
    for(i = len; i != 0U; i--)
    {
        DequePushFrontByte(deque_obj, push_data[i-1]);
    }

    return 0;
}

/**
 * @brief: 从后面添加多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param len  uint16_t 数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePushBackBytes(sDeque* deque_obj, void* bytes, uint16_t len)
{
    uint16_t i;
    uint8_t* push_data = (uint8_t*)bytes;
    
    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U) || 
       (len == 0U))
    { /* 参数错误 */
        return -1;
    }
    
    if(len > DequeGetFreeSize(deque_obj))
    { /* 装不下/参数错误 */
        return -1;
    }

    for(i = 0; i < len; i++)
    {
        DequePushBackByte(deque_obj, push_data[i]);
    }

    return 0;
}

/**
 * @brief: 从前面取多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param need_len  uint16_t 需要数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePopFrontBytes(sDeque* deque_obj, void* bytes, uint16_t need_len)
{
    uint16_t i;
    uint8_t* pop_data = (uint8_t*)bytes;
    
    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U) || 
       (bytes == NULL) ||
       (need_len == 0U))
    { /* 参数错误 */
        return -1;
    }

    if(need_len > DequeGetUsedSize(deque_obj))
    {   /* 需要数据超过已有数据 */
        return -1;
    }

    for(i = 0; i < need_len; i++)
    {
        pop_data[i] = DequePopFrontByte(deque_obj);
    }

    return 0;
}

/**
 * @brief: 从后面取多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param need_len  uint16_t 需要数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePopBackBytes(sDeque* deque_obj, void* bytes, uint16_t need_len)
{
    uint16_t i;
    uint8_t* pop_data = (uint8_t*)bytes;

    if((deque_obj == NULL) ||
       (deque_obj->buffer == NULL) ||
       (deque_obj->size == 0U) || 
       (bytes == NULL) ||
       (need_len == 0U))
    { /* 参数错误 */
        return -1;
    }

    if(need_len > DequeGetUsedSize(deque_obj))
    {   /* 需要数据超过已有数据 */
        return -1;
    }

    for(i = need_len; i != 0U; i--)
    {
        pop_data[i-1] = DequePopBackByte(deque_obj);
    }

    return 0;
}


