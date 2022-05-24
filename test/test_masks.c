#include "unity.h"
#include "masks.h"
#include "bitboard_mapping.h"

void test_pawn_move_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pawn_move_mask(SQ_A8, 0x300ULL, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pawn_move_mask(SQ_A1, 0x300ULL, WHITE));

	TEST_ASSERT_EQUAL_UINT64(
		0x4040000ULL, pawn_move_mask(SQ_C2, 0x00ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x202000000000ULL, pawn_move_mask(SQ_F7, 0x00ULL, BLACK)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x40000ULL, pawn_move_mask(SQ_C2, 0x4000000ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, pawn_move_mask(SQ_C2, 0x40000ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x400000000000ULL, pawn_move_mask(SQ_G7, 0x4000000000ULL, BLACK)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, pawn_move_mask(SQ_G7, 0x400000000000ULL, BLACK)
	);
}

void test_rook_attacks_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0xFE01010101010101ULL, test_rook_attacks_mask(SQ_A8, 0x00ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x808080808080807FULL, test_rook_attacks_mask(SQ_H1, 0x00ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x8080808F7080808ULL, test_rook_attacks_mask(SQ_D4, 0x00ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, test_rook_attacks_mask(SQ_H1, 0x8040ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0xFE00000000000000ULL, test_rook_attacks_mask(SQ_A8, 0x1000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, test_rook_attacks_mask(SQ_D4, 0x814080000ULL)
	);
}