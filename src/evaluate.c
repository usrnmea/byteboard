#include "bitboard.h"
#include "piece.h"
#include "position.h"
#include "evaluate.h"

#include <assert.h>
#include <stdlib.h>

Evaluation piece_type_value[PIECE_TYPE_NB] = {
	10, 35, 35, 52, 100, 0
};

Evaluation evaluate_position(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	eval += evaluate_material(pos);

	return NO_EVAL;
}

Evaluation evaluate_material(const Position *pos)
{
	Evaluation value = DRAW;

	for(PieceType pt = PAWN; pt < PIECE_TYPE_NB; pt++) {
		uint32_t piece_count = population_count(
			pieces(pos, make_piece(WHITE, pt))
		);

		value += piece_count * piece_type_value[pt - 1];

		piece_count = population_count(
			pieces(pos, make_piece(BLACK, pt))
		);

		value -= piece_count * piece_type_value[pt - 1];
	}

	return value;
}

Evaluation evaluate_central_pawns(const Position *pos)
{
	assert(pos != NULL);

	Evaluation value = DRAW;

	U64 white_pawns = pieces(pos, W_PAWN);
	U64 black_pawns = pieces(pos, B_PAWN);

	Evaluation central_pawn_value[2] = {5, 2};

	value += population_count(
		0x1818000000ULL & white_pawns
	) * central_pawn_value[0];

	value -= population_count(
		0x1818000000ULL & black_pawns
	) * central_pawn_value[0];

	value += population_count(
		0x3c24243c0000ULL & white_pawns
	) * central_pawn_value[1];

	value -= population_count(
		0x3c24243c0000ULL & black_pawns
	) * central_pawn_value[1];

	return value;
}
