#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "rays.h"
#include "patterns.h"

// Initializing everything needed for tests
void test_patterns_init(void)
{
	init_rays();
}

void test_pawn_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x2020000ULL, white_pawn_move_pattern(SQ_B2));
	TEST_ASSERT_EQUAL_UINT64(0x02ULL, black_pawn_move_pattern(SQ_B2));

	TEST_ASSERT_EQUAL_UINT64(
		0x8000000000000000ULL, white_pawn_move_pattern(SQ_H7)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x808000000000ULL, black_pawn_move_pattern(SQ_H7)
	);
}

void test_pawn_attack_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x50000ULL, white_pawn_attack_pattern(SQ_B2));
	TEST_ASSERT_EQUAL_UINT64(0x05ULL, black_pawn_attack_pattern(SQ_B2));

	TEST_ASSERT_EQUAL_UINT64(
		0x40000000000000ULL, white_pawn_attack_pattern(SQ_H6)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x2000000000000ULL, white_pawn_attack_pattern(SQ_A6)
	);

	TEST_ASSERT_EQUAL_UINT64(0x40ULL, black_pawn_attack_pattern(SQ_H2));
	TEST_ASSERT_EQUAL_UINT64(0x02ULL, black_pawn_attack_pattern(SQ_A2));
}

void test_knight_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(0x402000ULL, knight_move_pattern(SQ_H1));
	TEST_ASSERT_EQUAL_UINT64(0x20400ULL, knight_move_pattern(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(
		0x4020000000000ULL, knight_move_pattern(SQ_A8)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x20400000000000ULL, knight_move_pattern(SQ_H8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x14220022140000ULL, knight_move_pattern(SQ_D5)
	);
}

void test_king_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(0xC040ULL, king_move_pattern(SQ_H1));
	TEST_ASSERT_EQUAL_UINT64(
		0x40C0000000000000ULL, king_move_pattern(SQ_H8)
	);
	TEST_ASSERT_EQUAL_UINT64(0x302ULL, king_move_pattern(SQ_A1));
	TEST_ASSERT_EQUAL_UINT64(
		0x203000000000000ULL, king_move_pattern(SQ_A8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x705070000000ULL, king_move_pattern(SQ_F5)
	);
}

void test_rook_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x808080808080807FULL, rook_move_pattern(SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x7F80808080808080ULL, rook_move_pattern(SQ_H8)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x1010101010101FEULL, rook_move_pattern(SQ_A1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0xFE01010101010101ULL, rook_move_pattern(SQ_A8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x202020DF20202020ULL, rook_move_pattern(SQ_F5)
	);
}

void test_bishop_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x102040810204000ULL, bishop_move_pattern(SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x40201008040201ULL, bishop_move_pattern(SQ_H8)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x8040201008040200ULL, bishop_move_pattern(SQ_A1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x2040810204080ULL, bishop_move_pattern(SQ_A8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x488500050880402ULL, bishop_move_pattern(SQ_F5)
	);
}

void test_queen_move_pattern(void)
{
	TEST_ASSERT_EQUAL_UINT64(
		0x8182848890A0c07FULL, queen_move_pattern(SQ_H1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x7Fc0a09088848281ULL, queen_move_pattern(SQ_H8)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0x81412111090503FEULL, queen_move_pattern(SQ_A1)
	);
	TEST_ASSERT_EQUAL_UINT64(
		0xFE03050911214181ULL, queen_move_pattern(SQ_A8)
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x24A870DF70A82422ULL, queen_move_pattern(SQ_F5)
	);
}
