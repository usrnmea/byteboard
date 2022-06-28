#include "bitboard.h"
#include "piece.h"
#include "position.h"
#include "evaluate.h"

#include <assert.h>
#include <stdlib.h>

Evaluation piece_type_value[PIECE_TYPE_NB] = {
	10, 35, 35, 52, 100, 0
};

GamePhase get_game_phase(const Position *pos)
{
	assert(pos != NULL);

	return OPENING;
}

Evaluation evaluate_position(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	eval += evaluate_material(pos);
	eval += evaluate_central_pawns(pos);
	eval += evaluate_doubled_pawns(pos);

	return eval;
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

	value += population_count(
		0x1818000000ULL & white_pawns
	) * 3;

	value -= population_count(
		0x1818000000ULL & black_pawns
	) * 3;

	return value;
}

Evaluation evaluate_doubled_pawns(const Position *pos)
{
	assert(pos != NULL);

	Evaluation value = DRAW;

	U64 white_pawns = pieces(pos, W_PAWN);
	U64 black_pawns = pieces(pos, B_PAWN);

	for(uint32_t i = 0; i < FILE_NB; i++) {
		uint32_t pawns_nb = population_count(white_pawns & files[i]);

		value -= pawns_nb * 5;
		value += !!pawns_nb * 5;

		pawns_nb = population_count(black_pawns & files[i]);

		value += pawns_nb * 5;
		value -= !!pawns_nb * 5;
	}

	return value;
}
