/**
 * \file
 */
#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"


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
 * \brief Selects one random move from the #MoveList
 *
 * \param move_list move list(shouldn't be empty)
 *
 * \return random move
 */
ExtMove get_random_move(MoveList *move_list);

/**
 * \brief Negamax algorithm which returns the best evaluation of the given
 * position.
 *
 * \param pos current position
 *
 * \param move_list legal moves
 *
 * \param depth
 *
 * \param alpha #Evaluation alpha parameter for negamax
 *
 * \param beta #Evaluation beta parameter for negamax
 *
 * \return best evaluation of the given position
 */
Evaluation negamax(
	Position *pos, MoveList *move_list, uint32_t depth,
	Evaluation alpha, Evaluation beta
);

#endif
