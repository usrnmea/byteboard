#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "evaluate.h"
#include "movegen.h"
#include "position.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
#include "rays.h"
#include "search.h"
#include "uci.h"

#include <stdlib.h>

void test_str_to_move(void)
{
	Position *pos = init_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R3K2R w KQ - 0 10"
	);

	Move move_1 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_G1
	};

	Move test_move_1 = str_to_move(pos, "e1g1");

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &test_move_1, sizeof(move_1));

	free(pos->state);
	free(pos);

	pos = init_position("6k1/8/8/2p5/3p4/8/3PP3/1K6 w - - 0 1");

	Move move_double_push = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};

	Move move_2 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_D4, .destination = SQ_E3
	};

	do_move(pos, move_double_push);

	Move test_move_2 = str_to_move(pos, "d4e3");

	TEST_ASSERT_EQUAL_MEMORY(&move_2, &test_move_2, sizeof(move_2));

	free(pos->state);
	free(pos);

	pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};

	Move test_move_3 = str_to_move(pos, "e2e4");

	TEST_ASSERT_EQUAL_MEMORY(&move_3, &test_move_3, sizeof(move_3));

	free(pos->state);
	free(pos);

	pos = init_position(
		"5r2/1k4PN/8/q7/8/8/8/3KR3 w - - 0 1"
	);

	Move move_4 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	Move test_move_4 = str_to_move(pos, "g7f8n");

	TEST_ASSERT_EQUAL_MEMORY(&move_4, &test_move_4, sizeof(move_4));

	free(pos->state);
	free(pos);
}

void test_move_to_str(void)
{
	Move move_1 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_G1
	};

	Move move_2 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_D4, .destination = SQ_E3
	};

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};

	Move move_4 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	char str_1[5], str_2[5], str_3[5], str_4[6];

	str_1[4] = '\0';
	str_2[4] = '\0';
	str_3[4] = '\0';
	str_4[5] = '\0';

	move_to_str(move_1, str_1);
	move_to_str(move_2, str_2);
	move_to_str(move_3, str_3);
	move_to_str(move_4, str_4);

	TEST_ASSERT_EQUAL_STRING("e1g1", str_1);
	TEST_ASSERT_EQUAL_STRING("d4e3", str_2);
	TEST_ASSERT_EQUAL_STRING("e2e4", str_3);
	TEST_ASSERT_EQUAL_STRING("g7f8n", str_4);
}

void test_get_position(void) 
{
	Position *pos = get_position("position startpos");

	TEST_ASSERT_EQUAL_UINT64(0xFFFF00000000FFFF, pos->state->occupied);

	free(pos->state);
	free(pos);

	pos = get_position("position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
	TEST_ASSERT_EQUAL_UINT64(0x917D731812A4FF91, pos->state->occupied);

	free(pos->state);
	free(pos);

	pos = get_position("position startpos moves e2e4 e7e5");
	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101000EFFF, pos->state->occupied);

	free(pos->state);
	free(pos);

	pos = get_position("position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves e2a6");
	TEST_ASSERT_EQUAL_UINT64(0x917D731812A4EF91, pos->state->occupied);

	free(pos->state);
	free(pos);

	pos = get_position("position fen 5r2/k5P1/8/8/8/8/8/2K5 w - - 0 1 moves g7f8q");
	TEST_ASSERT_EQUAL_UINT64(0x2001000000000004, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x2000000000000000, pos->board.WhiteQueens);

	free(pos->state);
	free(pos);
}