#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"

#include <stdlib.h>

/// Because of the constant change in evaluations, tests are very difficult to
/// do. That's why testing is done on the most basic things. For example, if
/// there is an obvious white advantage, the function does not return DRAW or
/// less.

void test_get_game_phase(void)
{
	Position *pos = init_position(
		"8/8/8/p3kPp1/6P1/4K3/8/8 w - - 0 1"
	);

	TEST_ASSERT_EQUAL(ENDGAME, get_game_phase(pos));

	free(pos->state);
	free(pos);

	pos = init_position(
		"3k4/4r3/3K4/3B4/8/8/8/5R2 w - - 0 1"
	);

	TEST_ASSERT_EQUAL(ENDGAME, get_game_phase(pos));

	free(pos->state);
	free(pos);

	pos = init_position(
		"6k1/8/6PP/5K2/2B5/2b5/8/8 b - - 0 1"
	);

	TEST_ASSERT_EQUAL(ENDGAME, get_game_phase(pos));

	free(pos->state);
	free(pos);

	pos = init_position(
		"r3k2r/ppq1pp1p/2n2np1/2Pp4/1P1P2b1/2N2N2/P3BPPP/R2Q1RK1 b"
		" Qkq - 2 5"
	);

	TEST_ASSERT_EQUAL(MIDDLEGAME, get_game_phase(pos));

	free(pos->state);
	free(pos);

	pos = init_position(
		"r2q1rk1/p1pp1ppp/bbp2n2/6B1/4P3/2N3P1/PPPQ1PBP/R3R1K1 b Qq -"
		" 0 1"
	);

	TEST_ASSERT_EQUAL(MIDDLEGAME, get_game_phase(pos));

	free(pos->state);
	free(pos);
}

void test_evaluate_position()
{
	Position *pos = init_position("8/1nbk4/8/8/8/8/1QRK4/8 w - - 0 1");

	TEST_ASSERT_GREATER_THAN(DRAW, evaluate_material(pos));

	free(pos->state);
	free(pos);

	pos = init_position("8/1qrk4/8/8/8/8/1BNK4/8 w - - 0 1");

	TEST_ASSERT_LESS_THAN(DRAW, evaluate_material(pos));

	free(pos->state);
	free(pos);
}

void test_evaluate_material(void)
{
	Position *pos = init_position("8/1nbk4/8/8/8/8/1QRK4/8 w - - 0 1");

	TEST_ASSERT_GREATER_THAN(DRAW, evaluate_material(pos));

	free(pos->state);
	free(pos);

	pos = init_position("8/1qrk4/8/8/8/8/1BNK4/8 w - - 0 1");

	TEST_ASSERT_LESS_THAN(DRAW, evaluate_material(pos));

	free(pos->state);
	free(pos);
}

void test_evaluate_central_pawns(void)
{
	Position *pos = init_position("K7/8/8/4p3/3PP3/2P5/8/k7 w - - 0 1");

	TEST_ASSERT_GREATER_THAN(DRAW, evaluate_central_pawns(pos));

	free(pos->state);
	free(pos);

	pos = init_position("K7/8/2p2p2/3pp3/4P3/2P5/8/k7 w - - 0 1");

	TEST_ASSERT_LESS_THAN(DRAW, evaluate_central_pawns(pos));

	free(pos->state);
	free(pos);
}

void test_evaluate_doubled_pawns(void)
{
	Position *pos = init_position(
		"4k3/pp1pppp1/3p2p1/8/8/8/PPPPPPPP/4K3 w - - 0 1"
	);

	TEST_ASSERT_GREATER_THAN(DRAW, evaluate_doubled_pawns(pos));

	free(pos->state);
	free(pos);

	pos = init_position(
		"4k3/pp1pppp1/3p2p1/8/8/P2P2P1/P1PP1PP1/4K3 w - - 0 1"
	);

	TEST_ASSERT_LESS_THAN(DRAW, evaluate_doubled_pawns(pos));

	free(pos->state);
	free(pos);
}
