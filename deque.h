#ifndef DEQUE_H__
#define DEQUE_H__

/////////////////////////////////////////头文件start/////////////////////////////////////////
#include "hal_type.h"
/////////////////////////////////////////头文件end///////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Duque
{
    uint8_t* buffer;    // 数据buff
    uint16_t size;      // 数据buff大小
    uint16_t head;      // 队列头 指向将要写的索引 +1为将要读的索引
    uint16_t tail;      // 队列尾 指向将要写的索引 -1为将要读的索引
    uint16_t count;     // 个数
}sDeque;

/**
 * @brief: Deque初始化
 * @param deque_obj  sDeque* obj
 * @param data_pool  void* 数据缓存
 * @param max_len  uint16_t 最大长度
 * @return  0成功 其他失败
 * @note:
 */
int8_t DequeInit(sDeque* deque_obj, void* data_pool, uint16_t max_len);

/**
 * @brief: Deque清空
 * @param deque_obj  sDeque* obj
 * @return  0成功 其他失败
 * @note: 
 */
int8_t DequeClear(sDeque* deque_obj);

/**
 * @brief: 获取队列空闲大小
 * @param deque_obj  sDeque*
 * @return  空闲大小
 * @note:
 */
uint16_t DequeGetFreeSize(sDeque* deque_obj);

/**
 * @brief: 获取队列已用大小
 * @param deque_obj  sDeque*
 * @return  已用大小
 * @note:
 */
uint16_t DequeGetUsedSize(sDeque* deque_obj);

/**
 * @brief: 从前面添加多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param len  uint16_t 数据长度
 * @return  无
 * @note:
 */
int8_t DequePushFrontBytes(sDeque* deque_obj, void* bytes, uint16_t len);

/**
 * @brief: 从后面添加多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param len  uint16_t 数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePushBackBytes(sDeque* deque_obj, void* bytes, uint16_t len);

/**
 * @brief: 从前面取多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param need_len  uint16_t 需要数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePopFrontBytes(sDeque* deque_obj, void* bytes, uint16_t need_len);

/**
 * @brief: 从后面取多个字节
 * @param deque_obj  sDeque* obj
 * @param bytes  void* 数据地址
 * @param need_len  uint16_t 需要数据长度
 * @return  无 
 * @note: 
 */
int8_t DequePopBackBytes(sDeque* deque_obj, void* bytes, uint16_t need_len);


#ifdef __cplusplus
}
#endif

#endif
