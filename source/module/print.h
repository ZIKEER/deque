/************************************************************************************
 *
 *   Copyright (C) 2020-2099 GoldCard. All rights reserved.
 *                              _        _                                   _
 *       ______________        | |      | |    ______    _____    _  _      | |
 *      /  _____________)      | |      | |   /  ____)  (___  \  | |/ /     | |
 *     /  /    ___     ___     | |    __| |  / /         ___| |  |   /    __| |
 *     |  |   (__ |   /  _ \   | |  /  _  |  | |        ( __  |  |  /   /  _  |
 *     \  \_____/ |   | |_| |  | |  | |_| |  \ \_____   ( |_| |  | |    | |_| |
 *      \________/    \____/   |_|  \____ /   \______)  (____/   |_|    \____ /
 *                                 (C)2020-2099 GoldCard
 *
 *
 ************************************************************************************/

#ifndef TOOL_MISC_PRINT_H
#define TOOL_MISC_PRINT_H

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 版     本: 1.0
* 修改历史: 首版
* 作     者: lss
*/

/*
* 使用说明:
*     1. 调用 DbgPrintReg 函数, 将串口发送函数注册进来
*     2. 在要打印的地方调用 DbgPrint 函数, 参数形式和 printf 相同
*/

/*
* 说明: 
*      DBG_PRINT_BUF_IS_GLOBAL 打开时, PRINT BUF是全局变量, 此时串口输出可阻塞式, 也可非阻塞式
*      DBG_PRINT_BUF_IS_GLOBAL 关闭时, PRINT BUF是局部变量, 此时串口输出只能时阻塞式
* 注意:
*      1. 当使用非阻塞式输出时, 如果上一次输出的字符串未输出完就输出第二个字符串, 第一个字符串会丢失部分字符
*      2. 根据Ram资源的紧张情况使用本宏
*/
#define DBG_PRINT_BUF_IS_GLOBAL
#define DBG_PRINT_BUF_SIZE	(100u)


/*
* brief: 串口发送回调函数
* param: str[in]: 待发送的字符串
*        len[in]: 待发送字符串长度
* result: -1: 发送失败
*         >0: 发送成功的字符数
*/
typedef uint16_t (*fUartSendStr)( const uint8_t* str, uint16_t len );


/*
* brief : 串口发送注册函数
* param : send_char[in]: 串口发送字符函数, DbgPrintBlock函数使用
*          send_str[in]: 串口发送字符串函数, DbgPrint函数使用
* result: -1: 注册失败
*          0: 注册成功
* remark: 两个注册函数根据使用接口填写, 不使用的接口可为NULL
*/
int8_t ToolMiscPrintReg( fUartSendStr send_str );


/*
* brief : 通过串口打印字符串
* param : format[in]: 输出字符串格式
* result: -1: 失败, 串口发送函数未注册
*         -2: 发送失败
*        >=0: 发送成功字符数
* remark: 此函数使用 注册的 fUartSendStr 函数发送, 阻塞式和非阻塞式发送, 取决于注册函数本身
*/
int16_t ToolMiscPrint( const char *format,... );


#ifdef __cplusplus
}
#endif

#endif

