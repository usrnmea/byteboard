#include "unity.h"
#include "movegen.h"

void test_init_move_list(void)
{
	MoveList *move_list = init_move_list();

	TEST_ASSERT_NOT_NULL(move_list);

	TEST_ASSERT_EQUAL(move_list->move_list, move_list->last);
}
