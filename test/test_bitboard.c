#include "unity.h"
#include "bitboard.h"

void test_bit_scan_forward(void)
{
	TEST_ASSERT_EQUAL_UINT32(0, bit_scan_forward(UNIVERSE));
	TEST_ASSERT_EQUAL_UINT32(4, bit_scan_forward(0x10ull));
	TEST_ASSERT_EQUAL_UINT32(56, bit_scan_forward(0x0100000000000000ull));
}

void test_bit_scan_reverse(void)
{
	TEST_ASSERT_EQUAL_UINT32(63, bit_scan_reverse(UNIVERSE));
	TEST_ASSERT_EQUAL_UINT32(4, bit_scan_reverse(0x10ull));
	TEST_ASSERT_EQUAL_UINT32(56, bit_scan_reverse(0x0100000000000000ull));
}

void test_population_count(void)
{
	TEST_ASSERT_EQUAL_UINT32(0, population_count(EMPTY));
	TEST_ASSERT_EQUAL_UINT32(64, population_count(UNIVERSE));
	TEST_ASSERT_EQUAL_UINT32(8, population_count(0xFFULL));
	TEST_ASSERT_EQUAL_UINT32(4, population_count(0x0000F0ULL));
}

void test_remove_lsb_macro(void)
{
	U64 bitboard = 0x01ULL;
	remove_lsb(bitboard);
	TEST_ASSERT_EQUAL_UINT64(bitboard, EMPTY);

	bitboard = UNIVERSE;
	remove_lsb(bitboard);
	TEST_ASSERT_EQUAL_UINT64(bitboard, 0xFFFFFFFFFFFFFFFEULL);

	bitboard = EMPTY;
	remove_lsb(bitboard);
	TEST_ASSERT_EQUAL_UINT64(bitboard, 0x00ULL);
}
