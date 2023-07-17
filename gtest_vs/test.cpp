#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);

}

#if 1//测试Deque

TEST(TestCaseDeque, Init) {
	sDeque deque1 = {0};
	uint8_t buff[200] = {0};

	{	/* 初始化 */
		
		EXPECT_NE(0, DequeInit(NULL, buff, sizeof(buff)));
		EXPECT_NE(0, DequeInit(&deque1, NULL, sizeof(buff)));
		EXPECT_NE(0, DequeInit(&deque1, buff, 0));
		EXPECT_EQ(0, DequeInit(&deque1, buff, sizeof(buff)));
	}
}

TEST(TestCaseDeque, Clear) {
	sDeque deque1 = {0};
	uint8_t buff[200] = {0};

	{	/* 清除 */
		EXPECT_NE(0, DequeClear(NULL));
		EXPECT_NE(0, DequeClear(&deque1));

		EXPECT_EQ(0, DequeInit(&deque1, buff, sizeof(buff)));
		EXPECT_EQ(0, DequeClear(&deque1));
	}
}

TEST(TestCaseDeque, GetFreeSize) {
	sDeque deque1 = { 0 };
	uint8_t buff[200] = { 0 };

	{	/* 获取空闲 */
		EXPECT_EQ(0, DequeGetFreeSize(NULL));
		EXPECT_EQ(0, DequeGetFreeSize(&deque1));

		EXPECT_EQ(0, DequeInit(&deque1, buff, sizeof(buff)));
		EXPECT_EQ(sizeof(buff), DequeGetFreeSize(&deque1));

		uint8_t buff1[8] = { 1,2,3,4,5,6,7,8 };
		DequePushFrontBytes(&deque1, buff1, 1);
		DequePushFrontBytes(&deque1, buff1, 2);
		DequePushBackBytes(&deque1, buff1, 1);
		DequePushBackBytes(&deque1, buff1, 2);
		EXPECT_EQ(sizeof(buff)-6, DequeGetFreeSize(&deque1));
	}
}

TEST(TestCaseDeque, GetUsedSize) {
	sDeque deque1 = { 0 };
	uint8_t buff[200] = { 0 };

	{	/* 获取已用 */
		EXPECT_EQ(0, DequeGetUsedSize(NULL));
		EXPECT_EQ(0, DequeGetUsedSize(&deque1));

		EXPECT_EQ(0, DequeInit(&deque1, buff, sizeof(buff)));
		EXPECT_EQ(0, DequeGetUsedSize(&deque1));

		uint8_t buff1[8] = { 1,2,3,4,5,6,7,8 };
		DequePushFrontBytes(&deque1, buff1, 1);
		DequePushFrontBytes(&deque1, buff1, 2);
		DequePushBackBytes(&deque1, buff1, 1);
		DequePushBackBytes(&deque1, buff1, 2);
		EXPECT_EQ(6, DequeGetUsedSize(&deque1));
	}
}

TEST(TestCaseDeque, PushFrontBytes) {
	sDeque deque1 = { 0 };
	uint8_t buff_pool[200] = { 0 };
	uint8_t buff_data[8] = { 1,2,3,4,5,6,7,8 };

	{	/* 从前插入 */
		EXPECT_NE(0, DequePushFrontBytes(NULL, buff_data, 2));
		EXPECT_NE(0, DequePushFrontBytes(&deque1, NULL, 2));
		EXPECT_NE(0, DequePushFrontBytes(&deque1, buff_data, 0));

		EXPECT_EQ(0, DequeInit(&deque1, buff_pool, sizeof(buff_pool)));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data[0], 1));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data[1], 2));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data[3], 1));
		EXPECT_NE(0, DequePushFrontBytes(&deque1, &buff_data[5], 200));
	}
}

TEST(TestCaseDeque, PushBackBytes) {
	sDeque deque1 = { 0 };
	uint8_t buff_pool[200] = { 0 };
	uint8_t buff_data_in[200] = { 1,2,3,4,5,6,7,8 };
	uint8_t buff_data_out[200] = { 0 };

	{	/* 从后插入 */
		EXPECT_NE(0, DequePushBackBytes(NULL, buff_pool, 2));
		EXPECT_NE(0, DequePushBackBytes(&deque1, NULL, 2));
		EXPECT_NE(0, DequePushBackBytes(&deque1, buff_pool, 0));

		EXPECT_EQ(0, DequeInit(&deque1, buff_pool, sizeof(buff_pool)));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[0], 1));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[1], 2));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[3], 1));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[5], 150));
		EXPECT_EQ(0, DequePopFrontBytes(&deque1, &buff_data_out[0], 150));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[5], 150));
		EXPECT_NE(0, DequePushBackBytes(&deque1, &buff_data_in[5], 150));
	}
}

TEST(TestCaseDeque, PopFrontBytes) {
	sDeque deque1 = { 0 };
	uint8_t buff_pool[200] = { 0 };
	uint8_t buff_data[8] = { 1,2,3,4,5,6,7,8 };

	{	/* 从前取出 */
		EXPECT_NE(0, DequePopFrontBytes(NULL, buff_data, 2));
		EXPECT_NE(0, DequePopFrontBytes(&deque1, NULL, 2));
		EXPECT_NE(0, DequePopFrontBytes(&deque1, buff_data, 0));

		EXPECT_EQ(0, DequeInit(&deque1, buff_pool, sizeof(buff_pool)));
		EXPECT_NE(0, DequePopFrontBytes(&deque1, &buff_data[0], 1));

		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data[1], 2));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data[1], 2));

		EXPECT_EQ(0, DequePopFrontBytes(&deque1, &buff_data[1], 2));
		EXPECT_EQ(0, DequePopFrontBytes(&deque1, &buff_data[3], 1));
		EXPECT_NE(0, DequePopFrontBytes(&deque1, &buff_data[5], 200));
	}
}

TEST(TestCaseDeque, PopBackBytes) {
	sDeque deque1 = { 0 };
	uint8_t buff_pool[200] = { 0 };
	uint8_t buff_data_in[200] = { 1,2,3,4,5,6,7,8 };
	uint8_t buff_data_out[200] = { 0 };

	{	/* 从后取出 */
		EXPECT_NE(0, DequePopBackBytes(NULL, buff_pool, 2));
		EXPECT_NE(0, DequePopBackBytes(&deque1, NULL, 2));
		EXPECT_NE(0, DequePopBackBytes(&deque1, buff_pool, 0));

		EXPECT_EQ(0, DequeInit(&deque1, buff_pool, sizeof(buff_pool)));
		EXPECT_NE(0, DequePopBackBytes(&deque1, &buff_data_out[0], 1));

		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[0], 1));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[1], 2));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[3], 1));
		EXPECT_EQ(0, DequePopBackBytes(&deque1, &buff_data_out[0], 3));

		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[0], 150));
		EXPECT_EQ(0, DequePopBackBytes(&deque1, &buff_data_out[0], 150));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[0], 150));
		EXPECT_EQ(0, DequePopBackBytes(&deque1, &buff_data_out[0], 150));
		EXPECT_NE(0, DequePopBackBytes(&deque1, &buff_data_out[0], 200));
	}
}

TEST(TestCaseDeque, Exmpale) {
	sDeque deque1 = { 0 };
	uint8_t buff_pool[200] = { 0 };
	uint8_t buff_data_in[200] = { 1,2,3,4,5,6,7,8 };
	uint8_t buff_data_out[200] = { 0 };

	{	/* 从后取出 */
		EXPECT_EQ(0, DequeInit(&deque1, buff_pool, sizeof(buff_pool)));
		EXPECT_EQ(sizeof(buff_pool), DequeGetFreeSize(&deque1));
		EXPECT_EQ(0, DequeGetUsedSize(&deque1));

		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[0], 1));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[1], 1));
		EXPECT_EQ(sizeof(buff_pool) - 2, DequeGetFreeSize(&deque1));
		EXPECT_EQ(2, DequeGetUsedSize(&deque1));

		EXPECT_EQ(0, DequeClear(&deque1));
		EXPECT_EQ(sizeof(buff_pool), DequeGetFreeSize(&deque1));
		EXPECT_EQ(0, DequeGetUsedSize(&deque1));


		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[1], 2));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[3], 1));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[1], 2));
		EXPECT_EQ(0, DequePushFrontBytes(&deque1, &buff_data_in[3], 1));

		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[0], 1));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[1], 2));
		EXPECT_EQ(0, DequePushBackBytes(&deque1, &buff_data_in[3], 1));

		EXPECT_EQ(0, DequePopBackBytes(&deque1, &buff_data_out[0], 10));
	}
}

#endif//测试Deque
