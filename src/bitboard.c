#include "bitboard.h"

#include <stdint.h>
#include <assert.h>

const U64 UNIVERSE = 0xFFFFFFFFFFFFFFFFULL;
const U64 EMPTY = 0x0000000000000000ULL;

const U64 RANK_8 = 0xFF00000000000000ULL;
const U64 RANK_7 = 0x00FF000000000000ULL;
const U64 RANK_6 = 0x0000FF0000000000ULL;
const U64 RANK_5 = 0x000000FF00000000ULL;
const U64 RANK_4 = 0x00000000FF000000ULL;
const U64 RANK_3 = 0x0000000000FF0000ULL;
const U64 RANK_2 = 0x000000000000FF00ULL;
const U64 RANK_1 = 0x00000000000000FFULL;

const U64 FILE_H = 0x8080808080808080ULL;
const U64 FILE_G = 0x4040404040404040ULL;
const U64 FILE_F = 0x2020202020202020ULL;
const U64 FILE_E = 0x1010101010101010ULL;
const U64 FILE_D = 0x0808080808080808ULL;
const U64 FILE_C = 0x0404040404040404ULL;
const U64 FILE_B = 0x0202020202020202ULL;
const U64 FILE_A = 0x0101010101010101ULL;

const U64 LIGHT_SQUARES = 0x55AA55AA55AA55AAULL;
const U64 DARK_SQUARES = 0xAA55AA55AA55AA55ULL;

static const uint32_t bit_scan_indexes[64] = {
	0,  47, 1,  56, 48, 27, 2,  60,
	57, 49, 41, 37, 28, 16, 3,  61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11, 4,  62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30, 9,  24,
	13, 18, 8,  12, 7,  6,  5,  63
};

/// \see https://www.chessprogramming.org/BitScan#With_separated_LS1B
uint32_t bit_scan_forward(U64 bitboard)
{
	assert(bitboard != EMPTY);

	return bit_scan_indexes[
		((bitboard ^ (bitboard - 1ULL)) * 0x03f79d71b4cb0a89ULL) >> 58
	];
}

/// \see https://www.chessprogramming.org/BitScan#De_Bruijn_Multiplication_2
uint32_t bit_scan_reverse(U64 bitboard)
{
	assert(bitboard != EMPTY);

	bitboard |= bitboard >> 1ULL;
	bitboard |= bitboard >> 2ULL;
	bitboard |= bitboard >> 4ULL;
	bitboard |= bitboard >> 8ULL;
	bitboard |= bitboard >> 16ULL;
	bitboard |= bitboard >> 32ULL;
	return bit_scan_indexes[(bitboard * 0x03f79d71b4cb0a89ULL) >> 58ULL];
}

/// \see
/// https://www.chessprogramming.org/Population_Count#Brian_Kernighan.27s_way
uint32_t population_count(U64 bitboard)
{
	uint32_t count = 0;

	while(bitboard) {
		count++;

		remove_lsb(bitboard);
	}

	return count;
}
