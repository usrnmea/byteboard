#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

ExtMove find_best(Position *position)
{
	assert(position != NULL);

	return (ExtMove) {
		.move = (Move) {
			.move_type = COMMON,
			.moved_piece_type = NO_PIECE_TYPE,
			.promotion_piece_type = NO_PIECE_TYPE,
			.color = COLOR_NB,
			.destination = SQ_NONE,
			.source = SQ_NONE,
		},
		.eval = NO_EVAL,
	};
}

ExtMove get_random_move(MoveList *move_list)
{
	assert(move_list != NULL);
	assert(ml_len(move_list) != 0);

	srand(time(NULL));

	uint32_t i = rand() % ml_len(move_list);

	return move_list->move_list[i];
}
