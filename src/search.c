#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

uint32_t max_depth = 3;

ExtMove best_move = {
	.eval = NO_EVAL,
};

ExtMove find_best(Position *position, uint32_t depth)
{
	assert(position != NULL);

	MoveList *move_list = generate_all_moves(position);

	max_depth = depth;

	int32_t turn = !position->state->previous_move.color == 0 ? 1 : -1;

	Evaluation best_score = negamax(position, move_list, max_depth, turn);

	best_move.eval = best_score;

	free(move_list);
	return best_move;
}

ExtMove get_random_move(MoveList *move_list)
{
	assert(move_list != NULL);
	assert(ml_len(move_list) != 0);

	srand(time(NULL));

	uint32_t i = rand() % ml_len(move_list);

	return move_list->move_list[i];
}

Evaluation negamax(
	Position *pos, MoveList *move_list, uint32_t depth, uint32_t turn
)
{
	if (depth == 0) {
		return turn * evaluate_material(pos);
	}

	Evaluation max_score = BLACK_WIN;

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);

		MoveList *next_moves = generate_all_moves(pos);

		Evaluation score = -negamax(pos, next_moves, depth - 1, -turn);

		if (score > max_score) {
			max_score = score;

			if (depth == max_depth) {
				move_list->move_list[i].eval = score;
				best_move = move_list->move_list[i];
			}
		}

		free(next_moves);
		undo_move(pos);
	}

	return max_score;
}
