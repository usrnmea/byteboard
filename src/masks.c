#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"
#include "rays.h"

#include <assert.h>


U64 pawn_move_mask(Square target, U64 occupied, Color color)
{
	assert(color < COLOR_NB);
	assert(target < SQ_NB);

	U64 bitboard = square_to_bitboard(target);

	bitboard = (bitboard << 8) >> (color * 16) & ~occupied;
	U64 bitboard_2 = bitboard & ((RANK_3 * !color) | (RANK_6 * color));
	bitboard |= (bitboard_2 << 8) >> (color * 16) & ~occupied;

	return bitboard;
}

U64 pawn_attack_mask(Square target, U64 occupied, Color color)
{
	assert(target < SQ_NB);
	assert(occupied != EMPTY);
	assert(color < COLOR_NB);

	return EMPTY;
}

U64 pawn_mask(Square target, U64 occupied, Color color)
{
	assert(target < SQ_NB);
	assert(occupied != EMPTY);
	assert(color < COLOR_NB);

	return EMPTY;
}

U64 rook_attacks_mask(Square target, U64 occupied)
{
	assert(target < SQ_NB);

	// Setting the least significant bit(| 0x01ULL and |0x80...00ULL)
	// ensures to scan at least an outer square, which would address an
	// empty ray set anyway, therefor not affecting the final result with
	// no blocker or a most outer one.

        U64 mask_west_occupied = (
		(ray_west[target] & occupied) | 0x01ULL
	);
	U64 mask_east_occupied = (
		(ray_east[target] & occupied) | 0x8000000000000000ULL
	);
	U64 mask_north_occupied = (
		(ray_north[target] & occupied) | 0x8000000000000000ULL
	);
	U64 mask_south_occupied = (
		(ray_south[target] & occupied) | 0x01ULL
	);

	Square nearest_west_occupied = bit_scan_reverse(mask_west_occupied);
	Square nearest_east_occupied = bit_scan_forward(mask_east_occupied);
	Square nearest_north_occupied = bit_scan_forward(mask_north_occupied);
	Square nearest_south_occupied = bit_scan_reverse(mask_south_occupied);

	U64 west_occupied_ray = ~(ray_west[nearest_west_occupied]);
	U64 east_occupied_ray = ~(ray_east[nearest_east_occupied]);
	U64 north_occupied_ray = ~(ray_north[nearest_north_occupied]);
	U64 south_occupied_ray = ~(ray_south[nearest_south_occupied]);

	U64 west_ray_mask = ray_west[target] & west_occupied_ray;
	U64 east_ray_mask = ray_east[target] & east_occupied_ray;
	U64 north_ray_mask = ray_north[target] & north_occupied_ray;
	U64 south_ray_mask = ray_south[target] & south_occupied_ray;

	return south_ray_mask | north_ray_mask | east_ray_mask | west_ray_mask;
}

U64 bishop_attacks_mask(Square target, U64 occupied)
{
	assert(target < SQ_NB);

	U64 north_east_ray = ray_north_east[target];
	U64 north_west_ray = ray_north_west[target];
	U64 south_east_ray = ray_south_east[target];
	U64 south_west_ray = ray_south_west[target];

	// Setting the least significant bit(| 0x01ULL and |0x80...00ULL)
	// ensures to scan at least an outer square, which would address an
	// empty ray set anyway, therefor not affecting the final result with
	// no blocker or a most outer one.

	Square nearest_north_east_occupied = bit_scan_forward(
		(north_east_ray & occupied) | 0x8000000000000000ULL
	);

	Square nearest_north_west_occupied = bit_scan_forward(
		(north_west_ray & occupied) | 0x8000000000000000ULL
	);

	Square nearest_south_east_occupied = bit_scan_reverse(
		(south_east_ray & occupied) | 0x01ULL
	);

	Square nearest_south_west_occupied = bit_scan_reverse(
		(south_west_ray & occupied) | 0x01ULL
	);

	north_east_ray &= ~(ray_north_east[nearest_north_east_occupied]);
	north_west_ray &= ~(ray_north_west[nearest_north_west_occupied]);
	south_east_ray &= ~(ray_south_east[nearest_south_east_occupied]);
	south_west_ray &= ~(ray_south_west[nearest_south_west_occupied]);


	return north_east_ray | north_west_ray | south_east_ray | south_west_ray;
}

U64 queen_attacks_mask(Square target, U64 occupied)
{
	assert(target < SQ_NB);

	return rook_attacks_mask(target, occupied) |
		bishop_attacks_mask(target, occupied);
}
