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
