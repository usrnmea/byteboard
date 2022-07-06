#include "bitboard.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "search.h"
#include "uci.h"

#include <assert.h>
#include <stdlib.h>

U64 side_key = 0;
U64 hash_hey = 0;

/// Seed for init hash keys
uint32_t random_state = 1804289383;

uint32_t get_random_U32_number(void)
{
    uint32_t number = random_state;

    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    random_state = number;

    return number;
}

U64 get_random_U64_number(void)
{
    U64 n1, n2, n3, n4;

    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

void init_hash_keys(void)
{
	for (uint32_t i = 0; i < 12; i++) {
		for (Square sq = SQ_A1; sq < SQ_NB; sq++)
			piece_keys[i][sq] = get_random_U64_number();
	}

	for (Square sq = SQ_A1; sq < SQ_NB; sq++)
		en_passant_keys[sq] = get_random_U64_number();

	side_key = get_random_U64_number();

	castling_keys[NO_CASTLING] = get_random_U64_number();
	castling_keys[WHITE_OO] = get_random_U64_number();
	castling_keys[WHITE_OOO] = get_random_U64_number();
	castling_keys[BLACK_OO] = get_random_U64_number();
	castling_keys[BLACK_OOO] = get_random_U64_number();
	castling_keys[KING_SIDE] = get_random_U64_number();
	castling_keys[QUEEN_SIDE] = get_random_U64_number();
	castling_keys[ALL_WHITE] = get_random_U64_number();
	castling_keys[ALL_BLACK] = get_random_U64_number();
	castling_keys[ALL_CASTLING] = get_random_U64_number();
}

U64 generate_hash_key(const Position *pos)
{
	assert(pos != NULL);

	U64 final_key = 0x00ULL;

	for (uint32_t i = 0; i < 12; i++) {
		U64 bb = pos->board.pieces[i];

		while (bb) {
			Square sq = bit_scan_forward(bb);

			final_key ^= piece_keys[i][sq];

			remove_lsb(bb);
		}
	}

	Square dst = pos->state->previous_move.destination;
	Square src = pos->state->previous_move.source;

	if (
		pos->state->previous_move.moved_piece_type == PAWN &&
		(dst > src ? dst - src : src - dst) == 16
	)
		final_key ^= en_passant_keys[(
			dst > src ? dst - 8 : src - 8
		)];

	final_key ^= castling_keys[pos->state->castling];

	if ((!pos->state->previous_move.color) == BLACK)
		final_key ^= side_key;

	return final_key;
}
