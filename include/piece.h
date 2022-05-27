/**
 * \file piece.h
 */
#ifndef __PIECE_H__
#define __PIECE_H__

#include <stdint.h>

/// Determines the type of piece
#define type_of_piece(piece) ((piece) & 7)

/// Determines the color of piece
#define color_of_piece(piece) ((piece) > 8)

/// Creates a piece from the given color and type.
#define make_piece(color, piece_type) ((color << 3) + (piece_type))


/**
 * \brief A set of all chess pieces which is represented in numbers from 1 to 6
 * respectively. This one doesn't take into account the color of the piece.
*/
typedef enum PieceType {
	NO_PIECE_TYPE,	/*!< If it is needed, the absence of the piece type
			equals zero. */
	PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
	PIECE_TYPE_NB = 6,	///< Number of types
} PieceType;

/**
 * \brief This enumerate represents chess pieces in numbers like PieceType does,
 * however takes into account them color. White pieces are represented from
 * 1 to 6 and black pieces are represented from 9 to 14.
*/
typedef enum Piece {
	NO_PIECE = 0,	/*!< If it is needed, the absence of the piece
			equals zero. */
	W_PAWN = 1, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
	B_PAWN = 9, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
	PIECE_NB = 12,	///< Number of pieces
} Piece;

/**
 * \brief The enumeration of chess colors, white is 0, black is 1.
*/
typedef enum Color {
	WHITE = 0, BLACK = 1,
	COLOR_NB = 2	///< Number of colors.
} Color;



#endif
