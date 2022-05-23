#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"

void test_square_to_bitboard_macro(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x01ULL, square_to_bitboard(SQ_A1));

	TEST_ASSERT_EQUAL_UINT64(
		0x8000000000000000ULL,
		square_to_bitboard(SQ_H8)
	);

	TEST_ASSERT_EQUAL_UINT64(0x04ULL, square_to_bitboard(SQ_C1));

	TEST_ASSERT_EQUAL_UINT64(
		0x0000200000000000ULL, 
		square_to_bitboard(SQ_F6)
	);
}
