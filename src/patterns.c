#include "bitboard.h"
#include "bitboard_mapping.h"
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
	return square_to_bitboard(target);
}

U64 black_pawn_attack_pattern(Square target)
{
	return square_to_bitboard(target);
}

U64 knight_move_pattern(Square target)
{
	return square_to_bitboard(target);
}

U64 king_move_pattern(Square target)
{
	return square_to_bitboard(target);
}

U64 rook_move_pattern(Square target)
{
	return square_to_bitboard(target);
}

U64 bishop_move_pattern(Square target)
{
	return square_to_bitboard(target);
}

U64 queen_move_pattern(Square target)
{
	return square_to_bitboard(target);
}

