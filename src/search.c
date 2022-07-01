#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

U64 nodes = 0;
U64 ply = 0;

/// Array for killer moves
ExtMove killer_moves[2][64];

/// Array with evaluation of history moves caused cutoff
Evaluation history_moves[12][64];

/// Lenght of the principle variation table
uint32_t pv_lenght[64];

/// Principle variation table
Move pv_table[64][64];

int cmp(const void *elem1, const void *elem2)
{
	ExtMove first = *((ExtMove*)elem1);
	ExtMove second = *((ExtMove*)elem2);

	if (first.eval > second.eval) return -1;
	if (first.eval < second.eval) return 1;

	return 0;
}

// Function for comparing between two Move structures
static inline int cmp_moves(Move move_1, Move move_2)
{
	return (
		move_1.color == move_2.color &&
		move_1.destination == move_2.destination &&
		move_1.moved_piece_type == move_2.moved_piece_type &&
		move_1.move_type == move_2.move_type &&
		move_1.promotion_piece_type == move_2.promotion_piece_type &&
		move_1.source == move_2.source
	);
}

ExtMove find_best(Position *position, uint32_t depth)
{
	assert(position != NULL);

	MoveList *move_list = generate_all_moves(position);

	sort_move_list(position, move_list);

	ExtMove best_move = {
		.eval = BLACK_WIN
	};

	nodes = 0;

	Evaluation score = negamax(
			position, move_list, depth,
			BLACK_WIN, WHITE_WIN
	);

	best_move.eval = score;
	best_move.move = pv_table[0][0];

	printf(
		"info score cp %d depth %d nodes %ld pv ",
		 best_move.eval, depth, nodes
	);

	for (uint32_t i = 0; i < pv_lenght[0]; i++) {
		char str[6];

		move_to_str(pv_table[0][i], str);
		printf("%s ", str);
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

void sort_move_list(Position *pos, MoveList *move_list)
{
	assert(pos != NULL);
	assert(move_list != NULL);

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		evaluate_move(pos, &move_list->move_list[i]);
	}

	qsort(
		move_list->move_list,
		ml_len(move_list),
		sizeof(*move_list->move_list),
		cmp
	);
}

void evaluate_move(Position *pos, ExtMove *move)
{
	assert(pos != NULL);

	if (
		piece_on(pos, move->move.destination) != NO_PIECE
		|| move->move.move_type == EN_PASSANT
	)
	{
		PieceType victim = PAWN;
		PieceType attacker = move->move.moved_piece_type;

		if (move->move.move_type != EN_PASSANT) {
			victim = type_of_piece(
				piece_on(pos, move->move.destination)
			);
		}

		move->eval = mvv_lva[attacker - 1][victim - 1] + 10000;
	}

	else {
		// Evaluate quite moves
		assert(piece_on(pos, move->move.destination) != W_KING);

		if (cmp_moves(killer_moves[0][ply].move, move->move))
			move->eval = 9000;

		else if (cmp_moves(killer_moves[1][ply].move, move->move))
			move->eval = 8000;

		else {
			PieceType pt = move->move.moved_piece_type - 1;
			Color color = move->move.color + 1;
			Square target = move->move.destination;

			move->eval = history_moves[pt * color][target];
		}
	}
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
	sort_move_list(pos, move_list);

	if(ml_len(move_list) == 0) {
		if(get_check_type(pos))
			return (color ? WHITE_WIN : BLACK_WIN) + ply;
		else
			return DRAW;
	}

	while(ml_len(move_list)) {
		Move current_move = ml_pop(move_list).move;

		ply++;

		if(
			piece_on(pos, current_move.destination) == NO_PIECE
			|| current_move.move_type != EN_PASSANT
		)
		{
			ply--;
			continue;
		}

		do_move(pos, current_move);

		Evaluation score = -quiescence(
			pos, -beta, -alpha
		);

		ply--;

		undo_move(pos);

		if(score >= beta) {
			free(move_list);
			return beta;
		}

		if(score > alpha)
			alpha = score;
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

	pv_lenght[ply] = ply;

	if (depth == 0) {
		max_score = quiescence(pos, alpha, beta);
		goto end;
	}

	nodes++;

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);

		ply++;

		MoveList *next_moves = generate_all_moves(pos);
		sort_move_list(pos, next_moves);

		Evaluation score = -negamax(
			pos, next_moves, depth - 1, -beta, -alpha
		);

		ply--;

		if (score > max_score)
			max_score = score;

		free(next_moves);

		undo_move(pos);

		if (max_score > alpha) {
			Move current_move = move_list->move_list[i].move;

			// History heuristic
			PieceType pt = current_move.moved_piece_type - 1;
			Color color = current_move.color + 1;
			Square target = current_move.destination;

			if (piece_on(pos, target) == NO_PIECE)
				history_moves[pt * color][target] += depth;

			alpha = max_score;

			// PV Table
			pv_table[ply][ply] = current_move;

			uint32_t next_ply = ply + 1;

			for (; next_ply < pv_lenght[ply + 1]; next_ply++) {
				Move deep_move = pv_table[ply + 1][next_ply];
				pv_table[ply][next_ply] = deep_move;
			}

			pv_lenght[ply] = pv_lenght[ply + 1];
		}

		if (alpha >= beta) {
			ExtMove ext_move = move_list->move_list[i];
			Square target = ext_move.move.destination;

			if (piece_on(pos, target) == NO_PIECE) {
				killer_moves[1][ply] = killer_moves[0][ply];
				killer_moves[0][ply] = ext_move;
			}

			break;
		}
	}

end:
	if(ml_len(move_list) == 0) {
		if(get_check_type(pos))
			max_score = BLACK_WIN + ply;
		else
			max_score = DRAW;
	}

	return max_score;
}
