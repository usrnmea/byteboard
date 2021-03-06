/**
 * \file
 */
#ifndef __POSITION_H__
#define __POSITION_H__

#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"

#include <stdbool.h>

/// Indicates the type of move
typedef enum MoveType {
	COMMON = 0,	///< Common movement of a piece
	EN_PASSANT = 1,	///< \see https://www.chessprogramming.org/En_passant
	PROMOTION = 2,	///< Promotion of a pawn into a piece
	CASTLING = 3	///< \see https://www.chessprogramming.org/Castling
} MoveType;

/// Represents one move. To be more precise, a half-move. A move in chess is
/// two half-moves by both sides. In code, we call a half move a "move" and a
/// full move a "full move".
typedef struct Move {
	MoveType move_type;		///< #MoveType
	PieceType moved_piece_type;	///< Moved piece type
	PieceType promotion_piece_type;	/*!< The type the pawn was promoted to
					(If a PROMOTION movetype was made,
					otherwise the value should be
					#NO_PIECE_TYPE) */
	Color color;			///< turn color
	Square source;			///< Source square
	Square destination;		///< Destination square
} Move;

/// Bit flags that can be used to indicate the castling availability for each
/// position
typedef enum Castling {
	NO_CASTLING,
	WHITE_OO,
	WHITE_OOO = WHITE_OO << 1,
	BLACK_OO = WHITE_OO << 2,
	BLACK_OOO = WHITE_OO << 3,
	KING_SIDE = WHITE_OO | BLACK_OO,
	QUEEN_SIDE = WHITE_OOO | BLACK_OOO,
	ALL_WHITE = WHITE_OO | WHITE_OOO,
	ALL_BLACK = BLACK_OO | BLACK_OOO,
	ALL_CASTLING = ALL_WHITE | ALL_BLACK,
} Castling;

/// When working with castling, it becomes necessary to determine Castling by
/// the cells to which the king or rook has moved/stood. For more convenient
/// work, this array was created. For king squares it returns #ALL_WHITE and
/// #ALL_BLACK, and for rooks #WHITE_OO, #WHITE_OOO, #BLACK_OO and #BLACK_OOO.
extern Castling castling_masks[SQ_NB];

/// Board definition
/// \see https://www.chessprogramming.org/Bitboard_Board-Definition
typedef union Board {
	struct {
		U64 WhitePawns;
		U64 WhiteKnights;
		U64 WhiteBishops;
		U64 WhiteRooks;
		U64 WhiteQueens;
		U64 WhiteKing;

		U64 BlackPawns;
		U64 BlackKnights;
		U64 BlackBishops;
		U64 BlackRooks;
		U64 BlackQueens;
		U64 BlackKing;
	};
	U64 pieces[12];		/*!< For more convenient access to pieces by
				index, use this array. */
} Board;

/// Position state
typedef struct PositionState {
	Move previous_move;	///< Previous move
	Castling castling;	///< Able castlings
	uint32_t move_50_rule;	///< 50 move rule counter

	U64 allies;	///< bitboard of all ally pieces
	U64 enemies;	///< bitboard of all enemy pieces
	U64 occupied;	///< bitboard of all occupied squares

	Piece captured_piece;	///< The piece captured on the previous move

	struct PositionState *previous_state;	///< previous position state
} PositionState;

/// Position definition
typedef struct Position {
	PositionState *state;		///< Position state
	Board board;			///< Board
} Position;

/// An enumeration indicating the type of check.
typedef enum CheckType {
	NO_CHECK,	///< No check
	SINGLE_CHECK,	///< \see https://www.chessprogramming.org/Check
	DOUBLE_CHECK,	///< \see https://www.chessprogramming.org/Double_Check
} CheckType;

/**
 * \brief Function to get check type
 *
 * \param position
 *
 * \return One of the three types of check
 */
CheckType get_check_type(const Position *position);

/**
 * \brief Initialize #position from fen
 *
 * \param fen fen string
 *
 * \return pointer to #position or NULL, when fen is incorrect or memory allocation
 * failed.
 *
 * \see https://www.chessprogramming.org/Forsyth-Edwards_Notation
 */
Position* init_position(const char *fen);

/**
 * \brief Returns a bitboard with all the pieces that attacked the square
 *
 * \param position
 *
 * \param target target square
 *
 * \param attacked_color the color of the attackers
 *
 * \return bitboard with all pieces whose attack mask touches the target cell
 */
U64 attacked_by(
	const Position *position,
	Square target,
	Color attackers_color
);

/**
 * \brief Returns the bitboard of pieces on the board
 *
 * \param position
 *
 * \param piece what pieces we need to get
 *
 * \return bitboard with pieces
 */
U64 pieces(const Position *pos, Piece piece);

/**
 * \brief Returns piece on target square
 *
 * \param position
 *
 * \param target target square
 *
 * \return piece on target square
 */
Piece piece_on(const Position *position, Square target);

/**
 * \brief Moves piece from source square to destination square
 * (this function doesn't update occupied field)
 *
 * \param position
 *
 * \param piece
 *
 * \param source source square
 *
 * \param destination destination square
 */
void move_piece(
	Position *position,
	Piece piece,
	Square source,
	Square destination
);

/**
 * \brief Sets piece to square (this function doesn't update any position state
 * field)
 *
 * \param position
 *
 * \param piece
 *
 * \param target target square
 */
void set_piece(Position *position, Piece piece, Square target);

/**
 * \brief Removes piece from square (this function doesn't update any position
 * state field)
 *
 * \param position
 *
 * \param piece
 *
 * \param target target square
 */
void remove_piece(Position *position, Piece piece, Square target);

/**
 * \brief Castling the king
 *
 * \param position
 *
 * \param castling #WHITE_OO or #WHITE_OOO or #BLACK_OO or #BLACK_OOO flag
 */
void do_castling(Position *position, Castling castling);

/**
 * \brief Makes null move for null move pruning
 *
 * \param pos current position
 */
void do_null_move(Position *pos);

/**
 * \brief Undoes null move
 *
 * \param pos current position
 */
void undo_null_move(Position *pos);

/**
 * \brief Makes a move
 *
 * \param position
 *
 * \param move
 */
void do_move(Position *position, Move move);

/**
 * \brief Returns the position to the state before the last move
 *
 * \param position
 */
void undo_move(Position *position);

/**
 * \brief Returns bitboard with pinned pieces near the king.
 * Depends on the turn.
 *
 * \param pos position
 *
 * \return bitboard with pinned pieces
 */
U64 get_pinned(Position *pos);

#endif
