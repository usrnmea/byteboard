/**
 * \file
 */
#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"

/** Stores function pointers needed to get the pawn move pattern.
 * Example:
 * \code{.c}
 * pawn_move_pattern[WHITE](SQ_C3);
 * \endcode
 * See white_pawn_move_pattern() and black_pawn_move_pattern()
 */
extern U64 (*pawn_move_pattern[COLOR_NB]) (Square);
/** Stores function pointers needed to get the pawn attack pattern.
 * Example:
 * \code{.c}
 * pawn_attack_pattern[WHITE](SQ_C3);
 * \endcode
 * See white_pawn_attack_pattern() and black_pawn_attack_pattern()
 */
extern U64 (*pawn_attack_pattern[COLOR_NB]) (Square);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the white pawn can move
 *
 * \param target square with pawn
 *
 * \return bitboard with pawn move patter
 */
U64 white_pawn_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the black pawn can move
 *
 * \param target square with pawn
 *
 * \return bitboard with pawn move patter
 */
U64 black_pawn_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares which the white pawn attacks
 *
 * \param target square with pawn
 *
 * \return bitboard with pawn move patter
 */
U64 white_pawn_attack_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which black pawn attacks
 *
 * \param target square with pawn
 *
 * \return bitboard with pawn move patter
 */
U64 black_pawn_attack_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the knight can move
 *
 * \param target square with knight
 *
 * \return bitboard with knight move pattern
 */
U64 knight_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the king can move
 *
 * \param target square with king 
 *
 * \return bitboard with king move pattern
 */
U64 king_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the rook can move
 *
 * \param target square with rook
 *
 * \return bitboard with rook move pattern
 */
U64 rook_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the bishop can move
 *
 * \param target square with bishop 
 *
 * \return bitboard with bishop move pattern
 */
U64 bishop_move_pattern(Square target);

/**
 * \brief Returns the bit of the board, where bits equal to one indicate the
 * squares to which the queen can move
 *
 * \param target square with queen 
 *
 * \return bitboard with queen move pattern
 */
U64 queen_move_pattern(Square target);

#endif
