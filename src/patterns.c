#include "bitboard.h"
#include "bitboard_mapping.h"
#include "rays.h"
#include "patterns.h"

#include <assert.h>

U64 white_pawn_move_pattern(Square target)
{
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	return (bitboard << 8) | ((bitboard & RANK_2) << 16); 
}

U64 black_pawn_move_pattern(Square target)
{
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	return (bitboard >> 8) | ((bitboard & RANK_7) >> 16); 
}

U64 white_pawn_attack_pattern(Square target)
{

	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	return (
		((bitboard & ~FILE_H) << 9)
		| ((bitboard & ~FILE_A) << 7)
	);
}

U64 black_pawn_attack_pattern(Square target)
{
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	return (
		((bitboard & ~FILE_A) >> 9)
		| ((bitboard & ~FILE_H) >> 7)
	);
}

/// https://www.chessprogramming.org/Knight_Pattern
U64 knight_move_pattern(Square target)
{
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	U64 l1 = (bitboard >> 1) & ~FILE_H;
	U64 l2 = (bitboard >> 2) & ~(FILE_H | FILE_G);
	U64 r1 = (bitboard << 1) & ~FILE_A;
	U64 r2 = (bitboard << 2) & ~(FILE_A | FILE_B);
	return (
		(l1 | r1) << 16
		| (l2 | r2) << 8
		| (l1 | r1) >> 16
		| (l2 | r2) >> 8
	);
}

/// https://www.chessprogramming.org/King_Pattern
U64 king_move_pattern(Square target)
{
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	return (
		(bitboard << 8) | (bitboard >> 8) | ((bitboard << 1) & ~FILE_A)
		| ((bitboard >> 1 ) & ~FILE_H) | ((bitboard << 9) & ~FILE_A)
		| ((bitboard << 7) & ~FILE_H) | ((bitboard >> 9) & ~FILE_H)
		| ((bitboard >> 7) & ~FILE_A)
	);
}

U64 rook_move_pattern(Square target)
{
	assert(target < SQ_NB);

	return ray_vertical[target] ^ ray_horizontal[target];
}

U64 bishop_move_pattern(Square target)
{
	assert(target < SQ_NB);

	return ray_diagonal[target] ^ ray_anti_diagonal[target];
}

U64 queen_move_pattern(Square target)
{
	assert(target < SQ_NB);

	return rook_move_pattern(target) | bishop_move_pattern(target);
}

