/**
 * \file bitboard.h
 */
#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <stdint.h>

/// bitboard typedef(also called bitsets or bitmaps)
/// used to represent the board inside a chess program in
/// a piece centric manner.
/// \see https://www.chessprogramming.org/Bitboards
typedef uint64_t U64;

/// Universal set. Contains all elements by setting all bits to binary one.
extern const U64 UNIVERSE;
/// Empty set. Represented by all bits zero.
extern const U64 EMPTY;

/// [RANK_1;RANK_8] bitboard represenration of chess ranks(horizontal lines). 
extern const U64 RANK_8;
extern const U64 RANK_7;
extern const U64 RANK_6;
extern const U64 RANK_5;
extern const U64 RANK_4;
extern const U64 RANK_3;
extern const U64 RANK_2;
extern const U64 RANK_1;

/// [FILE_1;FILE_8] bitboard represenration of chess files(vertical lines). 
extern const U64 FILE_H;
extern const U64 FILE_G;
extern const U64 FILE_F;
extern const U64 FILE_E;
extern const U64 FILE_D;
extern const U64 FILE_C;
extern const U64 FILE_B;
extern const U64 FILE_A;

/// Bitboard of all light squares (1 = light, 0 = dark)
extern const U64 LIGHT_SQUARES;
/// Bitboard of all dark squares (1 = dark, 0 = light)
extern const U64 DARK_SQUARES;

/// A set of indexes used in the implementation of the bit_scan functions
/// \see https://www.chessprogramming.org/BitScan#With_separated_LS1B
static const uint32_t bit_scan_indexes[64];

/**
 * \brief a function that determines the bit-index of the least significant 1
 * bit (LS1B) in an integer such as bitboards
 *
 * \param bitboard target bitboard
 *
 * \return bit-index of the least significant 1 bit
 *
 * \see https://www.chessprogramming.org/BitScan
 */
uint32_t bit_scan_forward(U64 bitboard);

/**
 * \brief a function that determines the bit-index of the most significant 1
 * bit (MS1B) in an integer such as bitboards
 *
 * \param bitboard target bitboard
 *
 * \return bit-index of the most significant 1 bit 
 *
 * \see https://www.chessprogramming.org/BitScan
 */
uint32_t bit_scan_reverse(U64 bitboard);

#endif
