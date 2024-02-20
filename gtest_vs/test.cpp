#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

#if 1//≤‚ ‘Deque

#include "deque.h"

TEST(TestCaseDeque, Init) {
	sDeque deque1 = { 0 };
	uint8_t buff[200] = { 0 };

	{	/* ≥ı ºªØ */

		EXPECT_NE(0, DequeInit(NULL, buff, sizeof(buff)));
		EXPECT_NE(0, DequeInit(&deque1, NULL, sizeof(buff)));
		EXPECT_NE(0, DequeInit(&deque1, buff, 0));
		EXPECT_EQ(0, DequeInit(&deque1, buff, sizeof(buff)));
	}
}
#endif

#if 1//≤‚ ‘MathDes
#include "math_des.h"

TEST(TestCaseMathDes, ENC)
{
	uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint8_t key[8] = { 7,6,5,4,3,2,1,0 };
	for (size_t i = 0; i < 1000; i++)
	{
		MathDes(data, key, 0);
	}
	/*for (size_t i = 0; i < 1000; i++)
	{
		MathDes(data, key, 1);
	}*/
}

#endif

#if 1//≤‚ ‘CommonDes
#include "common_des.h"

TEST(TestCaseCommonDes, ENC)
{
	uint64_t data_u64 = 0xED8EDC49AF;
	uint64_t key_u64 = 0x282262B772D44;
	for (size_t i = 0; i < 1000; i++)
	{
		des(data_u64, key_u64, 0);
	}
	/*for (size_t i = 0; i < 1000; i++)
	{
		MathDes(data, key, 1);
	}*/
}

#endif