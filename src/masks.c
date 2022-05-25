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
	return square_to_bitboard(target);
}

U64 rook_attacks_mask(Square target, U64 occupied)
{
	assert(target < SQ_NB);

	U64 mask_west_occupied = (ray_west[target] & occupied) | 0x01ULL;
	U64 mask_east_occupied = (ray_east[target] & occupied) | 0x8000000000000000ULL;
	U64 mask_north_occupied = (ray_north[target] & occupied) | 0x8000000000000000ULL;
	U64 mask_south_occupied = (ray_south[target] & occupied) | 0x01ULL;

	uint32_t nearest_west_occupied = bit_scan_reverse(mask_west_occupied);
	uint32_t nearest_east_occupied = bit_scan_forward(mask_east_occupied);
	uint32_t nearest_north_occupied = bit_scan_forward(mask_north_occupied);
	uint32_t nearest_south_occupied = bit_scan_reverse(mask_south_occupied);

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
	return square_to_bitboard(target);
}

U64 queen_attacks_mask(Square target, U64 occupied)
{
	assert(target < SQ_NB);
	return square_to_bitboard(target);
}
