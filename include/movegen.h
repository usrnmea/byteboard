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
 * \brief Returns a special structure with an array of all
 * legal moves available at the given position.
 *
 * \param pos current position
 *
 * \return pointer to the #MoveList with all legal moves or NULL in case
 * generation failed.
 *
 * \see https://www.chessprogramming.org/Move_Generation
 */
MoveList *generate_all_moves(Position *pos);

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

/**
 * \brief Generates all legal castling.
 * Should be called only whether king is not checked.
 *
 * \param pos current position
 *
 * \param move_list the list with all moves
 *
 * \see https://www.chessprogramming.org/Castling
 */
void generate_castlings(Position *pos, MoveList *move_list);

/**
 * \brief A function that returns a mask with flags indicating which
 * castings are available for a given position. WARNING! This function should
 * only be used when there is no check
 *
 * \param color
 *
 * \param castling castling mask
 *
 * \param occupied occupied bitboard
 *
 * \see https://www.chessprogramming.org/Castling
 */
Castling possible_castlings(
	Position *pos,
	Color color
);

/**
 * \brief A function that returns a mask with flags indicating which
 * castings are available for a given position. WARNING! This function should
 * only be used when there is no check
 *
 * \param pos current position
 *
 * \param target target square
 *
 * \param color
 */
U64 king_safe_moves_mask(
	Position *pos,
	Square target,
	Color color
);

/**
 * \brief Takes bitboard with destinations and filter moves by check ray and
 * pin ray for the given square.
 *
 * \param pos current position
 *
 * \param source source square
 *
 * \param destinations bitboard with all destinations
 *
 * \param check_ray check ray
 *
 * \return bitboard with legal moves
 */
U64 filter_legal_moves(
	Position *pos,
	Square source,
	U64 destinations,
	U64 check_ray
);

/**
 * \brief Generates all possible moves for the knight and adds them into the
 * move list.
 *
 * \param move_list the list with all moves
 *
 * \param pos current position
 *
 * \param check_ray check ray
 */
void generate_knight_moves(
	MoveList *move_list, Position *pos, U64 check_ray
);

/**
 * \brief Generates all possible moves for the pawns and adds them into the
 * move list.
 *
 * \param move_list the list with all moves
 *
 * \param pos current position
 *
 * \param check_ray check ray
 */
void generate_pawn_moves(
	MoveList *move_list, Position *pos, U64 check_ray
);

/**
 * \brief Adds all possible moves of the #PROMOTION type to the #MoveList
 *
 * \param move_list the list with all moves
 *
 * \param position current position
 *
 * \param check_ray check ray
 *
 * \param pawns_on_last_rank pawns on the #RANK_2 (for #BLACK) or #RANK_7 (for
 * #WHITE)
 */
void generate_pawn_promotions(
	MoveList *move_list,
	Position *position,
	U64 check_ray,
	U64 pawns_on_last_rank
);

/**
 * \brief Adds all possible moves of the #COMMON type to the #MoveList for
 * sliding pieces.
 *
 * \param move_list the list with all moves
 *
 * \param pos current position
 *
 * \param pt piece type
 *
 * \param check_ray check ray
 *
 * \param get_attacks pointer to the mask function
 *
 * \see https://www.chessprogramming.org/Sliding_Pieces
 */
void generate_sliding_pieces(
	MoveList *move_list,
	Position *pos,
	PieceType pt,
	U64 check_ray,
	U64 (*get_attacks)(Square target, U64 occupied)
);

#endif
