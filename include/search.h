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
 * \param depth search depth
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

#endif
