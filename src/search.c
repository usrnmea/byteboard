#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

U64 nodes = 0;

ExtMove find_best(Position *position, uint32_t depth)
{
	assert(position != NULL);

	MoveList *move_list = generate_all_moves(position);

	ExtMove best_move = {
		.eval = BLACK_WIN
	};

	nodes = 0;

	while(ml_len(move_list)) {
		ExtMove move = ml_pop(move_list);

		do_move(position, move.move);

		MoveList *new_move_list = generate_all_moves(position);

		move.eval = -negamax(
			position, new_move_list, depth - 1,
			BLACK_WIN, WHITE_WIN
		);

		free(new_move_list);

		undo_move(position);

		if(move.eval >= best_move.eval)
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

Evaluation quiescence(Position *pos, Evaluation alpha, Evaluation beta)
{
	assert(pos != NULL);

	nodes++;

	Color color = !pos->state->previous_move.color;
	Evaluation stand_pat = evaluate_position(pos) * (color ? -1 : 1);

	if (stand_pat >= beta)
		return beta;
	if (alpha < stand_pat)
		alpha = stand_pat;

	MoveList *move_list = generate_all_moves(pos);

	if (ml_len(move_list) > 0) {
		for (uint32_t i = 0; i < ml_len(move_list); i++) {
			Move current_move = move_list->move_list[i].move;

			// Capture
			if (piece_on(pos, current_move.destination) != NO_PIECE)
			{
				do_move(pos, current_move);

				Evaluation score = -quiescence(
					pos, -beta, -alpha
				);

				undo_move(pos);

				if (score >= beta) {
					free(move_list);
					return beta;
				}
				if (score > alpha)
					alpha = score;
			}
		}
	}

	free(move_list);

	return alpha;
}

Evaluation negamax(
	Position *pos, MoveList *move_list, uint32_t depth,
	Evaluation alpha, Evaluation beta
)
{
	Evaluation max_score = BLACK_WIN;


	if (depth == 0) {
		max_score = quiescence(pos, alpha, beta);

		goto end;
	}

	nodes++;

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);

		MoveList *next_moves = generate_all_moves(pos);

		Evaluation score = -negamax(
			pos, next_moves, depth - 1, -beta, -alpha
		);

		if (score > max_score)
			max_score = score;

		free(next_moves);

		undo_move(pos);

		if (max_score > alpha)
			alpha = max_score;

		if (alpha >= beta)
			break;
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
