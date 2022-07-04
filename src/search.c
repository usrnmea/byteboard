#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"
#include "uci.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

U64 nodes = 0;
U64 ply = 0;

uint8_t follow_PV = 0;
uint8_t eval_PV = 0;

/// Array for killer moves
ExtMove killer_moves[2][MAX_PLY];

/// Array with evaluation of history moves caused cutoff
Evaluation history_moves[12][64];

/// Lenght of the principle variation table
uint32_t pv_lenght[MAX_PLY];

/// Principle variation table
Move pv_table[MAX_PLY][MAX_PLY];

/// Full depth searching constant for LMR
const uint32_t FULL_DEPTH_MOVES = 4;

/// Reduction limit in LMR
const uint32_t REDUCTION_LIMIT = 3;

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

	ExtMove best_move = {
		.eval = BLACK_WIN
	};

	time_info.stopped = 0;

	nodes = 0;
	ply = 0;

	memset(killer_moves, 0, sizeof(killer_moves));
	memset(history_moves, 0, sizeof(history_moves));

	memset(pv_table, 0, sizeof(pv_table));
	memset(pv_lenght, 0, sizeof(pv_lenght));

	for (uint32_t curr_depth = 1; curr_depth <= depth; curr_depth++) {
		if (time_info.stopped == 1)
			break;

		follow_PV = 1;

		Evaluation score = negamax(
			position, curr_depth,
			BLACK_WIN, WHITE_WIN
		);

		best_move.eval = score;
		best_move.move = pv_table[0][0];

		printf(
			"info score cp %d depth %d nodes %ld pv ",
			best_move.eval, curr_depth, nodes
		);

		for (uint32_t i = 0; i < pv_lenght[0]; i++) {
			char str[6];

			move_to_str(pv_table[0][i], str);
			printf("%s ", str);
		}

		printf("\n");
	}

	return best_move;
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

	if (eval_PV) {
		if (cmp_moves(pv_table[0][ply], move->move)) {
			eval_PV = 0;

			move->eval = 20000;
		}
	}

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

void complete_pv_evaluation(MoveList *move_list)
{
	follow_PV = 0;

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		if (cmp_moves(pv_table[0][ply], move_list->move_list[i].move)) {
			eval_PV = 1;

			follow_PV = 1;
		}
	}
}

Evaluation quiescence(Position *pos, Evaluation alpha, Evaluation beta)
{
	assert(pos != NULL);

	if ((nodes & 2047) == 0)
		communicate();

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
		free(move_list);

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

		if (time_info.stopped == 1)
			return NO_EVAL;

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
	Position *pos, uint32_t depth,
	Evaluation alpha, Evaluation beta
)
{
	assert(pos != NULL);

	if ((nodes & 2047) == 0)
		communicate();

	uint32_t moves_searched = 0;

	Evaluation max_score = BLACK_WIN;

	pv_lenght[ply] = ply;

	if (depth == 0) {
		return quiescence(pos, alpha, beta);
	}

	if (ply > MAX_PLY - 1)
		return evaluate_position(pos);

	nodes++;

	if (get_check_type(pos) != NO_CHECK)
		depth++;

	if (depth >= 3 && get_check_type(pos) == NO_CHECK && ply) {
		do_null_move(pos);

		Evaluation score = -negamax(
			pos, depth - 1 - 2, -beta, -beta + 1
		);

		undo_null_move(pos);

		if (time_info.stopped == 1)
			return NO_EVAL;

		if (score >= beta)
			return beta;
	}

	MoveList *move_list = generate_all_moves(pos);

	if (follow_PV)
		complete_pv_evaluation(move_list);

	sort_move_list(pos, move_list);

	for (uint32_t i = 0; i < ml_len(move_list); i++) {
		Move current_move = move_list->move_list[i].move;

		do_move(pos, current_move);

		ply++;

		Evaluation score = NO_EVAL;

		if (moves_searched == 0)
			score = -negamax(pos, depth - 1, -beta, -alpha);

		else {
			Piece capture = piece_on(pos, current_move.destination);
			PieceType pt = current_move.promotion_piece_type;

			if (
				moves_searched >= FULL_DEPTH_MOVES &&
				depth >= REDUCTION_LIMIT &&
				get_check_type(pos) == NO_CHECK &&
				capture == NO_PIECE &&
				pt == NO_PIECE_TYPE
			)
				score = -negamax(pos, depth - 1, -beta, -alpha);
			else
				score = alpha + 1;

			if (score > alpha) {
				score = -negamax(
					pos, depth - 1, -alpha - 1, -alpha
				);
				if (score > alpha && score < beta)
					score = -negamax(
						pos, depth - 1,
						-beta, -alpha
					);
			}
		}

		ply--;

		if (score > max_score)
			max_score = score;

		undo_move(pos);

		if (time_info.stopped == 1)
			return NO_EVAL;

		moves_searched++;

		if (max_score > alpha) {
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

	if(ml_len(move_list) == 0) {
		if(get_check_type(pos))
			max_score = BLACK_WIN + ply;
		else
			max_score = DRAW;
	}

	free(move_list);

	return max_score;
}
