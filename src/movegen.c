#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"

#include <stdlib.h>
#include <assert.h>

/*
Initializes the list of moves.
The creation of the list of moves should be
done only through this function
*/
MoveList *init_move_list(void)
{
	MoveList *move_list = malloc(sizeof(MoveList));
	assert(move_list != NULL);

	move_list->last = move_list->move_list;

	return move_list;
}

void add_common_moves(
	MoveList *move_list,
	const Piece piece,
	const Square source,
	U64 destinations
)
{
	assert(move_list != NULL);
	assert(source < SQ_NB);
	assert(piece != NO_PIECE);

	Move move = {
		.move_type = COMMON, .moved_piece_type = type_of_piece(piece),
		.promotion_piece_type = NO_PIECE_TYPE, .color = color_of_piece(piece),
		.source = source
	};

	ExtMove ext_move = {.move = move, .eval = NO_EVAL};

	while (destinations) {
		ext_move.move.destination = bit_scan_forward(destinations);

		ml_add(move_list, ext_move);

		remove_lsb(destinations);
	}
}

void add_castlings(
	MoveList *move_list,
	const Square source,
	U64 destinations
)
{
	assert(move_list != NULL);
	assert(source < SQ_NB);

	Move move = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE,
		.color = source == SQ_E8, .source = source
	};

	ExtMove ext_move = {.move = move, .eval = NO_EVAL};

	while (destinations) {
		ext_move.move.destination = bit_scan_forward(destinations);

		ml_add(move_list, ext_move);

		remove_lsb(destinations);
	}
}

void add_promotions(
	MoveList *move_list,
	Color color,
	Square source,
	U64 destinations
)
{
	assert(move_list != NULL);
	assert(source < SQ_NB);
	assert(color < COLOR_NB);

	Move move = {
		.move_type = PROMOTION, .moved_piece_type = PAWN,
		.color = color, .source = source
	};

	ExtMove ext_move = {
		.move = move, .eval = NO_EVAL
	};

	while (destinations) {
		ext_move.move.destination = bit_scan_forward(destinations);

		for(PieceType pc_type = KNIGHT; pc_type < KING; pc_type++) {
			ext_move.move.promotion_piece_type = pc_type;
			ml_add(move_list, ext_move);
		}

		remove_lsb(destinations);
	}
}

void add_en_passant(
	MoveList *move_list,
	Color color,
	U64 sources,
	Square destination
)
{
	assert(move_list != NULL);
	assert(destination < SQ_NB);
	assert(population_count(sources) <= 2);
	assert(color < COLOR_NB);

	Move move = {
		.move_type = EN_PASSANT, .moved_piece_type = PAWN,
		.promotion_piece_type = NO_PIECE_TYPE,
		.color = color, .destination = destination
	};

	ExtMove ext_move = {
		.move = move, .eval = NO_EVAL
	};

	while (sources) {
		ext_move.move.source = bit_scan_forward(sources);

		ml_add(move_list, ext_move);

		remove_lsb(sources);
	}
}

U64 king_safe_moves_mask(
	Position *pos,
	Square target,
	Color color
)
{
	assert(pos != NULL);
	assert(target < SQ_NB);
	assert(color < COLOR_NB);

	U64 king_moves = king_move_pattern(target);

	U64 linear = (
		pieces(pos, make_piece(!color, ROOK))
		| pieces(pos, make_piece(!color, QUEEN))
	);

	U64 diagonal = (
		pieces(pos, make_piece(!color, BISHOP))
		| pieces(pos, make_piece(!color, QUEEN))
	);

	U64 knights = pieces(pos, make_piece(!color, KNIGHT));
	U64 pawns = pieces(pos, make_piece(!color, PAWN));
	U64 king = pieces(pos, make_piece(!color, KING));

	U64 occupied = pos->state->occupied;

	while (linear) {
		U64 crossing_moves = rook_attacks_mask(
			bit_scan_forward(linear),
			occupied ^ (0x01ULL << target)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(linear);
	}

	while (diagonal) {
		U64 crossing_moves = bishop_attacks_mask(
			bit_scan_forward(diagonal),
			occupied ^ (0x01ULL << target)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(diagonal);
	}

	while (knights) {
		U64 crossing_moves = knight_move_pattern(
			bit_scan_forward(knights)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(knights);
	}

	while (pawns) {
		U64 crossing_moves = pawn_move_pattern[color](
			bit_scan_forward(pawns)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(pawns);
	}

	while (king) {
		U64 crossing_moves = king_move_pattern(
			bit_scan_forward(king)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(king);
	}

	return (
		king_moves 
		^ pieces(pos, make_piece(color, PAWN))
		^ pieces(pos, make_piece(color, KNIGHT))
		^ pieces(pos, make_piece(color, BISHOP))
		^ pieces(pos, make_piece(color, ROOK))
		^ pieces(pos, make_piece(color, QUEEN))
	) & king_moves;
}

Castling possible_castlings(
	Position *pos,
	Color color,
	Square target
)
{
	assert(pos != NULL);
	assert(color < COLOR_NB);
	assert(target < SQ_NB);

	return NO_CASTLING;
}

void generate_castlings(Position *pos, MoveList *move_list)
{
	Castling castlings = pos->state->castling;
	Color color = !pos->state->previous_move.color;

	U64 rooks = pieces(pos, make_piece(color, ROOK));

	castlings &= possible_castlings(
		pos,
		color,
		pieces(pos, make_piece(color, KING))
	);

	U64 destinations = 0x00ULL;

	while (rooks)
	{
		Square rook = bit_scan_forward(rooks);

		if (castling_masks[rook] & castlings)
			destinations |= rook;

		remove_lsb(rooks);
	}

	add_castlings(
		move_list,
		pieces(pos, make_piece(color, KING)),
		destinations
	);
}
