#ifndef __MOVEGEN_H__
#define __MOVEGEN_H__

#include "bitboard.h"
#include "position.h"
#include "evaluate.h"

/// Macro to add a move to move_list.
#define add_move(move_list, move) *move_list->last = move;

/// Macro to remove a last move in move_list.
#define remove_move(move_list) --move_list->last;

/// Macro to remove a last move in move_list and return this move.
#define pop_move(move_list) *--move_list->last;


/// Approximate number of available half moves for one position.
#define MOVE_MAX 256

/// The list of all available moves in the position.
typedef struct MoveList {
    ExtMove move_list[MOVE_MAX];	///< Array with all available moves.
    ExtMove *last;			///< Pointer to the last element in array.
} MoveList;

/**
 * \brief Initialize #MoveList.
 *
 * \return pointer to #MoveList
 */
MoveList *init_move_list(void);

#endif
