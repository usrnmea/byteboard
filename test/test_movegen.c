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
