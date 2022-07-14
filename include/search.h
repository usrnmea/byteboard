/**
 * \file
 */
#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "hash.h"

/// Counter for the number of nodes
extern U64 nodes;

/// Ply counter
extern U64 ply;

/// PV flag that indicates do we follow PV line or not
extern uint8_t follow_PV;

/// PV flag that indicates do we evaluate PV moves or not
extern uint8_t eval_PV;

/// Max number of plies
#define MAX_PLY 64

/// MVV LVA table
static const Evaluation mvv_lva[6][6] = {
	{105, 355, 355, 525, 1005, 100005},
	{104, 354, 354, 524, 1004, 100004},
	{103, 353, 353, 523, 1003, 100003},
	{102, 352, 352, 522, 1002, 100002},
	{101, 351, 351, 521, 1001, 100001},
	{100, 350, 350, 520, 1000, 100000}
};

/**
 * \brief Returns the best move according to the chess engine
 *
 * \param position position
 *
 * \param depth the depth at which the engine will find the best move
 *
 * \return #ExtMove with the evaluation and the move
 */
ExtMove find_best(Position *position, uint32_t depth);

/**
 * \brief Sorts moves from the strongest to the weakest.
 *
 * \param pos current position
 *
 * \param move_list move list
 */
void sort_move_list(Position *pos, MoveList *move_list);

/**
 * \brief Evaluates the given move.
 *
 * \param pos current position
 *
 * \param move move to be evaluated
 */
void evaluate_move(Position *pos, ExtMove *move);

/**
 * \brief Evaluates pv moves in move list
 *
 * \param move_list move list
 */
void complete_pv_evaluation(MoveList *move_list);

/**
 * \brief Function which realises quiescence search algorithm.
 *
 * \param pos current position
 *
 * \param alpha alpha value
 *
 * \param beta beta value
 *
 * \return evaluation of the quiet moves
 *
 * \see https://www.chessprogramming.org/Quiescence_Search
 */
Evaluation quiescence(Position *pos, Evaluation alpha, Evaluation beta);

/**
 * \brief Negamax algorithm which returns the best evaluation of the given
 * position.
 *
 * \param pos current position
 *
 * \param depth search depth
 *
 * \param alpha #Evaluation alpha parameter for negamax
 *
 * \param beta #Evaluation beta parameter for negamax
 *
 * \return best evaluation of the given position
 */
Evaluation negamax(
	Position *pos, uint32_t depth,
	Evaluation alpha, Evaluation beta
);

#endif
