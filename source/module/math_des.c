/***********************************Copyright(c)*******************************
**
**                             金卡智能集团股份有限公司
**
**                                 产品开发部
**
**                         http://www.china-goldcard.com
**
**-----------------------------------文件信息----------------------------------
** 文   件   名: math_des.c
** 创   建   人: wuzeyuan
** 版        本：V1.0.2
** 最后修改日期: 
** 描        述:
**
**---------------------------------历史版本信息--------------------------------
** 创   建   人:wuzeyuan 
** 版        本:V1.0.0
** 日        期:2021.10.12
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
#include "math_des.h"

static const uint8_t DesIp[] =
{
	58u, 50u, 42u, 34u, 26u, 18u, 10u, 2u,
	60u, 52u, 44u, 36u, 28u, 20u, 12u, 4u,
	62u, 54u, 46u, 38u, 30u, 22u, 14u, 6u,
	64u, 56u, 48u, 40u, 32u, 24u, 16u, 8u,
	57u, 49u, 41u, 33u, 25u, 17u, 9u,  1u,
	59u, 51u, 43u, 35u, 27u, 19u, 11u, 3u,
	61u, 53u, 45u, 37u, 29u, 21u, 13u, 5u,
	63u, 55u, 47u, 39u, 31u, 23u, 15u, 7u
};

static const uint8_t DesIp_1[] =
{
	40u, 8u,  48u, 16u, 56u, 24u, 64u, 32u,
	39u, 7u,  47u, 15u, 55u, 23u, 63u, 31u,
	38u, 6u,  46u, 14u, 54u, 22u, 62u, 30u,
	37u, 5u,  45u, 13u, 53u, 21u, 61u, 29u,
	36u, 4u,  44u, 12u, 52u, 20u, 60u, 28u,
	35u, 3u,  43u, 11u, 51u, 19u, 59u, 27u,
	34u, 2u,  42u, 10u, 50u, 18u, 58u, 26u,
	33u, 1u,  41u, 9u,  49u, 17u, 57u, 25u
};

static const uint8_t DesS[8][4][16] =
{
	{
		{14u, 4u, 13u, 1u, 2u, 15u, 11u, 8u, 3u, 10u, 6u, 12u, 5u, 9u, 0u, 7u},
		{0u, 15u, 7u, 4u, 14u, 2u, 13u, 1u, 10u, 6u, 12u, 11u, 9u, 5u, 3u, 8u},
		{4u, 1u, 14u, 8u, 13u, 6u, 2u, 11u, 15u, 12u, 9u, 7u, 3u, 10u, 5u, 0u},
		{15u, 12u, 8u, 2u, 4u, 9u, 1u, 7u, 5u, 11u, 3u, 14u, 10u, 0u, 6u, 13u}
	},

	{
		{15u, 1u, 8u, 14u, 6u, 11u, 3u, 4u, 9u, 7u, 2u, 13u, 12u, 0u, 5u, 10u},
		{3u, 13u, 4u, 7u, 15u, 2u, 8u, 14u, 12u, 0u, 1u, 10u, 6u, 9u, 11u, 5u},
		{0u, 14u, 7u, 11u, 10u, 4u, 13u, 1u, 5u, 8u, 12u, 6u, 9u, 3u, 2u, 15u},
		{13u, 8u, 10u, 1u, 3u, 15u, 4u, 2u, 11u, 6u, 7u, 12u, 0u, 5u, 14u, 9u}
	},

	{
		{10u, 0u, 9u, 14u, 6u, 3u, 15u, 5u, 1u, 13u, 12u, 7u, 11u, 4u, 2u, 8u},
		{13u, 7u, 0u, 9u, 3u, 4u, 6u, 10u, 2u, 8u, 5u, 14u, 12u, 11u, 15u, 1u},
		{13u, 6u, 4u, 9u, 8u, 15u, 3u, 0u, 11u, 1u, 2u, 12u, 5u, 10u, 14u, 7u},
		{1u, 10u, 13u, 0u, 6u, 9u, 8u, 7u, 4u, 15u, 14u, 3u, 11u, 5u, 2u, 12u}
	},

	{
		{7u, 13u, 14u, 3u, 0u, 6u, 9u, 10u, 1u, 2u, 8u, 5u, 11u, 12u, 4u, 15u},
		{13u, 8u, 11u, 5u, 6u, 15u, 0u, 3u, 4u, 7u, 2u, 12u, 1u, 10u, 14u, 9u},
		{10u, 6u, 9u, 0u, 12u, 11u, 7u, 13u, 15u, 1u, 3u, 14u, 5u, 2u, 8u, 4u},
		{3u, 15u, 0u, 6u, 10u, 1u, 13u, 8u, 9u, 4u, 5u, 11u, 12u, 7u, 2u, 14u}
	},

	{
		{2u, 12u, 4u, 1u, 7u, 10u, 11u, 6u, 8u, 5u, 3u, 15u, 13u, 0u, 14u, 9u},
		{14u, 11u, 2u, 12u, 4u, 7u, 13u, 1u, 5u, 0u, 15u, 10u, 3u, 9u, 8u, 6u},
		{4u, 2u, 1u, 11u, 10u, 13u, 7u, 8u, 15u, 9u, 12u, 5u, 6u, 3u, 0u, 14u},
		{11u, 8u, 12u, 7u, 1u, 14u, 2u, 13u, 6u, 15u, 0u, 9u, 10u, 4u, 5u, 3u}
	},

	{
		{12u, 1u, 10u, 15u, 9u, 2u, 6u, 8u, 0u, 13u, 3u, 4u, 14u, 7u, 5u, 11u},
		{10u, 15u, 4u, 2u, 7u, 12u, 9u, 5u, 6u, 1u, 13u, 14u, 0u, 11u, 3u, 8u},
		{9u, 14u, 15u, 5u, 2u, 8u, 12u, 3u, 7u, 0u, 4u, 10u, 1u, 13u, 11u, 6u},
		{4u, 3u, 2u, 12u, 9u, 5u, 15u, 10u, 11u, 14u, 1u, 7u, 6u, 0u, 8u, 13u}
	},

	{
		{4u, 11u, 2u, 14u, 15u, 0u, 8u, 13u, 3u, 12u, 9u, 7u, 5u, 10u, 6u, 1u},
		{13u, 0u, 11u, 7u, 4u, 9u, 1u, 10u, 14u, 3u, 5u, 12u, 2u, 15u, 8u, 6u},
		{1u, 4u, 11u, 13u, 12u, 3u, 7u, 14u, 10u, 15u, 6u, 8u, 0u, 5u, 9u, 2u},
		{6u, 11u, 13u, 8u, 1u, 4u, 10u, 7u, 9u, 5u, 0u, 15u, 14u, 2u, 3u, 12u}
	},

	{
		{13u,2u,  8u,  4u, 6u, 15u, 11u, 1u,  10u, 9u, 	3u,  14u, 5u, 0u, 12u, 7u},
		{1u, 15u, 13u, 8u, 10u,3u,  7u,  4u,  12u, 5u, 	6u,  11u, 0u, 14u,9u,  2u},
		{7u, 11u, 4u,  1u, 9u, 12u, 14u, 2u,  0u,  6u, 	10u, 13u, 15u,3u, 5u,  8u},
		{2u, 1u, 14u, 7u, 4u, 10u, 8u,  13u, 15u, 12u, 9u,  0u,  3u, 5u, 6u,  11u}
	}
};

static const uint8_t DesE[] =
{
	32u, 1u,  2u,  3u,  4u,  5u,
	4u,  5u,  6u,  7u,  8u,  9u,
	8u,  9u,  10u, 11u, 12u, 13u,
	12u, 13u, 14u, 15u, 16u, 17u,
	16u, 17u, 18u, 19u, 20u, 21u,
	20u, 21u, 22u, 23u, 24u, 25u,
	24u, 25u, 26u, 27u, 28u, 29u,
	28u, 29u, 30u, 31u, 32u, 1u
};

static const uint8_t DesP[] =
{
	16u, 7u,  20u, 21u,
	29u, 12u, 28u, 17u,
	1u,  15u, 23u, 26u,
	5u,  18u, 31u, 10u,
	2u,  8u,  24u, 14u,
	32u, 27u, 3u,  9u,
	19u, 13u, 30u, 6u,
	22u, 11u, 4u,  25u
};

static const uint8_t DesPc_1[] =
{
	57u, 49u, 41u, 33u, 25u, 17u, 9u,
	1u,  58u, 50u, 42u, 34u, 26u, 18u,
	10u, 2u,  59u, 51u, 43u, 35u, 27u,
	19u, 11u, 3u,  60u, 52u, 44u, 36u,
	63u, 55u, 47u, 39u, 31u, 23u, 15u,
	7u,  62u, 54u, 46u, 38u, 30u, 22u,
	14u, 6u,  61u, 53u, 45u, 37u, 29u,
	21u, 13u, 5u,  28u, 20u, 12u, 4u
};

static const uint8_t DesPc_2[] =
{
	14u, 17u, 11u, 24u, 1u,  5u,
	3u,  28u, 15u, 6u,  21u, 10u,
	23u, 19u, 12u, 4u,  26u, 8u,
	16u, 7u,  27u, 20u, 13u, 2u,
	41u, 52u, 31u, 37u, 47u, 55u,
	30u, 40u, 51u, 45u, 33u, 48u,
	44u, 49u, 39u, 56u, 34u, 53u,
	46u, 42u, 50u, 36u, 29u, 32u
};

static const uint8_t DesRots[] =
{
	1u, 1u, 2u, 2u,
	2u, 2u, 2u, 2u,
	1u, 2u, 2u, 2u,
	2u, 2u, 2u, 1u,
	0u
};

static void movram(const uint8_t* source,uint8_t* target,uint8_t length);
static void doxor(uint8_t* sourceaddr,const uint8_t* targetaddr,uint8_t length);
static void setbit(uint8_t* dataddr,uint8_t pos,uint8_t b0);
static uint8_t getbit(const uint8_t* dataddr,uint8_t pos);
static void selectbits(const uint8_t* source,const uint8_t* table,uint8_t* target,uint8_t count);
static void shlc(uint8_t* data_p);
static void shrc(uint8_t* data_p);
static void strans(const uint8_t* index,uint8_t* target);


static void movram(const uint8_t* source,uint8_t* target,uint8_t length)
{
	uint8_t i;
	for(i = 0;i < length;i++)
	{
		target[i] = source[i];
	}
}

static void doxor(uint8_t* sourceaddr,const uint8_t* targetaddr,uint8_t length)
{
	uint8_t i;
	for (i = 0;i < length;i++)
	{
		sourceaddr[i] ^= targetaddr[i];
	}
}

static void setbit(uint8_t* dataddr,uint8_t pos,uint8_t b0)
{
	uint8_t byte_count;
	uint8_t bit_count;
	uint8_t temp;

	temp = 1;
	byte_count = (pos - 1u) >>3;
	bit_count = 7u - ((pos - 1u) & 7u);
	temp <<= bit_count;

	if(b0!=0u)
	{
		dataddr[byte_count] |= temp;
	}
	else
	{
		temp = ~temp;
		dataddr[byte_count] &= temp;
	}
}

static uint8_t getbit(const uint8_t* dataddr,uint8_t pos)
{
	uint8_t byte_count;
	uint8_t bit_count;
	uint8_t temp,ret;

	temp = 1;
	byte_count = (pos - 1u) >>3;
	bit_count = 7u - ((pos - 1u) &7u);
	temp <<= bit_count;
	if((dataddr[byte_count] & temp)!=0u)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	
	return ret;
}

static void selectbits(const uint8_t* source,const uint8_t* table,uint8_t* target,uint8_t count)
{
	uint8_t i;

	for(i = 0;i < count;i++)
	{
		setbit(target,i + 1u,getbit(source,table[i]));
	}
}

static void shlc(uint8_t* data_p)
{
	uint8_t i,b0;

	b0 = getbit(data_p,1);

	for(i = 0u;i < 7u;i++)
	{
		data_p[i] <<= 1;

		if(i != 6u)
		{
			setbit(&data_p[i],8u,getbit(&data_p[i + 1u],1));
		}
	}

	setbit(data_p,56,getbit(data_p,28));
	setbit(data_p,28,b0);
}

static void shrc(uint8_t* data_p)
{
	uint8_t b0;
	int i;

	b0 = getbit(data_p,56);

	for(i = 6;i >= 0;i--)
	{
		data_p[i] >>= 1;
		if(i != 0)
		{
			setbit(&data_p[i],1,getbit(&data_p[i - 1],8));
		}
	}

	setbit(data_p,1,getbit(data_p,29));
	setbit(data_p,29,b0);
}

/* The problem is about yielded in this function */
static void strans(const uint8_t* index,uint8_t* target)
{
	uint8_t row,line,t,i,j;
	uint8_t k,m;
	for(i = 0;i < 4u;i++)
	{
		m=(i<<3u);//i*8
		for(k=0;k<4u;k++)
		{
			m+=i;
		}
//		t =0;
		line=0;
		row=0;
		setbit(&line,7, getbit(index,m + 1u));
		setbit(&line,8, getbit(index,m + 6u));

		for(j = 2;j < 6u;j++)
		{
			setbit(&row,3u + j, getbit(index,m + j));
		}

		t = DesS[i+i][line][row];
		t <<= 4;
		row=0;
		line=0;
		setbit(&line,7, getbit(index,m + 7u));
		setbit(&line,8, getbit(index,m + 12u));

		for(j = 2;j < 6u;j++)
		{
			setbit(&row,3u + j,getbit(index,m+ 6u + j));
		}

		t |= DesS[(i <<1) + 1u][line][row];
		target[i] = t;
	}
}

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
void MathDes(uint8_t *data_p, const uint8_t* key_p, int8_t type)
{
	uint8_t tempbuf[12];
	uint8_t key[7];
	uint8_t i,j,count;
	void (*f)(uint8_t* data_p);

	selectbits(data_p,DesIp,tempbuf,64);
	movram(tempbuf,data_p,8);
	selectbits(key_p,DesPc_1,key,56);

	for(i = 0;i < 16u;i ++)
	{
		selectbits(&data_p[4],DesE,tempbuf,48);
		if(type ==1)		//jia mi
		{
			f = shlc;
			count = i;
		}
		else
		{
			count = 16u - i;
			f = shrc;
		}

		for(j = 0;j < DesRots[count];j++)/*KEY ---*/
		{
			f(key);
		}

		selectbits(key,DesPc_2,&tempbuf[6],48);/*KEY ----*/
		doxor(tempbuf,&tempbuf[6],6);
		strans(tempbuf,&tempbuf[6]);
		selectbits(&tempbuf[6],DesP,tempbuf,32);
		doxor(tempbuf,data_p,4);
		if(i < 15u)
		{
			movram(&data_p[4],data_p,4);
			movram(tempbuf,&data_p[4],4);
		}
	}

	movram(tempbuf,data_p,4);
	selectbits(data_p,DesIp_1,tempbuf,64);
	movram(tempbuf,data_p,8);
}

/************************************函数说明**********************************
**函数名称：Math3Des
**
**具体描述：3Des加密/解密
**
**输    入：data_p:明文或密文起始地址,key_p，key_c_p，key_c_p:密钥起始地址,type：1是加密 0是解密
**
**输    出：加密/解密后结果：data_p
******************************************************************************/
void Math3Des(uint8_t *data_p, const uint8_t* key_a_p, const uint8_t* key_b_p, const uint8_t* key_c_p, uint8_t type)
{
    if (type == 1)
	{
		MathDes(data_p, key_a_p, 1);
		MathDes(data_p, key_b_p, 0);
		MathDes(data_p, key_c_p, 1);
	}
	else
	{
		MathDes(data_p, key_c_p, 0);
		MathDes(data_p, key_b_p, 1);
		MathDes(data_p, key_a_p, 0);		
	}
}
/************************************函数说明**********************************
**函数名称：Math3DesN8Bytes
**
**具体描述：多字节3Des加密/解密
**
**输    入：data_p:明文或密文起始地址,key_p，key_c_p，key_c_p:密钥起始地址,type：1是加密 0是解密
**
**输    出：加密/解密后结果：data_p
******************************************************************************/
void Math3DesN8Bytes(uint8_t *data_p, uint16_t bytes_len, const uint8_t* key_a_p, const uint8_t* key_b_p, const uint8_t* key_c_p, uint8_t type)
{
    uint16_t i;
	for (i = 0; i < bytes_len; i+=8)
	{
		Math3Des(&data_p[i], key_a_p, key_b_p, key_c_p, type);
	}
}
