#include "bitboard.h"
#include "bitboard_mapping.h"

#include <assert.h>

U64 ray_between[SQ_NB][SQ_NB];

U64 ray_horizontal[SQ_NB];
U64 ray_vertical[SQ_NB];

U64 ray_west[SQ_NB];
U64 ray_south[SQ_NB];
U64 ray_east[SQ_NB];
U64 ray_north[SQ_NB];

U64 ray_diagonal[SQ_NB];
U64 ray_anti_diagonal[SQ_NB];

U64 ray_north_west[SQ_NB];
U64 ray_north_east[SQ_NB];
U64 ray_south_west[SQ_NB];
U64 ray_south_east[SQ_NB];

void init_rays(void) {}

//RAYS
//Returns a ray between 2 squares
U64 get_ray_between(Square from, Square to) {
	assert(from < SQ_NB);
	assert(to < SQ_NB);

	const U64 m1   = -1ULL;
	const U64 a2a7 = 0x0001010101010100ULL;
	const U64 b2g7 = 0x0040201008040200ULL;
	const U64 h1b7 = 0x0002040810204080ULL;
	U64 between, line, rank, file;

	between  = (m1 << from) ^ (m1 << to);
	file  = (to & 7) - (from & 7);
	rank  = ((to | 7) -  from) >> 3 ;
	line  = ((file  &  7) - 1) & a2a7;
	line += 2 * (((rank  &  7) - 1) >> 58);
	line += (((rank - file) & 15) - 1) & b2g7;
	line += (((rank + file) & 15) - 1) & h1b7;
	line *= between & -between;
	return line & between;
}

// This is the function for calculating the west ray
U64 get_ray_west(Square square) {
	assert(square < SQ_NB);
	return (0x01ULL << square) - (0x01ULL << (square & 56));
}

// This is the function for calculating the south ray
U64 get_ray_south(Square square) {
	assert(square < SQ_NB);
	return 0x0080808080808080ULL >> (square ^ 63);
}

// This is the function for calculating the east ray
U64 get_ray_east(Square square) {
	assert(square < SQ_NB);
	return 2 * ((0x01ULL << (square | 7)) - (0x01ULL << square) );
}

// This is the function for calculating the north ray
U64 get_ray_north(Square square) {
	assert(square < SQ_NB);
	return 0x0101010101010100ULL << square;
}

// This is a function for calculating a diagonal ray(including square)
U64 get_ray_diagonal(Square square) {
	assert(square < SQ_NB);

	int32_t diagonal = 8 * (square & 7) - (square & 56);
	int32_t nort = -diagonal & (diagonal >> 31);
	int32_t sout =  diagonal & (-diagonal >> 31);
	return (0x8040201008040201ULL >> sout) << nort;
}

// This is a function for calculating a anti diagonal ray(including square)
U64 get_ray_anti_diagonal(Square square) {
	assert(square < SQ_NB);

	int32_t diagonal = 56 - 8 * (square & 7) - (square & 56);
	int32_t north = -diagonal & ( diagonal >> 31);
	int32_t south =  diagonal & (-diagonal >> 31);
	return (0x0102040810204080ULL >> south) << north;
}

// This is a function for calculating the northeast ray
U64 get_ray_north_east(Square square) {
	assert(square < SQ_NB);
	return get_ray_diagonal(square) & (-2ULL << square);
}

// This is a function for calculating the southwest ray
U64 get_ray_south_west(Square square) {
	assert(square < SQ_NB);
	return get_ray_diagonal(square) & ((1ULL << square) - 1);
}

// This is a function for calculating the anti northwest ray
U64 get_ray_north_west(Square square) {
	assert(square < SQ_NB);
	return get_ray_anti_diagonal(square) & (-2ULL << square);
}

// This is a function for calculating the southeast ray
U64 get_ray_south_east(Square square) {
	assert(square < SQ_NB);
	return get_ray_anti_diagonal(square) & ((1ULL << square) - 1);
}
