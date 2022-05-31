#include "unity.h"
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "evaluate.h"
#include "movegen.h"

#include <stdlib.h>

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
