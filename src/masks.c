#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"


U64 pawn_move_mask(Square target, U64 mask, Color color)
{
	return square_to_bitboard(target);
}

U64 rook_attacks_mask(Square target, U64 mask)
{
	return square_to_bitboard(target);
}

U64 bishop_attacks_mask(Square target, U64 mask)
{
	return square_to_bitboard(target);
}

U64 queen_attacks_mask(Square target, U64 mask)
{
	return square_to_bitboard(target);
}
