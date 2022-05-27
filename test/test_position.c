#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"

#include <stdlib.h>

void test_init_position(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/6K1 w - - 0 1");

	TEST_ASSERT_NOT_NULL(pos);

	TEST_ASSERT_EQUAL_UINT64(
		0x9002000100800440ULL, pos->position_state->occupied
	);
	
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);

	TEST_ASSERT_EQUAL_UINT64(0x800400ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x100000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(
		0x8002000000000000ULL, pos->board.BlackBishops
	);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	free(pos->position_state);
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

	pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	TEST_ASSERT_NOT_NULL(pos);

	TEST_ASSERT_EQUAL_UINT64(
		0xFFFF00000000FFFFULL, pos->position_state->occupied
	);

	TEST_ASSERT_EQUAL_UINT64(0xFF00ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x42ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x24ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0xFF000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x4200000000000000ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x2400000000000000ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x800000000000000ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);

	free(pos->position_state);
	free(pos);
}

void test_pieces(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/6K1 w - - 0 1");

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, W_PAWN));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, W_KNIGHT));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, W_ROOK));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, W_QUEEN));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, B_KNIGHT));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, B_ROOK));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pieces(pos, B_QUEEN));

	TEST_ASSERT_EQUAL_UINT64(0x800400ULL, pieces(pos, W_BISHOP));
	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pieces(pos, W_KING));
	TEST_ASSERT_EQUAL_UINT64(0x100000000ULL, pieces(pos, B_PAWN));
	TEST_ASSERT_EQUAL_UINT64(
		0x8002000000000000ULL, pieces(pos, B_BISHOP)
	);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pieces(pos, B_KING));

	free(pos->position_state);
	free(pos);

	pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	TEST_ASSERT_EQUAL_UINT64(0xFF00ULL, pieces(pos, W_PAWN));
	TEST_ASSERT_EQUAL_UINT64(0x42ULL, pieces(pos, W_KNIGHT));
	TEST_ASSERT_EQUAL_UINT64(0x24ULL, pieces(pos, W_BISHOP));
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pieces(pos, W_ROOK));
	TEST_ASSERT_EQUAL_UINT64(0x8ULL, pieces(pos, W_QUEEN));
	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pieces(pos, W_KING));
	TEST_ASSERT_EQUAL_UINT64(0xFF000000000000ULL, pieces(pos, B_PAWN));
	TEST_ASSERT_EQUAL_UINT64(0x4200000000000000ULL, pieces(pos, B_KNIGHT));
	TEST_ASSERT_EQUAL_UINT64(0x2400000000000000ULL, pieces(pos, B_BISHOP));
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pieces(pos, B_ROOK));
	TEST_ASSERT_EQUAL_UINT64(0x800000000000000ULL, pieces(pos, B_QUEEN));
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pieces(pos, B_KING));

	free(pos->position_state);
	free(pos);
}

void test_set_piece(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/6K1 w - - 0 1");
	
	set_piece(pos, W_PAWN, SQ_E5);
	set_piece(pos, B_ROOK, SQ_A8);
	set_piece(pos, B_QUEEN, SQ_A2);
	set_piece(pos, W_KNIGHT, SQ_H5);
	
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);

	TEST_ASSERT_EQUAL_UINT64(0x800400ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x100000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(
		0x8002000000000000ULL, pos->board.BlackBishops
	);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x100000000000000ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x100ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x8000000000ULL, pos->board.WhiteKnights);

	free(pos->position_state);
	free(pos);
}

void test_remove_piece(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/6K1 w - - 0 1");

	remove_piece(pos, W_PAWN, SQ_A1);
	remove_piece(pos, W_BISHOP, SQ_C2);
	remove_piece(pos, B_PAWN, SQ_A5);
	remove_piece(pos, B_BISHOP, SQ_B7);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackPawns);

	TEST_ASSERT_EQUAL_UINT64(0x800000ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(
		0x8000000000000000ULL, pos->board.BlackBishops
	);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);

	free(pos->position_state);
	free(pos);
}

void test_move_piece(void)
{
	Position *pos = init_position("4k2b/1b6/8/p7/8/7B/2B5/6K1 w - - 0 1");

	move_piece(pos, W_PAWN, SQ_A2, SQ_A3);
	move_piece(pos, W_KING, SQ_G1, SQ_F2);
	move_piece(pos, B_BISHOP, SQ_B7, SQ_C8);
	move_piece(pos, W_BISHOP, SQ_C2, SQ_B1);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);

	TEST_ASSERT_EQUAL_UINT64(0x800002ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x2000ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x100000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(
		0x8400000000000000ULL, pos->board.BlackBishops
	);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x10000ULL, pos->board.WhitePawns);

	free(pos->position_state);
	free(pos);
}

void test_piece_on(void)
{
	Position *pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	TEST_ASSERT_EQUAL(W_PAWN, piece_on(pos, SQ_A2));
	TEST_ASSERT_EQUAL(W_ROOK, piece_on(pos, SQ_A1));
	TEST_ASSERT_EQUAL(W_KING, piece_on(pos, SQ_E1));

	TEST_ASSERT_EQUAL(B_PAWN, piece_on(pos, SQ_A7));
	TEST_ASSERT_EQUAL(B_ROOK, piece_on(pos, SQ_A8));
	TEST_ASSERT_EQUAL(B_QUEEN, piece_on(pos, SQ_D8));

	free(pos->position_state);
	free(pos);
}

void test_attaked_by(void)
{
	Position *pos = init_position(
		"8/6b1/3p4/r1r1K3/8/5n2/4q3/k7 w - - 0 1"
	);

	TEST_ASSERT_EQUAL_UINT64(
		0x40080400201000ULL, attacked_by(pos, SQ_E5, BLACK)
	);
	TEST_ASSERT_EQUAL_UINT64(EMPTY, attacked_by(pos, SQ_D6, BLACK));
	TEST_ASSERT_EQUAL_UINT64(
		0x1000000000ULL, attacked_by(pos, SQ_D6, WHITE)
	);

	free(pos->position_state);
	free(pos);
}
