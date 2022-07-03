/**
 * \file
 */
#ifndef __BITBOARD_MAPPING_H__
#define __BITBOARD_MAPPING_H__

#include "bitboard.h"

/// Converts square to bitboard(U64)
#define square_to_bitboard(square) (0x01ULL << (square))

/// Returns the file square index
#define file_of(square) ((square) % 8)
/// Returns the rank square index
#define rank_of(square) ((square) / 8)

/**
 * \brief A set of all 64 squares enumerated from 0..63. Each bit of the
 * bitboard can be marked with one chessboard. We use little-endian,
 * A1 = 0 and H8 = 63.
 */
typedef enum Square {
	SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
	SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
	SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
	SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
	SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
	SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
	SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
	SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,

	SQ_NB = 64,	///< Number of squares
	SQ_NONE = 100	/*!< None Square. If you were expecting a normal square
			but got this one, then there is most likely an error in
			the code. Use SQ_NONE to fill empty structures. */
} Square;



#endif
