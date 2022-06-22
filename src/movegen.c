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
		.promotion_piece_type = NO_PIECE_TYPE,
		.color = color_of_piece(piece),
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
			occupied ^ square_to_bitboard(target)
		) & king_moves;

		king_moves &= ~(crossing_moves);

		remove_lsb(linear);
	}

	while (diagonal) {
		U64 crossing_moves = bishop_attacks_mask(
			bit_scan_forward(diagonal),
			occupied ^ square_to_bitboard(target)
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
	Color color
)
{
	assert(pos != NULL);
	assert(color < COLOR_NB);

	Castling cast = NO_CASTLING;

	if(color == WHITE) {
		bool oo = !(
			pos->state->occupied & 0x60ULL
			|| attacked_by(pos, SQ_F1, BLACK)
			|| attacked_by(pos, SQ_G1, BLACK)
		);

		bool ooo = !(
			pos->state->occupied & 0x0CULL
			|| attacked_by(pos, SQ_D1, BLACK)
			|| attacked_by(pos, SQ_C1, BLACK)
		);

		cast = (oo * WHITE_OO) | (ooo * WHITE_OOO);
	} else {
		bool oo = !(
			pos->state->occupied & 0x6000000000000000ULL
			|| attacked_by(pos, SQ_F8, WHITE)
			|| attacked_by(pos, SQ_G8, WHITE)
		);

		bool ooo = !(
			pos->state->occupied & 0x0C00000000000000ULL
			|| attacked_by(pos, SQ_D8, WHITE)
			|| attacked_by(pos, SQ_C8, WHITE)
		);

		cast = (oo * BLACK_OO) | (ooo * BLACK_OOO);
	}

	return cast & pos->state->castling;
}

void generate_castlings(Position *pos, MoveList *move_list)
{
	Castling castlings = pos->state->castling;
	Color color = !pos->state->previous_move.color;

	U64 rooks = pieces(pos, make_piece(color, ROOK));

	castlings &= possible_castlings(
		pos,
		color
	);

	U64 destinations = 0x00ULL;

	while (rooks)
	{
		Square rook = bit_scan_forward(rooks);

		if (castling_masks[rook] & castlings)
			destinations |= square_to_bitboard(rook);

		remove_lsb(rooks);
	}

        add_castlings(
                move_list,
                bit_scan_forward(pieces(pos, make_piece(color, KING))),
                destinations
        );
}

U64 filter_legal_moves(
	Position *pos,
	Square source,
	U64 destinations,
	U64 check_ray
)
{
	U64 source_bitboard = square_to_bitboard(source);
	U64 allies = pos->state->allies;
	U64 pinned = get_pinned(pos);

	Color color = !pos->state->previous_move.color;

	destinations &= check_ray;
	destinations &= ~(allies);

	if (source_bitboard & pinned) {
		U64 king_bitboard = pieces(pos, make_piece(color, KING));
		Square king = bit_scan_forward(king_bitboard);

		U64 blockers = queen_attacks_mask(king, pos->state->occupied);

		U64 sliding = (
			pieces(pos, make_piece(!color, ROOK))
			| pieces(pos, make_piece(!color, BISHOP))
			| pieces(pos, make_piece(!color, QUEEN))
		);

		U64 pinners = queen_attacks_mask(
			king,
			pos->state->occupied ^ blockers
		) & sliding;

		Square pinner = bit_scan_forward(attacked_by(
			pos,
			source,
			!color
		) & pinners);

		U64 pin_ray = ray_between[king][pinner];

		destinations &= pin_ray | square_to_bitboard(pinner);
	}

	return destinations;
}

void generate_knight_moves(
	MoveList *move_list, Position *pos, U64 check_ray
)
{
	Color color = !pos->state->previous_move.color;
	U64 knights = pieces(pos, make_piece(color, KNIGHT));
	U64 allies = pos->state->allies;

	knights &= ~(get_pinned(pos));

	while (knights) {
		Square knight_sq = bit_scan_forward(knights);

		add_common_moves(
			move_list,
			make_piece(color, KNIGHT),
			knight_sq,
			knight_move_pattern(knight_sq) & ~(allies) & check_ray
		);

		remove_lsb(knights);
	}
}

void generate_pawn_moves(
	MoveList *move_list, Position *pos, U64 check_ray
)
{
	Color color = !pos->state->previous_move.color;

	U64 pawns = pieces(pos, make_piece(color, PAWN));

	// Promotions
	U64 pawns_on_last_rank = pawns & (color ? RANK_2 : RANK_7);
	pawns ^= pawns_on_last_rank;

	while (pawns_on_last_rank) {
		Square pawn_sq = bit_scan_forward(pawns_on_last_rank);

		U64 pawn_moves = pawn_mask(
			pawn_sq, pos->state->occupied, color
		);

		pawn_moves = filter_legal_moves(
			pos,
			pawn_sq,
			pawn_moves,
			check_ray
		);

		add_promotions(
			move_list,
			color,
			pawn_sq,
			pawn_moves
		);

		remove_lsb(pawns_on_last_rank);
	}

	// Common moves
	U64 tmp = pawns;
	while (tmp) {
		Square pawn_sq = bit_scan_forward(tmp);

		U64 pawn_moves = pawn_mask(
			pawn_sq, pos->state->occupied, color
		);


		pawn_moves = filter_legal_moves(
			pos,
			pawn_sq,
			pawn_moves,
			check_ray
		);

		add_common_moves(
			move_list,
			make_piece(color, PAWN),
			pawn_sq,
			pawn_moves
		);

		remove_lsb(tmp);
	}

	Move prev_mv = pos->state->previous_move;
	Square dst = prev_mv.destination;
	Square src = prev_mv.source;

	U64 dst_bb = square_to_bitboard(dst);

	if(
		prev_mv.moved_piece_type == PAWN
		&& (dst > src ? dst - src : src - dst) == 16
	) {
		U64 sources = (dst_bb << 1 | dst_bb >> 1) & pawns;

		Square target = dst - 8 + (16 * color);

		remove_piece(pos, make_piece(color, PAWN), target);
		remove_piece(pos, make_piece(!color, PAWN), dst);

		pos->state->occupied ^= dst_bb | sources;
		pos->state->allies ^= sources;
		pos->state->enemies ^= dst_bb;

		CheckType check_type = get_check_type(pos);

		set_piece(pos, make_piece(color, PAWN), target);
		set_piece(pos, make_piece(!color, PAWN), dst);

		pos->state->occupied ^= dst_bb | sources;
		pos->state->allies ^= sources;
		pos->state->enemies ^= dst_bb;

		if(!check_type) {
			add_en_passant(
				move_list,
				color,
				sources,
				target
			);
		}

	}
}

void generate_pawn_promotions(
	MoveList *move_list,
	Position *pos,
	U64 check_ray,
	U64 pawns_on_last_rank
)
{
	assert(move_list != NULL);
	assert(pos != NULL);
}
