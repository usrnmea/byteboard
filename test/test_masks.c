#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"

void test_masks_init(void)
{
	init_rays();
}

void test_pawn_move_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pawn_move_mask(SQ_A8, 0x100000000000300ULL, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pawn_move_mask(SQ_A1, 0x301ULL, WHITE));

	TEST_ASSERT_EQUAL_UINT64(
		0x4040000ULL, pawn_move_mask(SQ_C2, 0x400ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x202000000000ULL, pawn_move_mask(SQ_F7, 0x20000000000000ULL, BLACK)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x40000ULL, pawn_move_mask(SQ_C2, 0x4000400ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, pawn_move_mask(SQ_C2, 0x40400ULL, WHITE)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x400000000000ULL, pawn_move_mask(SQ_G7, 0x40004000000000ULL, BLACK)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x00ULL, pawn_move_mask(SQ_G7, 0x40400000000000ULL, BLACK)
	);
}

void test_rook_attacks_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0xFE01010101010101ULL, rook_attacks_mask(SQ_A8, 0x100000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x808080808080807FULL, rook_attacks_mask(SQ_H1, 0x80ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x8080808F7080808ULL, rook_attacks_mask(SQ_D4, 0x8000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x8040ULL, rook_attacks_mask(SQ_H1, 0x80C0ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0xFE01000000000000ULL, rook_attacks_mask(SQ_A8, 0x101000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x814080000ULL, rook_attacks_mask(SQ_D4, 0x81C080000ULL)
	);
}

void test_bishop_attacks_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x2040810204080ULL, bishop_attacks_mask(SQ_A8, 0x100000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x40201008040201ULL, bishop_attacks_mask(SQ_H8, 0x8000000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x8041221400142241ULL, bishop_attacks_mask(SQ_D4, 0x8000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x200ULL, bishop_attacks_mask(SQ_A1, 0x201ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x80402010080500ULL, bishop_attacks_mask(SQ_B1, 0x102ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x500ULL, bishop_attacks_mask(SQ_B1, 0x502ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x1400140000ULL, bishop_attacks_mask(SQ_D4, 0x1408140000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x1400142241ULL, bishop_attacks_mask(SQ_D4, 0x1408000000ULL)
	);
}

void test_queen_attacks_mask(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x81412111090503FEULL, queen_attacks_mask(SQ_A1, 0x1ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x40C0A09088848281ULL, queen_attacks_mask(SQ_H8, 0xC000000000000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x88492A1CF71C2A49ULL, queen_attacks_mask(SQ_D4, 0x8000000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x8041221C141C2241ULL, queen_attacks_mask(SQ_D4, 0x81C080000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x808081CF71C0808ULL, queen_attacks_mask(SQ_D4, 0x1400140000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x20202020202070DFULL, queen_attacks_mask(SQ_F1, 0x5020ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x1C141C0000ULL, queen_attacks_mask(SQ_D4, 0x1C1C1C0000ULL)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x302ULL, queen_attacks_mask(SQ_A1, 0x303ULL)
	);
}