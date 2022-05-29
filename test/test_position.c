#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"

#include <stdlib.h>

// Initializing everything needed for tests
void test_init(void)
{
	init_rays();
}

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

void test_do_move(void)
{
	Position *pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	Move move = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};
	do_move(pos, move);

	TEST_ASSERT_EQUAL_UINT64(0xFFFF00001000EFFF, pos->position_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->position_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xFFFF000000000000, pos->position_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->position_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->position_state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->position_state->move_50_rule);


	Move move_2 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E7, .destination = SQ_E5
	};
	do_move(pos, move_2);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101000EFF, pos->position_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->position_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->position_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->position_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->position_state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->position_state->move_50_rule);

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_F3
	};
	do_move(pos, move_3);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101020EFBF, pos->position_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->position_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->position_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->position_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->position_state->castling);
	TEST_ASSERT_EQUAL_UINT32(1, pos->position_state->move_50_rule);

	Move move_4 = {
		.move_type = COMMON, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_E7
	};
	do_move(pos, move_4);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101020EFBF, pos->position_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF001000000000, pos->position_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->position_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->position_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->position_state->castling);
	TEST_ASSERT_EQUAL_UINT32(2, pos->position_state->move_50_rule);

	Move move_5 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_F3, .destination = SQ_E5
	};
	do_move(pos, move_5);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101000EFBF, pos->position_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x100000EFBF, pos->position_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF000000000000, pos->position_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(B_PAWN, pos->position_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->position_state->castling);
	TEST_ASSERT_EQUAL_UINT32(3, pos->position_state->move_50_rule);

	// Tests for previous_state
	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101020EFBF, pos->position_state->previous_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF001000000000, pos->position_state->previous_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->position_state->previous_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->position_state->previous_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->position_state->previous_state->castling);
	TEST_ASSERT_EQUAL_UINT32(2, pos->position_state->previous_state->move_50_rule);

	// Tests for previous_move
	TEST_ASSERT_EQUAL_UINT32(COMMON, pos->position_state->previous_move.move_type);
	TEST_ASSERT_EQUAL_UINT32(KING, pos->position_state->previous_move.moved_piece_type);
	TEST_ASSERT_EQUAL_UINT32(NO_PIECE_TYPE, pos->position_state->previous_move.promotion_piece_type);
	TEST_ASSERT_EQUAL_UINT32(BLACK, pos->position_state->previous_move.color);
	TEST_ASSERT_EQUAL_UINT32(SQ_E8, pos->position_state->previous_move.source);
	TEST_ASSERT_EQUAL_UINT32(SQ_E7, pos->position_state->previous_move.destination);

	// Tests for promotion

}
