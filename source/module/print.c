#include <stdio.h>
#include <stdarg.h>

#include "print.h"




#if defined (__CC_ARM) || defined(__ICCARM__)
	// ARM Compiler, IAR Compiler
	typedef char*				uintptr_t;
	typedef void*				voidprt_t;
#elif defined (__GNUC__)
	// GNU Compiler
	typedef unsigned long		uintptr_t;
	typedef void*				voidprt_t;
#elif defined (__RL78__)
	// cs+ Compiler
	typedef char __far *		uintptr_t;
	typedef void __far*			voidprt_t;
#else
	#error "NOT SUPPORT THIS COMPILE"
#endif
typedef unsigned long phys_addr_t;


#define PRINT_FLAG_ZEROPAD			(0x01u)		/* pad with zero */
#define PRINT_FLAG_SIGN_NUMBER		(0x02u)		/* unsigned/signed long */
#define PRINT_FLAG_PLUS				(0x04u)		// 右对齐, 输出正负号
#define PRINT_FLAG_SPACE			(0x08u)		/* space if plus */
#define PRINT_FLAG_LEFT_ALIGN		(0x10u)		// 左对齐输出，不够右边补空格
#define PRINT_FLAG_HEX_LOWER_CASE	(0x20u)		// Must be 32 == 0x20
#define PRINT_FLAG_WELL_SPECIAL		(0x40u)		// 对c,s,d,u类无影响; 对o类，在输出时加前缀o; 对x类，在输出时加前缀0x
#define PRINT_FLAG_ERRSTR			(0x80u)		/* %dE showing error string if enabled */


#define TOOL_MISC_PUT_CH( _str, _end, _ch )	\
do{									\
	if( (_str) < (_end) )			\
	{								\
		*(_str) = (_ch);			\
	}								\
	_str++;							\
}while( 0 )

#define STRING_END_CHAR		('\0')

#if 0
static const uint8_t	ErrorBufOverFlow[] = "PRINT BUF OVERFLOW\r\n";
#endif

#ifdef DBG_PRINT_BUF_IS_GLOBAL
uint8_t	PrintBuf[DBG_PRINT_BUF_SIZE];
#endif


fUartSendStr	UartSendStr  = NULL;

#if 0

/*
* brief : 字符串拷贝, 包含尾部的空字符
* param : dest[in/out]: 字符串存放 buffer
*             size[in]: 字符串存放 buffer 大小
*              src[in]: 源字符串, 以空结尾
* result: 0: 参数非法
*        >0: 字符串长度
*/
static uint16_t DbgPrintStrCpy( uint8_t* dest, uint16_t size, const uint8_t* src )
{
	uint16_t	i, num = 0u;
	
	if( NULL == dest 
	||	NULL == src )
	{
		return 0;
	}

	for( i=0u; i<size; i++ )
	{
		dest[i] = src[i];
		num++;
		
		if( src[i] == STRING_END_CHAR )
		{
			break;
		}
	}

	return num;
}

static uint16_t MyDataFormatD( uint8_t* buf, uint16_t size, int64_t val, uint8_t base, uint8_t lead, uint8_t width )
{
	uint16_t	index, i;
	uint64_t	uval;
	const uint8_t	HexTab[]={
		'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
	};

	if( NULL == buf
	||	size < 1u 
	||	base < 2u )
	{
		return 0u;
	}
	
	index = 0u;
	if( val < 0 )
	{
		buf[index++] = '-';
		uval = -val;
	}
	else
	{
		uval = val;
	}

	do
	{
		if( index > size )
		{
			break;
		}
		buf[index++] = HexTab[ uval % base ];
	}while( (uval /= base) != 0 );
	
	
	if( width > 0u && index < width )
	{
		for ( i=width - index; i; i-- )
		{
			buf[index++] = lead;
		}
	}

	return index;
}
#endif

/* Decimal conversion is by far the most typical, and is used
 * for /proc and /sys data. This directly impacts e.g. top performance
 * with many processes running. We optimize it for speed
 * using code from
 * http://www.cs.uiowa.edu/~jones/bcd/decimal.html
 * (with permission from the author, Douglas W. Jones). */

/* Formats correctly any integer in [0,99999].
 * Outputs from one to five digits depending on input.
 * On i386 gcc 4.1.2 -O2: ~250 bytes of code. */
static char *ToolMiscPutDecTrunc( char *buf, uint32_t q )
{
	uint32_t d3, d2, d1, d0;
	
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	d0 = 6 * (d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0'; /* least significant digit */
	d1 = q + 9 * d3 + 5 * d2 + d1;
	if( d1 != 0 )
	{
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10 * q;
		*buf++ = d1 + '0'; /* next digit */

		d2 = q + 2 * d2;
		if( (d2 != 0) || (d3 != 0) )
		{
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10 * q;
			*buf++ = d2 + '0'; /* next digit */

			d3 = q + 4 * d3;
			if( d3 != 0 )
			{
				q = (d3 * 0xcd) >> 11;
				d3 = d3 - 10 * q;
				*buf++ = d3 + '0';	/* next digit */
				if( q != 0 )
				{
					*buf++ = q + '0'; /* most sign. digit */
				}
			}
		}
	}
	
	return buf;
}


/* Same with if's removed. Always emits five digits */
static char *ToolMiscPutDecFull(char *buf, uint32_t q)
{
	/* BTW, if q is in [0,9999], 8-bit ints will be enough, */
	/* but anyway, gcc produces better code with full-sized ints */
	uint32_t	d3, d2, d1, d0;
	
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	/*
	 * Possible ways to approx. divide by 10
	 * gcc -O2 replaces multiply with shifts and adds
	 * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
	 * (x * 0x67) >> 10:  1100111
	 * (x * 0x34) >> 9:    110100 - same
	 * (x * 0x1a) >> 8:     11010 - same
	 * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
	 */

	d0 = 6 * (d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10 * q;
	*buf++ = d0 + '0';
	d1 = q + 9 * d3 + 5 * d2 + d1;
	q = (d1 * 0xcd) >> 11;
	d1 = d1 - 10 * q;
	*buf++ = d1 + '0';

	d2 = q + 2 * d2;
	q = (d2 * 0xd) >> 7;
	d2 = d2 - 10 * q;
	*buf++ = d2 + '0';

	d3 = q + 4 * d3;
	q = (d3 * 0xcd) >> 11; /* - shorter code */
	/* q = (d3 * 0x67) >> 10; - would also work */
	d3 = d3 - 10 * q;
	*buf++ = d3 + '0';
	*buf++ = q + '0';
	
	return buf;
}

static char *ToolMiscPutDec( char *buf, uint64_t num )
{
	uint32_t	rem;

	while( 1 )
	{
		if( num < 100000 )
		{
			return ToolMiscPutDecTrunc( buf, num );
		}
		
		rem = num % 100000;
		num = num / 100000;
		buf = ToolMiscPutDecFull(buf, rem);
	}
}


static uintptr_t ToolMiscPutNumber( uintptr_t buf, uintptr_t end, uint64_t num, uint16_t base, int32_t field_width, int32_t precision, uint16_t flags )
{
	char		tmp[66];
	uint8_t		sign, need_pfx;
	uint16_t	lower_case, shift;
	int32_t		i;
	uint32_t	mask;

	static const char digits[16] = "0123456789ABCDEF";

	need_pfx   = ((flags & PRINT_FLAG_WELL_SPECIAL) && base != 10);
	lower_case = (flags & PRINT_FLAG_HEX_LOWER_CASE);
	if( flags & PRINT_FLAG_LEFT_ALIGN )
	{
		flags &= ~PRINT_FLAG_ZEROPAD;
	}
	
	sign = 0;
	if( flags & PRINT_FLAG_SIGN_NUMBER )
	{
		// 有符号数
		if( (int64_t)num < 0 )
		{
			sign = '-';
			num  = -(int64_t) num;
			field_width--;
		}
		else if( flags & PRINT_FLAG_PLUS )
		{
			sign = '+';
			field_width--;
		}
		else if( flags & PRINT_FLAG_SPACE )
		{
			sign = ' ';
			field_width--;
		}
	}
	
	if( need_pfx != 0U )
	{
		field_width--;
		if( base == 16 )
		{
			field_width--;
		}
	}

	/* generate full string in tmp[], in reverse order */
	i = 0;
	if( num == 0 )
	{
		tmp[i++] = '0';
	}
	else if( base != 10 )
	{
		// 8进制 or 16进制
		mask  = base - 1;
		shift = 3;

		if( base == 16 )
		{
			shift = 4;
		}
		
		do
		{
			tmp[i++] = (digits[((uint8_t)num) & mask] | lower_case);
			num >>= shift;
		}while( num );
	}
	else
	{
		// 10进制
		i = (int32_t)(ToolMiscPutDec(tmp, num) - tmp);
	}

	if( i > precision )
	{
		precision = i;
	}
	
	/* leading space padding */
	field_width -= precision;
	if( !(flags & (PRINT_FLAG_ZEROPAD + PRINT_FLAG_LEFT_ALIGN)) )
	{
		while( --field_width >= 0 )
		{
			TOOL_MISC_PUT_CH( buf, end, ' ' );
		}
	}
	
	if( sign != 0u )
	{
		TOOL_MISC_PUT_CH( buf, end, sign );
	}
	
	// "0x" / "0" prefix
	if( need_pfx != 0u )
	{
		TOOL_MISC_PUT_CH( buf, end, '0' );
		if( base == 16 )
		{
			TOOL_MISC_PUT_CH( buf, end, 'X' | lower_case);
		}
	}
	
	/* zero or space padding */
	if( !(flags & PRINT_FLAG_LEFT_ALIGN) )
	{
		char c = (flags & PRINT_FLAG_ZEROPAD) ? '0' : ' ';

		while( --field_width >= 0 )
		{
			TOOL_MISC_PUT_CH(buf, end, c);
		}
	}
	
	/* hmm even more zero padding? */
	while( i <= --precision )
	{
		TOOL_MISC_PUT_CH( buf, end, '0' );
	}
	
	/* actual digits of result */
	while( --i >= 0 )
	{
		TOOL_MISC_PUT_CH( buf, end, tmp[i] );
	}
	
	/* trailing space padding */
	while( --field_width >= 0 )
	{
		TOOL_MISC_PUT_CH( buf, end, ' ' );
	}
	
	return buf;
}




static uintptr_t ToolMiscPutString( uintptr_t buf, uintptr_t end, uintptr_t sub_str, int32_t field_width, int32_t precision, uint16_t flags )
{
	int32_t		i, len;
	uintptr_t	p;
	
	if( sub_str == NULL )
	{
		sub_str = "<NULL>";
	}
	
	for( p = sub_str; *p != STRING_END_CHAR && precision--; ++p );
	len = p - sub_str;
	
	if( !(flags & PRINT_FLAG_LEFT_ALIGN) )
	{
		while( len < field_width-- )
		{
			TOOL_MISC_PUT_CH( buf, end, ' ' );
		}
	}
	
	for( i = 0; i < len; ++i )
	{
		TOOL_MISC_PUT_CH( buf, end, *sub_str++ );
	}
	
	while( len < field_width-- )
	{
		TOOL_MISC_PUT_CH( buf, end, ' ' );
	}
	
	return buf;
}


/*
* Show a '%p' thing.	A kernel extension is that the '%p' is followed
* by an extra set of alphanumeric characters that are extended format
* specifiers.
*
* Right now we handle:
*
* - 'M' For a 6-byte MAC address, it prints the address in the
* 	  usual colon-separated hex notation
* - 'I' [46] for IPv4/IPv6 addresses printed in the usual way (dot-separated
* 	  decimal for v4 and colon separated network-order 16 bit hex for v6)
* - 'i' [46] for 'raw' IPv4/IPv6 addresses, IPv6 omits the colons, IPv4 is
* 	  currently the same
*/
static uintptr_t ToolMiscPutPointer( uintptr_t buf, uintptr_t end, voidprt_t ptr, int32_t field_width, int32_t precision, uint16_t flags )
{
	uint64_t num = (uint64_t)ptr;

	flags |= PRINT_FLAG_HEX_LOWER_CASE;
	if( field_width == -1 )
	{
		field_width = 2 * sizeof(voidprt_t);
		flags |= PRINT_FLAG_ZEROPAD;
	}
	
	return ToolMiscPutNumber( buf, end, num, 16, field_width, precision, flags );
}




uint16_t ToolMiscGetFlags( const uint8_t** format )
{
	uint16_t	flags;

	if( NULL == format )
	{
		return 0u;
	}

	flags = 0;
	do
	{
		++(*format);
		switch( **format )
		{
		case '-':	// 左对齐,右边补空格
			flags |= PRINT_FLAG_LEFT_ALIGN;
			continue;
		case '+':	// 右对齐,左边补0
			flags |= PRINT_FLAG_PLUS;
			continue;
		case ' ':	// 左边补空格
			flags |= PRINT_FLAG_SPACE;
			continue;
		case '#':	// 8进制,前边加o; 16进制,前边加0x
			flags |= PRINT_FLAG_WELL_SPECIAL;
			continue;
		case '0':	// 前边补0
			flags |= PRINT_FLAG_ZEROPAD;
			continue;
		default:
			break;
		}
	}while( 0 );

	return flags;
}

int16_t ToolMiscGetWidth( const uint8_t** format, uint16_t* flags, va_list* args )
{
	int16_t	field_width = -1;

	if( NULL == format )
	{
		return field_width;
	}
	
	if( **format == '*' )
	{
		// 占位符, 对应位置参数作为宽度
		(*format)++;
		field_width = va_arg(*args, int);
		if( field_width < 0 )
		{
			field_width = -field_width;
			if( flags != NULL )
			{
				*flags |= PRINT_FLAG_LEFT_ALIGN;
			}
		}

		return field_width;
	}

	if( **format >= '0' && **format <= '9' )
	{
		field_width = 0;
		while( **format >= '0' && **format <= '9' )
		{
			field_width *= 10;
			field_width += *((*format)++) - '0';
		}
	}

	return field_width;
}

int16_t ToolMiscGetPrecision( const uint8_t** format, va_list* args )
{
	int16_t	precision = -1;

	if( NULL == format 
	||	**format != '.' )
	{
		return precision;
	}
	
	(*format)++;
	if( **format >= '0' && **format <= '9' )
	{
		precision = 0;
		while( **format >= '0' && **format <= '9' )
		{
			precision *= 10;
			precision += *((*format)++) - '0';
		}
	}
	else if( **format == '*' )
	{
		(*format)++;
		precision = va_arg(args, int);
	}
	
	if( precision < 0 )
	{
		precision = 0;
	}

	return precision;
}

int16_t ToolMiscGetQualifier( const uint8_t** format )
{
	int16_t	qualifier = -1;

	if( NULL == format )
	{
		return qualifier;
	}
	
	if( **format == 'h'							// short int
	||	**format == 'l' || **format == 'L' )	// long int

	{
		qualifier = *((*format)++);
		if( qualifier == 'l' && **format == 'l' )
		{
			qualifier = 'L';
			(*format)++;
		}
	}

	return qualifier;
}

uintptr_t ToolMiscPutChar( uintptr_t buf, uintptr_t end, uint8_t ch, int32_t field_width, uint16_t flags )
{
	if( !(flags & PRINT_FLAG_LEFT_ALIGN) )
	{
		while( --field_width > 0 )
		{
			TOOL_MISC_PUT_CH( buf, end, ' ' );
		}
	}
	TOOL_MISC_PUT_CH( buf, end, ch );
	while( --field_width > 0 )
	{
		TOOL_MISC_PUT_CH( buf, end, ' ' );
	}

	return buf;
}

static uintptr_t ToolMiscProcVarPara( uintptr_t buf, uintptr_t end, int32_t field_width, int32_t precision, uint16_t flags, va_list* args )
{
	if( NULL == buf
	||	NULL == end 
	||	NULL == args )
	{
		return ;
	}

	return ;
}

/*
* brief : 数据内容格式化成字符串后,存放到指定 buf 中
* param : dest[in/out]: 字符串存放 buffer
*             size[in]: 字符串存放 buffer 大小
*              fmt[in]: 参数格式字符串
*             args[in]: 参数列表
* result: 0: 参数非法
*        >0: 字符串长度
* remark: [标志][最少宽度][.精度] [长度] 类型
*/
static int16_t ToolMiscVsprint( uintptr_t buf, uint16_t size, const uint8_t *fmt, va_list args )
{
	int16_t		qualifier;
	uint16_t	base, flags;
	int32_t		field_width, precision;
	uint64_t	num_64;
	uintptr_t	str;
	uintptr_t	end;
	
	str = buf;
	end = buf + size;
	for( ; *fmt ; ++fmt )
	{
		if( *fmt != '%' )
		{
			TOOL_MISC_PUT_CH( str, end, *fmt );
			continue;
		}

		// 处理格式标记
		flags = ToolMiscGetFlags( &fmt );
		
		// 获取宽度
		field_width = ToolMiscGetWidth( &fmt, &flags, &args );

		// 获取精度
		precision = ToolMiscGetPrecision( &fmt, &args );

		// 获取数据转换格式
		qualifier = ToolMiscGetQualifier( &fmt );

		// 获取进制
		base = 10;
		switch( *fmt )
		{
		case 'c':   // character
			str = ToolMiscPutChar( str, end, va_arg(args, char), field_width, flags );
			continue;

		case 's':   // string
			str = ToolMiscPutString( str, end, va_arg(args, uintptr_t), field_width, precision, flags );
			continue;

		case 'p':   // void*
			str = ToolMiscPutPointer( str, end, va_arg(args, voidprt_t), field_width, precision, flags );
			continue;

		case '%':	// %
			TOOL_MISC_PUT_CH( str, end, '%' );
			continue;

		case 'o':	// octal number
			base = 8;
			break;

		case 'x':	// lower case
			flags |= PRINT_FLAG_HEX_LOWER_CASE;
		case 'X':	// upper case
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= PRINT_FLAG_SIGN_NUMBER;
		/* fallthrough */
		case 'u':
			break;

		default:
			TOOL_MISC_PUT_CH( str, end, '%' );
			if( *fmt )
			{
				TOOL_MISC_PUT_CH( str, end, *fmt );
			}
			else
			{
				--fmt;
			}
			continue;
		}
		
		if( qualifier == 'L' )  /* "quad" for 64 bit variables */
		{
			num_64 = va_arg( args, unsigned long long );
		}
		else if( qualifier == 'l' )
		{
			num_64 = va_arg( args, unsigned long );
			if( flags & PRINT_FLAG_SIGN_NUMBER )
			{
				num_64 = (signed long)num_64;
			}
		}
		else if( qualifier == 'h' )
		{
			num_64 = (unsigned short)va_arg(args, int);
			if( flags & PRINT_FLAG_SIGN_NUMBER )
			{
				num_64 = (signed short)num_64;
			}
		}
		else
		{
			num_64 = va_arg( args, unsigned int );
			if( flags & PRINT_FLAG_SIGN_NUMBER )
			{
				num_64 = (signed int)num_64;
			}
		}
		str = ToolMiscPutNumber( str, end, num_64, base, field_width, precision, flags );
	}

	if( size > 0 )
	{
		TOOL_MISC_PUT_CH( str, end, '\0' );
		if( str > end )
		{
			end[-1] = '\0';
		}
		--str;
	}
	
	return str - buf;
}

#if 0
typedef union
{
	int				i_val;
	unsigned int	u_val;

	uintptr_t		uint_ptr;
}uVarVal;

static uint16_t MyVsprint( uint8_t* buf, uint16_t size, const int8_t *fmt, va_list args )
{
#define BREAK_WHEN_CH_IS_END(ch)		\
	{									\
		if( (ch) == STRING_END_CHAR)	\
			break;						\
	}

	uint8_t		filed_prefix;
	uint16_t	str_len, field_width, ov_ofs, i;
	uVarVal		var_val;
	
	if( NULL == buf 
	||	size < 1u 
	||	NULL == fmt )
	{
		return 0u;
	}

	str_len     = 0u;
	field_width = 0u;
	filed_prefix = ' ';
	for( ; *fmt != STRING_END_CHAR && str_len < size; fmt++ )
	{
		if( *fmt != '%' )
		{
			buf[str_len++] = *fmt;
			continue;
		}

		fmt++;
		BREAK_WHEN_CH_IS_END( *fmt );
		// 前导码是0
		if( *fmt == '0' )
		{
			filed_prefix = '0';
			fmt++;
			BREAK_WHEN_CH_IS_END( *fmt );
		}

		// 字符串宽度
		while( *fmt >= '0' && *fmt <= '9' )
		{
			//紧接着的数字是长度，算出指定长度
			field_width *= 10u;
			field_width += (*fmt - '0');
			fmt++;
			BREAK_WHEN_CH_IS_END( *fmt );
		}
		BREAK_WHEN_CH_IS_END( *fmt );
		
		switch( *fmt )
		{
		case 'd':
			var_val.i_val = va_arg(args, int);
			str_len += MyDataFormatD( &buf[str_len], size - str_len, var_val.i_val, 10u, filed_prefix, field_width );
			break;
		case 'o':
			var_val.u_val = va_arg(args, unsigned int);
			str_len += MyDataFormatD( &buf[str_len], size - str_len, var_val.u_val, 8u, filed_prefix, field_width );
			break;
		case 'u':
//			out_num(va_arg(args, unsigned int)，10, ead, maxwidth);
			break;
		case 'x':
			var_val.u_val = va_arg(args, unsigned int);
			str_len += MyDataFormatD( &buf[str_len], size - str_len, var_val.u_val, 16u, filed_prefix, field_width );
			break;
		case 'c':
//			outc(va_arg(args, int));
			break;

		case 's':
			fmt++;
			BREAK_WHEN_CH_IS_END( *fmt );
			var_val.uint_ptr = va_arg(args, uintptr_t);
			for( i=str_len; i<size; i++ )
			{
				if( *var_val.uint_ptr == '\0' )
				{
					break;
				}
				buf[str_len++] = (uint8_t)(*var_val.uint_ptr++);
			}
			break;
		default:
			buf[str_len++] = *fmt++;
			break;
		}
	}

	if( str_len >= size )
	{
		ov_ofs = str_len - sizeof(ErrorBufOverFlow);
		(void)DbgPrintStrCpy( &PrintBuf[ov_ofs], sizeof(ErrorBufOverFlow), ErrorBufOverFlow );
	}

	return str_len;
}
#endif


/*
* brief : 串口发送注册函数
* param : send_char[in]: 串口发送字符函数, DbgPrintBlock函数使用
*          send_str[in]: 串口发送字符串函数, DbgPrint函数使用
* result: -1: 注册失败
*          0: 注册成功
* remark: 两个注册函数根据使用接口填写, 不使用的接口可为NULL
*/
int8_t ToolMiscPrintReg( fUartSendStr send_str )
{
	UartSendStr  = send_str;
	
	return 0;
}

/*
* brief : 通过串口打印字符串
* param : format[in]: 输出字符串格式
* result: -1: 失败, 串口发送函数未注册
*         -2: 发送失败
*        >=0: 发送成功字符数
* remark: 此函数使用 注册的 fUartSendStr 函数发送, 阻塞式和非阻塞式发送, 取决于注册函数本身
*/
int16_t ToolMiscPrint( const char *format,... )
{
#ifndef DBG_PRINT_BUF_IS_GLOBAL
	uint8_t PrintBuf[DBG_PRINT_BUF_SIZE];
#endif
	uint16_t	str_len, sent_len;
	va_list		args;
	
	if( NULL == UartSendStr )
	{
		return -1;	// 输出函数未注册
	}

	args = (va_list)NULL;
	va_start( args, format );
//	str_len  = MyVsprint( PrintBuf, sizeof(PrintBuf), format, args );
	str_len  = ToolMiscVsprint( (uintptr_t)PrintBuf, sizeof(PrintBuf), (const uint8_t *)format, args );
	sent_len = UartSendStr( PrintBuf, str_len );
	va_end( args );
	
	if( sent_len < 1 )
	{
		return -2;	// 发送失败
	}

	return (int16_t)sent_len;
}








