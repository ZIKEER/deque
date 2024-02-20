/***********************************Copyright(c)*******************************
**
**                             金卡智能集团股份有限公司
**
**                                 产品开发部
**
**                         http://www.china-goldcard.com
**
**-----------------------------------文件信息----------------------------------
** 文   件   名: math_des.h
** 创   建   人: 
** 版        本：v1.0.2
** 最后修改日期: 
** 描        述:
**
**---------------------------------历史版本信息--------------------------------
** 创   建   人: wuzeyuan
** 版        本: V1.0.0
** 日        期: 2021.10.12
** 描        述:
**
**-----------------------------------------------------------------------------
** 修   改   人: 胡中卫
** 版        本: v1.0.1
** 日        期: 2022-5-7
** 描        述: 增加3des接口
**
**-----------------------------------------------------------------------------
**-----------------------------------------------------------------------------
** 修   改   人: 胡中卫
** 版        本: v1.0.2
** 日        期: 2022-8-5
** 描        述: 取消加解密宏
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef MATH_DES_H__
#define MATH_DES_H__

/////////////////////////////////////////头文件start/////////////////////////////////////////
#include "hal_type.h"
/////////////////////////////////////////头文件end///////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif


/************************************函数说明**********************************
**函数名称：MathDes
**
**具体描述：DES加解密
**
**输    入：data_p:明文或密文起始地址,key_p:8位密钥起始地址,type：1是加密 0是解密
**
**输    出：加密从data_p输出密文，解密从data_p输出明文
**
**全局变量：
**
**调用模块：
**
**创建作者：
**
**创建时间：2014-11-07 16:08
-------------------------------------------------------------------------------
**修改作者：
**修改时间：
**具体描述：
-------------------------------------------------------------------------------
******************************************************************************/
extern void MathDes(uint8_t *data_p, const uint8_t* key_p, int8_t type);
/************************************函数说明**********************************
**函数名称：Math3Des
**
**具体描述：3Des加密/解密
**
**输    入：data_p:明文或密文起始地址,key_p，key_c_p，key_c_p:密钥起始地址,type：1是加密 0是解密
**
**输    出：加密/解密后结果：data_p
******************************************************************************/
extern void Math3Des(uint8_t *data_p, const uint8_t* key_a_p, const uint8_t* key_b_p, const uint8_t* key_c_p, uint8_t type);
/************************************函数说明**********************************
**函数名称：Math3DesN8Bytes
**
**具体描述：多字节3Des加密/解密
**
**输    入：data_p:明文或密文起始地址,key_p，key_c_p，key_c_p:密钥起始地址,type：1是加密 0是解密
**
**输    出：加密/解密后结果：data_p
******************************************************************************/
extern void Math3DesN8Bytes(uint8_t *data_p, uint16_t bytes_len, const uint8_t* key_a_p, const uint8_t* key_b_p, const uint8_t* key_c_p, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif
