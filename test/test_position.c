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
		0x9002000100800440ULL, pos->state->occupied
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
	free(pos->state);
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
		0xFFFF00000000FFFFULL, pos->state->occupied
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	free(pos->state);
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

	TEST_ASSERT_EQUAL_UINT64(0xFFFF00001000EFFF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xFFFF000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);


	Move move_2 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E7, .destination = SQ_E5
	};
	do_move(pos, move_2);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101000EFFF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_F3
	};
	do_move(pos, move_3);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101020EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(1, pos->state->move_50_rule);

	Move move_4 = {
		.move_type = COMMON, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_E7
	};
	do_move(pos, move_4);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101020EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF001000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(2, pos->state->move_50_rule);

	Move move_5 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_F3, .destination = SQ_E5
	};
	do_move(pos, move_5);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101000EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x101000EFBF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(B_PAWN, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);

	// Tests for previous_state
	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101020EFBF, pos->state->previous_state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->state->previous_state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF001000000000, pos->state->previous_state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->previous_state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->previous_state->castling);
	TEST_ASSERT_EQUAL_UINT32(2, pos->state->previous_state->move_50_rule);

	// Tests for previous_move
	TEST_ASSERT_EQUAL_UINT32(COMMON, pos->state->previous_move.move_type);
	TEST_ASSERT_EQUAL_UINT32(KNIGHT, pos->state->previous_move.moved_piece_type);
	TEST_ASSERT_EQUAL_UINT32(NO_PIECE_TYPE, pos->state->previous_move.promotion_piece_type);
	TEST_ASSERT_EQUAL_UINT32(WHITE, pos->state->previous_move.color);
	TEST_ASSERT_EQUAL_UINT32(SQ_F3, pos->state->previous_move.source);
	TEST_ASSERT_EQUAL_UINT32(SQ_E5, pos->state->previous_move.destination);

	free(pos->state);
	free(pos);

	// Tests for promotion
	pos = init_position("8/7P/k7/8/8/8/K7/8 w - - 0 1");

	Move move_promotion = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_H7, .destination = SQ_H8
	};
	do_move(pos, move_promotion);

	TEST_ASSERT_EQUAL_UINT64(0x8000010000000100, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x10000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x8000000000000100, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);

	TEST_ASSERT_EQUAL_UINT64(0x100ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x10000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x8000000000000000ULL, pos->board.WhiteQueens);

	free(pos->state);
	free(pos);

	// Tests for castling
	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_white_00 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_G1
	};
	do_move(pos, move_castling_white_00);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F761, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xF761, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x21ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_BLACK, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_white_000 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_C1
	};
	do_move(pos, move_castling_white_000);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F78c, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xF78CULL, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x4ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x88ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_BLACK, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_black_00 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_G8
	};
	do_move(pos, move_castling_black_00);

	TEST_ASSERT_EQUAL_UINT64(0x61F700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x61F7000000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x4000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x2100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_black_000 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_C8
	};
	do_move(pos, move_castling_black_000);

	TEST_ASSERT_EQUAL_UINT64(0x8CF700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x8CF7000000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x400000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8800000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);

	free(pos->state);
	free(pos);

	// Tests for en passant
	pos = init_position("1k5r/8/8/8/5pP1/2P5/3P4/1K6 w - - 0 1");

	Move move_en_passant = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_F4, .destination = SQ_G3
	};
	do_move(pos, move_en_passant);

	TEST_ASSERT_EQUAL_UINT64(0x8200000000440802, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x40802, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x8200000000400000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);

	TEST_ASSERT_EQUAL_UINT64(0x2ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x200000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x40800ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x400000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x8000000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL(W_PAWN, pos->state->captured_piece);

	free(pos->state);
	free(pos);
}

void test_do_castling(void)
{
	Position *pos = init_position(
		"r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1"
	);

	do_castling(pos, WHITE_OO);
	do_castling(pos, BLACK_OOO);

	TEST_ASSERT_EQUAL_UINT64(0x40ULL, pieces(pos, W_KING));
	TEST_ASSERT_EQUAL_UINT64(0x21ULL, pieces(pos, W_ROOK));
	TEST_ASSERT_EQUAL_UINT64(0x400000000000000ULL, pieces(pos, B_KING));
	TEST_ASSERT_EQUAL_UINT64(0x8800000000000000ULL, pieces(pos, B_ROOK));
}

void test_undo_move(void)
{
	// Tests for common
	Position *pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	Move move = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};
	do_move(pos, move);

	Move move_2 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E7, .destination = SQ_E5
	};
	do_move(pos, move_2);

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_F3
	};
	do_move(pos, move_3);

	Move move_4 = {
		.move_type = COMMON, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_E7
	};
	do_move(pos, move_4);

	Move move_5 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_F3, .destination = SQ_E5
	};
	do_move(pos, move_5);

	Move move_6 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_D7, .destination = SQ_D6
	};
	do_move(pos, move_6);

	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101000EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x101000EFBF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(B_PAWN, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);

	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0xEFFF00101020EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xEFFF001000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(2, pos->state->move_50_rule);

	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101020EFBF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1020EFBF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(1, pos->state->move_50_rule);

	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0xFFEF00101000EFFF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xFFEF001000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);

	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0xFFFF00001000EFFF, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xFFFF000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x1000EFFF, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT32(NO_PIECE, pos->state->captured_piece);
	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);
	TEST_ASSERT_EQUAL_UINT32(0, pos->state->move_50_rule);

	free(pos->state);
	free(pos);

	// Tests for castling
	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_white_00 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_G1
	};
	do_move(pos, move_castling_white_00);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R w KQkq - 0 1");

	Move move_castling_white_000 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_C1
	};
	do_move(pos, move_castling_white_000);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R b KQkq - 0 1");

	Move move_castling_black_00 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_G8
	};
	do_move(pos, move_castling_black_00);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);

	free(pos->state);
	free(pos);

	pos = init_position("r3k2r/ppp1pppp/8/8/8/8/PPP1PPPP/R3K2R b KQkq - 0 1");

	Move move_castling_black_000 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_C8
	};
	do_move(pos, move_castling_black_000);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x91F700000000F791, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x91F7000000000000, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0xF791, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x10ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x1000000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0xF700ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0xF7000000000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x81ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x8100000000000000ULL, pos->board.BlackRooks);

	TEST_ASSERT_EQUAL_UINT32(ALL_CASTLING, pos->state->castling);

	free(pos->state);
	free(pos);

	// Tests for promotion
	pos = init_position("8/7P/k7/8/8/8/K7/8 w - - 0 1");

	Move move_promotion = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_H7, .destination = SQ_H8
	};
	do_move(pos, move_promotion);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x80010000000100, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x80000000000100, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x10000000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackRooks);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);

	TEST_ASSERT_EQUAL_UINT64(0x100ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x10000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x80000000000000ULL, pos->board.WhitePawns);

	free(pos->state);
	free(pos);

	// Tests for en passant
	pos = init_position("1k5r/8/8/8/5pP1/2P5/3P4/1K6 w - - 0 1");

	Move move_en_passant = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_F4, .destination = SQ_G3
	};
	do_move(pos, move_en_passant);
	undo_move(pos);

	TEST_ASSERT_EQUAL_UINT64(0x8200000060040802, pos->state->occupied);
	TEST_ASSERT_EQUAL_UINT64(0x40040802, pos->state->allies);
	TEST_ASSERT_EQUAL_UINT64(0x8200000020000000, pos->state->enemies);

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackKnights);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.BlackBishops);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteQueens);
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, pos->board.WhiteRooks);

	TEST_ASSERT_EQUAL_UINT64(0x2ULL, pos->board.WhiteKing);
	TEST_ASSERT_EQUAL_UINT64(0x200000000000000ULL, pos->board.BlackKing);
	TEST_ASSERT_EQUAL_UINT64(0x40040800ULL, pos->board.WhitePawns);
	TEST_ASSERT_EQUAL_UINT64(0x20000000ULL, pos->board.BlackPawns);
	TEST_ASSERT_EQUAL_UINT64(0x8000000000000000ULL, pos->board.BlackRooks);

	free(pos->state);
	free(pos);
}

void test_get_check_type(void)
{
	Position *pos = init_position("8/8/3n1N2/8/4K3/8/pp6/k6q w - - 0 1");

	TEST_ASSERT_EQUAL(DOUBLE_CHECK, get_check_type(pos));

	free(pos->state);
	free(pos);

	pos = init_position("8/1b6/8/8/4K3/8/pp6/k7 w - - 0 1");

	TEST_ASSERT_EQUAL(SINGLE_CHECK, get_check_type(pos));

	free(pos->state);
	free(pos);

	pos = init_position("8/8/8/8/2R1K3/8/pp6/k7 w - - 0 1");

	TEST_ASSERT_EQUAL(NO_CHECK, get_check_type(pos));

	free(pos->state);
	free(pos);
}

void test_get_pinned(void)
{
	Position *pos_1 = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);
	Position *pos_2 = init_position(
		"8/4n1n1/Q3rk2/q7/8/8/P2PP2P/R2NK2R w Q - 0 1"
	);
	Position *pos_3 = init_position(
		"3Q4/4n1n1/4rk2/8/7B/8/4P2P/1q1NK2R w - - 0 1"
	);

	pos_1->state->previous_move.color = BLACK;
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, get_pinned(pos_1));

	pos_1->state->previous_move.color = WHITE;
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, get_pinned(pos_1));

	pos_2->state->previous_move.color = BLACK;
	TEST_ASSERT_EQUAL_UINT64(0x1800ULL, get_pinned(pos_2));

	pos_2->state->previous_move.color = WHITE;
	TEST_ASSERT_EQUAL_UINT64(0x100000000000ULL, get_pinned(pos_2));

	pos_3->state->previous_move.color = BLACK;
	TEST_ASSERT_EQUAL_UINT64(0x1008ULL, get_pinned(pos_3));

	pos_3->state->previous_move.color = WHITE;
	TEST_ASSERT_EQUAL_UINT64(0x10000000000000ULL, get_pinned(pos_3));

	free(pos_1->state);
	free(pos_2->state);
	free(pos_3->state);

	free(pos_1);
	free(pos_2);
	free(pos_3);
}
