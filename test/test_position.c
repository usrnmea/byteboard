#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"

#include <stdlib.h>

void test_init_position(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/2p3K1 w - - 0 1");

	TEST_ASSERT_NOT_NULL(pos);

	TEST_ASSERT_EQUAL_UINT64(0x9002000100800444ULL, pos->position_state->occupied);
	
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[0]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[1]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[3]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[4]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[7]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[9]);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.pieces[10]);

	TEST_ASSERT_EQUAL_UINT64(0x800400ULL, pos->board.pieces[2]);
	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pos->board.pieces[5]);
	TEST_ASSERT_EQUAL_UINT64(0x100000004ULL, pos->board.pieces[6]);
	TEST_ASSERT_EQUAL_UINT64(0x8002000000000000ULL, pos->board.pieces[8]);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.pieces[11]);
	free(pos);

	pos = init_position("this is not valid FEN");

	TEST_ASSERT_NULL(pos);
	free(pos);

	pos = init_position("2q5/8/8/8/8/8/8/6Q1 w - - 0 1");

	TEST_ASSERT_NULL(pos);
	free(pos);

	pos = init_position("1k6/8/8/2PP4/2P1P3/5Q2/8/8 w - - 0 1");

	TEST_ASSERT_NULL(pos);
	free(pos);

	pos = init_position("1k6/8/8/6K1/8/8/8/P5p1 w - - 0 1");

	TEST_ASSERT_NULL(pos);
	free(pos);

	pos = init_position("1k3pp1/8/8/6K1/8/8/8/8 w - - 0 1");

	TEST_ASSERT_NULL(pos);
	free(pos);

	pos = init_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	TEST_ASSERT_NOT_NULL(pos);

	TEST_ASSERT_EQUAL_UINT64(0xFFFF00000000FFFFULL, pos->position_state->occupied);

	TEST_ASSERT_EQUAL_UINT64(0xFF00ULL, pos->board.pieces[0]);
	TEST_ASSERT_EQUAL_UINT64(0x42ULL, pos->board.pieces[1]);
	TEST_ASSERT_EQUAL_UINT64(0x24ULL, pos->board.pieces[2]);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.pieces[3]);
	TEST_ASSERT_EQUAL_UINT64(0x8ULL, pos->board.pieces[4]);
	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.pieces[5]);
	TEST_ASSERT_EQUAL_UINT64(0xFF000000000000ULL, pos->board.pieces[6]);
	TEST_ASSERT_EQUAL_UINT64(0x4200000000000000ULL, pos->board.pieces[7]);
	TEST_ASSERT_EQUAL_UINT64(0x2400000000000000ULL, pos->board.pieces[8]);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.pieces[9]);
	TEST_ASSERT_EQUAL_UINT64(0x800000000000000ULL, pos->board.pieces[10]);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.pieces[11]);
	free(pos);
}