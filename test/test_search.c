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

// Initializing everything needed for tests
void test_init(void)
{
	init_rays();
}

void test_find_best(void)
{
	Position *pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	ExtMove best = find_best(pos, 1);

	TEST_ASSERT_TRUE(best.move.moved_piece_type != NO_PIECE_TYPE);
	TEST_ASSERT_TRUE(best.move.color < COLOR_NB);
	TEST_ASSERT_TRUE(best.move.destination < SQ_NB);
	TEST_ASSERT_TRUE(best.move.source < SQ_NB);

	free(pos->state);
	free(pos);
}
