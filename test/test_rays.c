#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "rays.h"

void test_get_ray_west(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x30000ULL, get_ray_west(SQ_C3));
	TEST_ASSERT_EQUAL_UINT64(0x0000000000000000ULL, get_ray_west(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(0x7F00000000000000ULL, get_ray_west(SQ_H8));
}

void test_get_ray_east(void)
{
	TEST_ASSERT_EQUAL_UINT64(0xFE00000000000000ULL, get_ray_east(SQ_A8));
	TEST_ASSERT_EQUAL_UINT64(0xF0000000ULL, get_ray_east(SQ_D4));
	TEST_ASSERT_EQUAL_UINT64(0x0000000000000000ULL, get_ray_east(SQ_H1));
}

void test_get_ray_north(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x0000000000000000ULL, get_ray_north(SQ_C8));
	TEST_ASSERT_EQUAL_UINT64(0x2020202020200000ULL, get_ray_north(SQ_F2));
	TEST_ASSERT_EQUAL_UINT64(0x101010101010100ULL, get_ray_north(SQ_A1));
}

void test_get_ray_south(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x0000000000000000ULL, get_ray_south(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(0x202ULL, get_ray_south(SQ_B3));
	TEST_ASSERT_EQUAL_UINT64(0x80808080808080ULL, get_ray_south(SQ_H8));
}

void test_get_ray_diagonal(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x80402010080402ULL, get_ray_diagonal(SQ_B1));
	TEST_ASSERT_EQUAL_UINT64(
		0x100000000000000ULL, get_ray_diagonal(SQ_A8)
	);
	TEST_ASSERT_EQUAL_UINT64(0x80ULL, get_ray_diagonal(SQ_H1));
}

void test_get_ray_anti_diagonal(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0X8000000000000000ULL, get_ray_anti_diagonal(SQ_H8)
	);
	TEST_ASSERT_EQUAL_UINT64(0X1ULL, get_ray_anti_diagonal(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(
		0X102040810204080ULL, get_ray_anti_diagonal(SQ_A8)
	);
}

void test_get_ray_south_west(void)
{
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_south_west(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(0X402010ULL, get_ray_south_west(SQ_H4));
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_south_west(SQ_A8));
}

void test_get_ray_north_east(void)
{
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_north_east(SQ_H1));
	TEST_ASSERT_EQUAL_UINT64(
		0X8040201008040200ULL, get_ray_north_east(SQ_A1)
	);
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_north_east(SQ_H8));
}

void test_get_ray_north_west(void)
{
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_north_west(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(
		0X102040810204000ULL, get_ray_north_west(SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_north_west(SQ_A8));
}

void test_get_ray_south_east(void)
{
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_south_east(SQ_H8));
	TEST_ASSERT_EQUAL_UINT64(
		0X2040810204080ULL, get_ray_south_east(SQ_A8)
	);
	TEST_ASSERT_EQUAL_UINT64(0X0ULL, get_ray_south_east(SQ_H1));
}

void test_get_ray_between(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x7EULL, get_ray_between(SQ_A1, SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x40404040000ULL, get_ray_between(SQ_C2, SQ_C7)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x2040810204000ULL, get_ray_between(SQ_A8, SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x8040200ULL, get_ray_between(SQ_A1, SQ_E5)
	);
	TEST_ASSERT_EQUAL_UINT64(
		get_ray_between(SQ_A3, SQ_F3), get_ray_between(SQ_F3, SQ_A3)
	);
}

void test_init_rays(void)
{
	init_rays();

	TEST_ASSERT_EQUAL_UINT64(
		ray_between[SQ_C3][SQ_C8], get_ray_between(SQ_C3, SQ_C8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		ray_vertical[SQ_C3],
		(
			get_ray_north(SQ_C3)
			| get_ray_south(SQ_C3)
			| square_to_bitboard(SQ_C3)
		)
	);
	TEST_ASSERT_EQUAL_UINT64(
		ray_horizontal[SQ_C3],
		(
			get_ray_east(SQ_C3)
			| get_ray_west(SQ_C3)
			| square_to_bitboard(SQ_C3)
		)
	);

	TEST_ASSERT_EQUAL_UINT64(ray_west[SQ_C3], get_ray_west(SQ_C3));
	TEST_ASSERT_EQUAL_UINT64(ray_east[SQ_C3], get_ray_east(SQ_C3));
	TEST_ASSERT_EQUAL_UINT64(ray_south[SQ_C3], get_ray_south(SQ_C3));
	TEST_ASSERT_EQUAL_UINT64(ray_north[SQ_C3], get_ray_north(SQ_C3));

	TEST_ASSERT_EQUAL_UINT64(
		ray_south_east[SQ_C3], get_ray_south_east(SQ_C3)
	);
	TEST_ASSERT_EQUAL_UINT64(
		ray_south_west[SQ_C3], get_ray_south_west(SQ_C3)
	);
	TEST_ASSERT_EQUAL_UINT64(
		ray_north_east[SQ_C3], get_ray_north_east(SQ_C3)
	);
	TEST_ASSERT_EQUAL_UINT64(
		ray_north_west[SQ_C3], get_ray_north_west(SQ_C3)
	);

	TEST_ASSERT_EQUAL_UINT64(
		ray_diagonal[SQ_C3], get_ray_diagonal(SQ_C3)
	);
	TEST_ASSERT_EQUAL_UINT64(
		ray_anti_diagonal[SQ_C3], get_ray_anti_diagonal(SQ_C3)
	);
}
