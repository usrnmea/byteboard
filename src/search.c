#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

ExtMove find_best(Position *position, __attribute__((unused)) uint32_t depth)
{
	assert(position != NULL);

	MoveList *move_list = generate_all_moves(position);

	ExtMove random_move = {
		.move = (Move) {
			.source = SQ_NONE,
			.destination = SQ_NONE,
			.color = COLOR_NB,
		},
		.eval = NO_EVAL,
	};

	if(ml_len(move_list))
		random_move = get_random_move(move_list);

	free(move_list);

	return random_move;
}

ExtMove get_random_move(MoveList *move_list)
{
	assert(move_list != NULL);
	assert(ml_len(move_list) != 0);

	srand(time(NULL));

	uint32_t i = rand() % ml_len(move_list);

	return move_list->move_list[i];
}
