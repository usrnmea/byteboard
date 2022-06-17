/**
 * \file
 */
#ifndef __MOVEGEN_H__
#define __MOVEGEN_H__

#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "masks.h"

/// Macro to add a move to move_list.
#define ml_add(move_list, move) (*(move_list)->last++ = (move))

/// Macro to get move_list len
#define ml_len(move_list) ((move_list)->last - (move_list)->move_list)

/// Macro to remove a last move in move_list.
#define ml_remove_last(move_list) (--(move_list)->last)

/// Macro to remove a last move in move_list and return this move.
#define ml_pop(move_list) (*--(move_list)->last)


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

/**
 * \brief Generates all possible moves at the given position.
 *
 * \param pos current position
 *
 * \return pointer to #MoveList
 *
 * \see https://www.chessprogramming.org/Moves
 */
MoveList *generate_all_moves(Position pos);

/**
 * \brief Adds to the move_list all moves from the Square source
 * for each destination from the destinations bitboard.
 *
 * \param move_list the list of all moves
 *
 * \param piece given piece
 *
 * \param source source square
 *
 * \param destinations bitboard with all destinations
 *
 * \see https://www.chessprogramming.org/Moves
 */
void add_common_moves(
	MoveList *move_list,
	const Piece piece,
	const Square source,
	U64 destinations
);

/**
 * \brief Adds to the move_list all castlings from the Square source
 * for each destination from the destinations bitboard.
 *
 * \param move_list the list of all moves
 *
 * \param source source square
 *
 * \param destinations bitboard with all destinations
 *
 * \see https://www.chessprogramming.org/Castling
 */
void add_castlings(
	MoveList *move_list,
	const Square source,
	U64 destinations
);

/**
 * \brief Adds to the move_list all promotions from the Square source
 * for each destination from the destinations bitboard.
 *
 * \param move_list the list of all moves
 *
 * \param color pawn color
 *
 * \param source source square
 *
 * \param destinations bitboard with all destinations
 *
 * \see https://www.chessprogramming.org/Promotions
 */
void add_promotions(
	MoveList *move_list,
	Color color,
	Square source,
	U64 destinations
);

/**
 * \brief Adds to the move_list all en passant moves from the source squares
 * for given destination square.
 *
 * \param move_list the list of all moves
 *
 * \param color pawn color
 *
 * \param sources bitboard with all source squares
 *
 * \param destination destination square
 *
 * \see https://www.chessprogramming.org/En_passant
 */
void add_en_passant(
	MoveList *move_list,
	Color color,
	U64 sources,
	Square destination
);

#endif
