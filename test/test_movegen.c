#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "evaluate.h"
#include "movegen.h"
#include "position.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"

#include <stdlib.h>

// Initializing everything needed for tests
void test_init(void)
{
	init_rays();
}

void test_init_move_list(void)
{
	MoveList *move_list = init_move_list();

	TEST_ASSERT_NOT_NULL(move_list);

	TEST_ASSERT_EQUAL(move_list->move_list, move_list->last);

	free(move_list);
}

void test_ml_add(void)
{
	MoveList *move_list = init_move_list();

	ExtMove move = {
		.move = {
			.move_type = COMMON,

			.moved_piece_type = PAWN,
			.promotion_piece_type = NO_PIECE_TYPE,

			.color = WHITE,

			.source = SQ_E4,
			.destination = SQ_E5,
		},

		.eval = NO_EVAL,
	};

	ml_add(move_list, move);

	TEST_ASSERT_EQUAL(1, move_list->last - move_list->move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move, &move_list->last[-1], sizeof(ExtMove));

	free(move_list);
}

void test_ml_len(void)
{

	MoveList *move_list = init_move_list();

	TEST_ASSERT_EQUAL(0, ml_len(move_list));

	move_list->last += 10;

	TEST_ASSERT_EQUAL(10, ml_len(move_list));

	move_list->last -= 8;

	TEST_ASSERT_EQUAL(2, ml_len(move_list));

	free(move_list);
}

void test_ml_remove(void)
{
	MoveList *move_list = init_move_list();

	ExtMove move = {
		.move = {
			.move_type = COMMON,

			.moved_piece_type = PAWN,
			.promotion_piece_type = NO_PIECE_TYPE,

			.color = WHITE,

			.source = SQ_E4,
			.destination = SQ_E5,
		},

		.eval = NO_EVAL,
	};

	ml_add(move_list, move);
	ml_add(move_list, move);

	ml_remove_last(move_list);

	TEST_ASSERT_EQUAL(1, ml_len(move_list));
	TEST_ASSERT_EQUAL_MEMORY(&move, &move_list->last[-1], sizeof(ExtMove));

	ml_remove_last(move_list);

	TEST_ASSERT_EQUAL(0, ml_len(move_list));

	free(move_list);
}

void test_ml_pop(void)
{
	MoveList *move_list = init_move_list();

	ExtMove move_1 = {
		.move = {
			.move_type = COMMON,

			.moved_piece_type = PAWN,
			.promotion_piece_type = NO_PIECE_TYPE,

			.color = WHITE,

			.source = SQ_E4,
			.destination = SQ_E5,
		},

		.eval = NO_EVAL,
	};

	ExtMove move_2 = {
		.move = {
			.move_type = COMMON,

			.moved_piece_type = ROOK,
			.promotion_piece_type = NO_PIECE_TYPE,

			.color = BLACK,

			.source = SQ_E6,
			.destination = SQ_E5,
		},

		.eval = NO_EVAL,
	};

	ml_add(move_list, move_1);
	ml_add(move_list, move_2);

	ExtMove popped_move = ml_pop(move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move_2, &popped_move, sizeof(ExtMove));

	popped_move = ml_pop(move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &popped_move, sizeof(ExtMove));

	free(move_list);
}

void test_add_common_moves(void)
{
	MoveList *move_list = init_move_list();

	add_common_moves(move_list, W_PAWN, SQ_E2, 0x10100000ULL);
	add_common_moves(move_list, W_KNIGHT, SQ_G1, 0xA00000ULL);
	add_common_moves(move_list, W_KNIGHT, SQ_B1, 0x50000ULL);
	add_common_moves(move_list, W_PAWN, SQ_D2, 0x8080000ULL);

	Move move_1 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E3
	};

	Move move_2 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E4
	};

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_F3
	};

	Move move_4 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_H3
	};

	Move move_5 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_B1, .destination = SQ_A3
	};

	Move move_6 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_B1, .destination = SQ_C3
	};

	Move move_7 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_D2, .destination = SQ_D3
	};

	Move move_8 = {
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_D2, .destination = SQ_D4
	};

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_MEMORY(&move_5, &move_list->move_list[4], sizeof(move_5));
	TEST_ASSERT_EQUAL_MEMORY(&move_6, &move_list->move_list[5], sizeof(move_6));
	TEST_ASSERT_EQUAL_MEMORY(&move_7, &move_list->move_list[6], sizeof(move_7));
	TEST_ASSERT_EQUAL_MEMORY(&move_8, &move_list->move_list[7], sizeof(move_8));

	free(move_list);
}

void test_add_castlings(void)
{
	MoveList *move_list = init_move_list();

	add_castlings(move_list, SQ_E1, 0x44ULL);
	add_castlings(move_list, SQ_E8, 0x4400000000000000ULL);
	
	Move move_1 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_C1
	};

	Move move_2 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_G1
	};

	Move move_3 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_C8
	};

	Move move_4 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_G8
	};

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	
	free(move_list);
}

void test_add_promotions(void)
{
	MoveList *move_list = init_move_list();

	add_promotions(move_list, WHITE, SQ_B7, 0x200000000000000ULL);
	add_promotions(move_list, BLACK, SQ_H2, 0x80ULL);

	Move move_1 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_B7, .destination = SQ_B8
	};

	Move move_2 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = BISHOP, .color = WHITE,
		.source = SQ_B7, .destination = SQ_B8
	};

	Move move_3 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = ROOK, .color = WHITE,
		.source = SQ_B7, .destination = SQ_B8
	};

	Move move_4 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_B7, .destination = SQ_B8
	};

	Move move_5 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = BLACK,
		.source = SQ_H2, .destination = SQ_H1
	};

	Move move_6 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = BISHOP, .color = BLACK,
		.source = SQ_H2, .destination = SQ_H1
	};

	Move move_7 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = ROOK, .color = BLACK,
		.source = SQ_H2, .destination = SQ_H1
	};

	Move move_8 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = BLACK,
		.source = SQ_H2, .destination = SQ_H1
	};

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_MEMORY(&move_5, &move_list->move_list[4], sizeof(move_5));
	TEST_ASSERT_EQUAL_MEMORY(&move_6, &move_list->move_list[5], sizeof(move_6));
	TEST_ASSERT_EQUAL_MEMORY(&move_7, &move_list->move_list[6], sizeof(move_7));
	TEST_ASSERT_EQUAL_MEMORY(&move_8, &move_list->move_list[7], sizeof(move_8));

	free(move_list);
}

void test_add_en_passant(void)
{
	MoveList *move_list = init_move_list();

	add_en_passant(move_list, WHITE, 0x2000ULL, SQ_G3);
	add_en_passant(move_list, WHITE, 0x2000ULL, SQ_E3);

	add_en_passant(move_list, BLACK, 0x80000000000ULL, SQ_C5);
	add_en_passant(move_list, BLACK, 0x80000000000ULL, SQ_E5);

	Move move_1 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_F2, .destination = SQ_G3
	};

	Move move_2 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_F2, .destination = SQ_E3
	};

	Move move_3 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_D6, .destination = SQ_C5
	};

	Move move_4 = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_D6, .destination = SQ_E5
	};

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));

	free(move_list);
}

void test_king_safe_moves_mask(void)
{
	Position *pos_1 = init_position("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
	Position *pos_2 = init_position("q5k1/8/5b2/8/8/8/5P2/KN6 w - - 0 1");
	Position *pos_3 = init_position("rkn5/8/8/8/8/7n/q3RK2/8 w - - 0 1");
	Position *pos_4 = init_position("7r/8/8/k7/q6K/8/8/4B3 w - - 0 1");
	Position *pos_5 = init_position("rr6/7k/8/8/8/8/8/K7 w - - 0 1");

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, king_safe_moves_mask(pos_2, SQ_A1, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x28ULL, king_safe_moves_mask(pos_1, SQ_E1, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x2800000000000000ULL, king_safe_moves_mask(pos_1, SQ_E8, BLACK));
	TEST_ASSERT_EQUAL_UINT64(0xA0E0000000000000ULL, king_safe_moves_mask(pos_2, SQ_G8, BLACK));
	TEST_ASSERT_EQUAL_UINT64(0x704030ULL, king_safe_moves_mask(pos_3, SQ_F2, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x7000000000000ULL, king_safe_moves_mask(pos_3, SQ_B8, BLACK));
	TEST_ASSERT_EQUAL_UINT64(0x4000400000ULL, king_safe_moves_mask(pos_4, SQ_H4, WHITE));
	TEST_ASSERT_EQUAL_UINT64(0x30200000000ULL, king_safe_moves_mask(pos_4, SQ_A5, BLACK));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, king_safe_moves_mask(pos_5, SQ_A1, WHITE));

	free(pos_1->state);
	free(pos_2->state);
	free(pos_3->state);
	free(pos_4->state);
	free(pos_5->state);

	free(pos_1);
	free(pos_2);
	free(pos_3);
	free(pos_4);
	free(pos_5);
}

void test_possible_castlings(void)
{
	Position *pos_1 = init_position("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
	Position *pos_2 = init_position("4k2r/2q5/8/8/8/B7/PP1PPPPP/R3K2R w KQk - 0 1");
	Position *pos_3 = init_position("r3k2r/2q5/8/8/8/B6B/PP1PP2P/R3K1QR w KQkq - 0 1");
	Position *pos_4 = init_position("r3k1r1/8/8/8/8/8/8/R3K2R w Kq - 0 1");
	Position *pos_5 = init_position("1q3rk1/8/8/6p1/8/8/8/R3K3 w Q - 0 1");

	TEST_ASSERT_EQUAL_UINT32(NO_CASTLING, possible_castlings(pos_4, WHITE));
	TEST_ASSERT_EQUAL_UINT32(BLACK_OOO, possible_castlings(pos_4, BLACK));
	TEST_ASSERT_EQUAL_UINT32(ALL_WHITE, possible_castlings(pos_1, WHITE));
	TEST_ASSERT_EQUAL_UINT32(BLACK_OOO, possible_castlings(pos_1, BLACK));
	TEST_ASSERT_EQUAL_UINT32(WHITE_OO, possible_castlings(pos_2, WHITE));
	TEST_ASSERT_EQUAL_UINT32(NO_CASTLING, possible_castlings(pos_2, BLACK));
	TEST_ASSERT_EQUAL_UINT32(NO_CASTLING, possible_castlings(pos_3, WHITE));
	TEST_ASSERT_EQUAL_UINT32(NO_CASTLING, possible_castlings(pos_3, BLACK));
	TEST_ASSERT_EQUAL_UINT32(WHITE_OOO, possible_castlings(pos_5, WHITE));
	TEST_ASSERT_EQUAL_UINT32(NO_CASTLING, possible_castlings(pos_5, BLACK));

	free(pos_1->state);
	free(pos_2->state);
	free(pos_3->state);
	free(pos_4->state);
	free(pos_5->state);

	free(pos_1);
	free(pos_2);
	free(pos_3);
	free(pos_4);
	free(pos_5);
}

void test_generate_castlings(void)
{
	Position *pos_1 = init_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Position *pos_2 = init_position("r3k2r/p1ppbp1p/bp3qpB/8/4Q3/2N5/PPP2PPP/R3K2R w KQkq - 0 1");
	Position *pos_3 = init_position("1r1k3r/8/8/8/6n1/8/5PP1/R3K2Q w Q - 0 1");

	MoveList *move_list = init_move_list();

	Move move_1 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E1, .destination = SQ_A1
	};

	Move move_2 = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_E8, .destination = SQ_A8
	};

	pos_1->state->previous_move.color = BLACK;
	generate_castlings(pos_1, move_list);

	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);
	move_list = init_move_list();

	pos_1->state->previous_move.color = WHITE;
	generate_castlings(pos_1, move_list);

	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);
	move_list = init_move_list();

	pos_2->state->previous_move.color = BLACK;
	generate_castlings(pos_2, move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));

	free(move_list);
	move_list = init_move_list();
	
	pos_2->state->previous_move.color = WHITE;
	generate_castlings(pos_2, move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[0], sizeof(move_1));

	free(move_list);
	move_list = init_move_list();
	
	pos_3->state->previous_move.color = BLACK;
	generate_castlings(pos_3, move_list);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));

	free(move_list);
	move_list = init_move_list();
	
	pos_3->state->previous_move.color = WHITE;
	generate_castlings(pos_3, move_list);

	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);

	free(pos_1->state);
	free(pos_2->state);
	free(pos_3->state);

	free(pos_1);
	free(pos_2);
	free(pos_3);
}

void test_filter_legal_moves(void)
{
	Position *pos = init_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_E1, 0x3828ULL, UNIVERSE));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_D1, 0x00ULL, UNIVERSE));

	free(pos->state);
	free(pos);
	
	pos = init_position("1k6/8/8/7B/7b/8/2n5/R3K3 w - - 0 1");
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_A1, 0x10101010101010EULL, EMPTY));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_H5, 0x1020400040201008ULL, EMPTY));

	free(pos->state);
	free(pos);

	pos = init_position("1k6/8/8/8/7b/R7/8/R3K3 w - - 0 1");	
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_A1, 0x10EULL, 0x80402000ULL));
	TEST_ASSERT_EQUAL_UINT64(0x400000ULL, filter_legal_moves(pos, SQ_A3, 0x101010101FE0100ULL, 0x80402000ULL));

	free(pos->state);
	free(pos);

	pos = init_position("3k4/8/8/7b/3n4/5P2/3QK3/5B2 w - - 0 1");
	TEST_ASSERT_EQUAL_UINT64(0x8000000ULL, filter_legal_moves(pos, SQ_D2, 0x8080708ULL, 0x8000000ULL));
	TEST_ASSERT_EQUAL_UINT64(0x00ULL, filter_legal_moves(pos, SQ_F3, 0x20000000ULL, 0x8000000ULL));

	free(pos->state);
	free(pos);
}

void test_generate_knight_moves(void)
{
	Position *pos = init_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	MoveList *move_list = init_move_list();

	Move move_1 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_B1, .destination = SQ_A3
	};

	Move move_2 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_B1, .destination = SQ_C3
	};

	Move move_3 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_F3
	};

	Move move_4 = {
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_G1, .destination = SQ_H3
	};

	generate_knight_moves(move_list, pos, UNIVERSE);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_UINT32(4, ml_len(move_list));

	do_move(pos, move_1);

	move_1 = (Move){
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_B8, .destination = SQ_A6
	};

	move_2 = (Move){
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_B8, .destination = SQ_C6
	};

	move_3 = (Move){
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_G8, .destination = SQ_F6
	};

	move_4 = (Move){
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = BLACK,
		.source = SQ_G8, .destination = SQ_H6
	};

	free(move_list);
	move_list = init_move_list();

	generate_knight_moves(move_list, pos, UNIVERSE);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_UINT32(4, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position("8/7k/8/5b2/8/8/1N6/1KN4r w - - 0 1");
	move_list = init_move_list();

	move_1 = (Move){
		.move_type = COMMON, .moved_piece_type = KNIGHT,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_B2, .destination = SQ_D3
	};

	generate_knight_moves(move_list, pos, 0x2010080400);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_UINT32(1, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);
}

void test_generate_pawn_moves(void)
{
	// Tests for promotion
	Position *pos = init_position("4k3/6P1/5K2/8/8/8/8/8 w - - 0 1");
	MoveList *move_list = init_move_list();

	Move move_1 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_2 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = BISHOP, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_3 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = ROOK, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_4 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	generate_pawn_moves(move_list, pos, UNIVERSE);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_UINT32(4, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position("2k2r2/6P1/4K3/8/8/8/8/8 w - - 0 1");
	move_list = init_move_list();

	move_1 = (Move){
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	move_2 = (Move){
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = BISHOP, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	move_3 = (Move){
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = ROOK, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	move_4 = (Move){
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_G7, .destination = SQ_F8
	};

	Move move_5 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = KNIGHT, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_6 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = BISHOP, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_7 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = ROOK, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	Move move_8 = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.promotion_piece_type = QUEEN, .color = WHITE,
		.source = SQ_G7, .destination = SQ_G8
	};

	generate_pawn_moves(move_list, pos, UNIVERSE);

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_MEMORY(&move_2, &move_list->move_list[1], sizeof(move_2));
	TEST_ASSERT_EQUAL_MEMORY(&move_3, &move_list->move_list[2], sizeof(move_3));
	TEST_ASSERT_EQUAL_MEMORY(&move_4, &move_list->move_list[3], sizeof(move_4));
	TEST_ASSERT_EQUAL_MEMORY(&move_5, &move_list->move_list[4], sizeof(move_5));
	TEST_ASSERT_EQUAL_MEMORY(&move_6, &move_list->move_list[5], sizeof(move_6));
	TEST_ASSERT_EQUAL_MEMORY(&move_7, &move_list->move_list[6], sizeof(move_7));
	TEST_ASSERT_EQUAL_MEMORY(&move_8, &move_list->move_list[7], sizeof(move_8));
	TEST_ASSERT_EQUAL_UINT32(8, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	// Tests for common moves
	pos = init_position("2k1r3/1p6/8/8/8/6P1/4KP2/2B5 w - - 0 1");
	move_list = init_move_list();

	generate_pawn_moves(move_list, pos, 0x1010101010100000ULL);

	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position("2kr4/8/7q/8/p7/3P4/3KP3/8 w - - 0 1");
	move_list = init_move_list();

	generate_pawn_moves(move_list, pos, 0x804020100000ULL);

	move_1 = (Move){
		.move_type = COMMON, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE, .color = WHITE,
		.source = SQ_E2, .destination = SQ_E3
	};

	TEST_ASSERT_EQUAL_MEMORY(&move_1, &move_list->move_list[0], sizeof(move_1));
	TEST_ASSERT_EQUAL_UINT32(1, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position("2kr4/8/7q/8/p7/3P4/3KP2r/8 w - - 0 1");
	move_list = init_move_list();

	generate_pawn_moves(move_list, pos, 0x804020100000ULL);

	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position(
		"rnbqkbnr/pppp1ppp/8/3PpP2/8/8/PPP1P1PP/RNBQKBNR w KQkq e6 0 1"
	);

	move_list = init_move_list();

	generate_pawn_moves(move_list, pos, UNIVERSE);

	TEST_ASSERT_EQUAL_UINT32(16, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position(
		"k7/2K2rb1/8/4pP2/8/8/8/8 w - e6 0 1"
	);

	move_list = init_move_list();

	PositionState state = *pos->state;

	generate_pawn_moves(move_list, pos, 0x38000000000000ULL);

	TEST_ASSERT_EQUAL_MEMORY(&state, pos->state, sizeof(PositionState));
	TEST_ASSERT_EQUAL_UINT32(0, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);
}

void test_generate_pawn_promotions(void)
{
	Position *pos = init_position(
		"8/1K2P2r/8/8/8/8/8/4k3 w - - 0 1"
	);

	MoveList *move_list = init_move_list();

	generate_pawn_promotions(move_list, pos, UNIVERSE, pieces(pos, W_PAWN));

	TEST_ASSERT_EQUAL(0, ml_len(move_list));

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position(
		"8/1K2P3/8/8/8/8/8/k7 w - - 0 1"
	);

	move_list = init_move_list();

	generate_pawn_promotions(move_list, pos, UNIVERSE, pieces(pos, W_PAWN));

	TEST_ASSERT_EQUAL(4, ml_len(move_list));

	while(ml_len(move_list)) {
		Move move = ml_pop(move_list).move;

		TEST_ASSERT_EQUAL(SQ_E8, move.destination);
		TEST_ASSERT_EQUAL(SQ_E7, move.source);

		TEST_ASSERT_EQUAL(PROMOTION, move.move_type);

		TEST_ASSERT_EQUAL(WHITE, move.color);

		TEST_ASSERT_NOT_EQUAL(
			NO_PIECE_TYPE,
			move.promotion_piece_type
		);

		TEST_ASSERT_EQUAL(PAWN, move.moved_piece_type);
	}

	free(move_list);
	free(pos->state);
	free(pos);

	pos = init_position(
		"8/8/6K1/8/8/8/3p4/k3N2R b - - 0 1"
	);

	move_list = init_move_list();

	generate_pawn_promotions(move_list, pos, UNIVERSE, pieces(pos, B_PAWN));

	TEST_ASSERT_EQUAL(8, ml_len(move_list));

	while(ml_len(move_list)) {
		Move move = ml_pop(move_list).move;

		TEST_ASSERT_TRUE(
			square_to_bitboard(move.destination)
			& (
				square_to_bitboard(SQ_D1)
				| square_to_bitboard(SQ_E1)
			)
		);
		TEST_ASSERT_EQUAL(SQ_D2, move.source);

		TEST_ASSERT_EQUAL(PROMOTION, move.move_type);

		TEST_ASSERT_EQUAL(BLACK, move.color);

		TEST_ASSERT_NOT_EQUAL(
			NO_PIECE_TYPE,
			move.promotion_piece_type
		);

		TEST_ASSERT_EQUAL(PAWN, move.moved_piece_type);
	}

	free(move_list);
	free(pos->state);
	free(pos);
}
