#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

ExtMove find_best(Position *position, uint32_t depth)
{
	assert(position != NULL);

	MoveList *move_list = generate_all_moves(position);

	ExtMove best_move = {
		.eval = BLACK_WIN
	};

	while(ml_len(move_list)) {
		ExtMove move = ml_pop(move_list);

		do_move(position, move.move);

		MoveList *new_move_list = generate_all_moves(position);

		move.eval = negamax(position, new_move_list, depth - 1);

		free(new_move_list);

		undo_move(position);

		if(move.eval >= BLACK_WIN)
			best_move = move;
	}

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
	Position *pos, MoveList *move_list, uint32_t depth
)
{
	Evaluation max_score = BLACK_WIN;


	if (depth == 0) {
		Color color = !pos->state->previous_move.color;

		max_score = evaluate_material(pos) * (color ? -1 : 1);

		goto end;
	}

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);

		MoveList *next_moves = generate_all_moves(pos);

		Evaluation score = -negamax(pos, next_moves, depth - 1);

		if (score > max_score)
			max_score = score;

		free(next_moves);

		undo_move(pos);
	}

end:
	if(ml_len(move_list) == 0) {
		if(get_check_type(pos))
			max_score = BLACK_WIN;
		else
			max_score = DRAW;
	}

	return max_score;
}
