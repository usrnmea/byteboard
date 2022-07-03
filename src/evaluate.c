#include "bitboard.h"
#include "piece.h"
#include "position.h"
#include "evaluate.h"

#include <assert.h>
#include <stdlib.h>

Evaluation piece_type_value[GAME_PHASES_NB][PIECE_TYPE_NB] = {
	{100, 320, 320, 500, 1000, 0},
	{150, 320, 320, 550, 1050, 0},
};

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static Evaluation non_pawn_material(const Position *pos, Color color)
{
	assert(pos != NULL);
	assert(color < COLOR_NB);

	Evaluation value = NO_EVAL;

	for(PieceType pt = KNIGHT; pt < PIECE_TYPE_NB; pt++) {
		uint32_t piece_count = population_count(
			pieces(pos, make_piece(color, pt))
		);

		value += piece_count * piece_type_value[MIDDLEGAME][pt - 1];
	}

	return value;
}

GamePhase get_game_phase(const Position *pos)
{
	assert(pos != NULL);

	const Evaluation midgame_limit = 6880;
	const Evaluation endgame_limit = 3400;

	Evaluation npm = non_pawn_material(pos, WHITE);
	npm += non_pawn_material(pos, BLACK);

	npm = MAX(
		endgame_limit,
		MIN(npm, midgame_limit)
	);

	npm = (
		((npm - endgame_limit) * 100) / (midgame_limit - endgame_limit)
	) << 0;

	// TODO: Use the npm as a coefficient of the phase of the game
	if(npm > 10)
		return MIDDLEGAME;
	else
		return ENDGAME;
}

#undef MAX
#undef MIN

Evaluation evaluate_position(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	eval += evaluate_material(pos, MIDDLEGAME);

	return eval;
}

Evaluation evaluate_midgame(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	return eval;
}

Evaluation evaluate_endgame(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	return eval;
}

Evaluation evaluate_material(const Position *pos, GamePhase gp)
{
	assert(pos != NULL);
	assert(gp < GAME_PHASES_NB);

	Evaluation value = DRAW;

	for(PieceType pt = PAWN; pt < PIECE_TYPE_NB; pt++) {
		uint32_t piece_count = population_count(
			pieces(pos, make_piece(WHITE, pt))
		);

		piece_count = population_count(
			pieces(pos, make_piece(BLACK, pt))
		);

		value += piece_count * piece_type_value[gp][pt - 1];
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
	) * 10;

	value -= population_count(
		0x1818000000ULL & black_pawns
	) * 10;

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

		value -= pawns_nb * 15;
		value += !!pawns_nb * 15;

		pawns_nb = population_count(black_pawns & files[i]);

		value += pawns_nb * 15;
		value -= !!pawns_nb * 15;
	}

	return value;
}

Evaluation evaluate_king_position(const Position *pos, GamePhase gp)
{
	assert(pos != NULL);

	Evaluation value = NO_EVAL;

	if(gp == MIDDLEGAME) {
		U64 safest_squares[COLOR_NB] = {
			0x46,
			0x4600000000000000ULL,
		};

		U64 unsafe_squares[COLOR_NB] = {
			0x3cffffffff3c00ULL,
			0x3cffffffff3c00ULL,
		};

		U64 king = pieces(pos, W_KING);

		value += !!(safest_squares[WHITE] & king) * 20;
		value -= !!(unsafe_squares[WHITE] & king) * 40;

		king = pieces(pos, B_KING);

		value -= !!(safest_squares[BLACK] & king) * 20;
		value += !!(unsafe_squares[BLACK] & king) * 40;
	} else {
		U64 worst = 0xe7c381000081c3e7ULL;
		U64 near_center = 0x3c664242663c00ULL;
		U64 center = 0x183c3c180000ULL;

		U64 king = pieces(pos, W_KING);

		value -= !!(worst & king) * 40;
		value += !!(near_center & king) * 10;
		value += !!(center & king) * 40;

		king = pieces(pos, B_KING);

		value += !!(worst & king) * 40;
		value -= !!(near_center & king) * 10;
		value -= !!(center & king) * 40;
	}

	return value;
}
