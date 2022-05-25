/**
 * \file
 */
#ifndef __MASKS_H__
#define __MASKS_H__

#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the pawn can move, not ignoring enemy pieces.
 *
 * \param target square with pawn
 *
 * \param occupied bitboard with chess pieces
 *
 * \param color pawn color
 *
 * \return bitboard with pawn move mask
 */
U64 pawn_move_mask(Square target, U64 occupied, Color color);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the rook can move, not ignoring enemy pieces.
 *
 * \param target square with rook
 *
 * \param occupied bitboard with chess pieces
 *
 * \return bitboard with rook move mask
 */
U64 rook_attacks_mask(Square target, U64 occupied);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the bishop can move, not ignoring enemy pieces.
 *
 * \param target square with bishop
 *
 * \param occupied bitboard with chess pieces
 *
 * \return bitboard with bishop move mask
 */
U64 bishop_attacks_mask(Square target, U64 occupied);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the queen can move, not ignoring enemy pieces.
 *
 * \param target square with queen
 *
 * \param occupied bitboard with chess pieces
 *
 * \return bitboard with queen move mask
 */
U64 queen_attacks_mask(Square target, U64 occupied);

#endif
