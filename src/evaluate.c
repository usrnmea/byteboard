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
