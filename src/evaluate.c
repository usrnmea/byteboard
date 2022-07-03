#include "bitboard.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
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

static Evaluation non_pawn_material(const Position *pos)
{
	assert(pos != NULL);

	Evaluation value = NO_EVAL;

	for(PieceType pt = KNIGHT; pt < PIECE_TYPE_NB; pt++) {
		uint32_t piece_count = population_count(
			pieces(pos, make_piece(WHITE, pt))
		);

		piece_count -= population_count(
			pieces(pos, make_piece(BLACK, pt))
		);

		value += piece_count * piece_type_value[MIDDLEGAME][pt - 1];
	}

	return value;
}

int32_t get_phase(const Position *pos)
{
	assert(pos != NULL);

	const Evaluation midgame_limit = 6880;
	const Evaluation endgame_limit = 3400;

	Evaluation npm = non_pawn_material(pos);

	npm = MAX(
		endgame_limit,
		MIN(npm, midgame_limit)
	);

	npm = (
		((npm - endgame_limit) * 100) / (midgame_limit - endgame_limit)
	) << 0;

	return npm;
}

#undef MAX
#undef MIN

Evaluation evaluate_position(const Position *pos)
{
	assert(pos != NULL);

	if(pos->state->move_50_rule == 50)
		return DRAW;

	int32_t phase = get_phase(pos);

	Evaluation eval = NO_EVAL;

	eval += evaluate_doubled_pawns(pos);

	Evaluation midgame_eval = evaluate_midgame(pos);
	Evaluation endgame_eval = evaluate_endgame(pos);

	midgame_eval += eval;
	endgame_eval += eval;

	eval = (
		(
			(midgame_eval * phase + (
				(endgame_eval * (128 - phase)) << 0
			)
		) / 128) << 0
	);

	return eval;
}

Evaluation evaluate_midgame(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	eval += evaluate_material(pos, MIDDLEGAME);
	eval += evaluate_mobility(pos, MIDDLEGAME);
	eval += evaluate_central_pawns(pos);

	return eval;
}

Evaluation evaluate_endgame(const Position *pos)
{
	assert(pos != NULL);

	Evaluation eval = NO_EVAL;

	eval += evaluate_material(pos, ENDGAME);
	eval += evaluate_mobility(pos, ENDGAME);
	eval += evaluate_passed_pawns(pos);

	return eval;
}

Evaluation evaluate_mobility(const Position *pos, GamePhase gp)
{
	assert(pos != NULL);
	assert(gp < GAME_PHASES_NB);

	Evaluation eval = DRAW;

	U64 occ = pos->state->occupied;

	U64 w_tmp = pieces(pos, W_BISHOP);
	U64 b_tmp = pieces(pos, B_BISHOP);

	while(w_tmp) {
		Square target = bit_scan_forward(w_tmp);

		eval += population_count(bishop_attacks_mask(target, occ));

		remove_lsb(w_tmp);
	}

	while(b_tmp) {
		Square target = bit_scan_forward(b_tmp);

		eval -= population_count(bishop_attacks_mask(target, occ));

		remove_lsb(b_tmp);
	}

	w_tmp = pieces(pos, W_KNIGHT);
	b_tmp = pieces(pos, B_KNIGHT);

	while(w_tmp) {
		Square target = bit_scan_forward(w_tmp);

		eval += population_count(knight_move_pattern(target));

		remove_lsb(w_tmp);
	}

	while(b_tmp) {
		Square target = bit_scan_forward(b_tmp);

		eval -= population_count(knight_move_pattern(target));

		remove_lsb(b_tmp);
	}

	if (gp == MIDDLEGAME)
		return eval;

	w_tmp = pieces(pos, W_ROOK);
	b_tmp = pieces(pos, B_ROOK);

	while(w_tmp) {
		Square target = bit_scan_forward(w_tmp);

		eval += population_count(rook_attacks_mask(target, occ));

		remove_lsb(w_tmp);
	}

	while(b_tmp) {
		Square target = bit_scan_forward(b_tmp);

		eval -= population_count(rook_attacks_mask(target, occ));

		remove_lsb(b_tmp);
	}

	w_tmp = pieces(pos, W_QUEEN);
	b_tmp = pieces(pos, B_QUEEN);

	while(w_tmp) {
		Square target = bit_scan_forward(w_tmp);

		eval += population_count(queen_attacks_mask(target, occ)) % 4;

		remove_lsb(w_tmp);
	}

	while(b_tmp) {
		Square target = bit_scan_forward(b_tmp);

		eval -= population_count(queen_attacks_mask(target, occ)) % 4;

		remove_lsb(b_tmp);
	}

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

		piece_count -= population_count(
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

Evaluation evaluate_passed_pawns(const Position *pos)
{
	assert(pos != NULL);

	Evaluation value = DRAW;

	U64 white_pawns = pieces(pos, W_PAWN);
	U64 black_pawns = pieces(pos, B_PAWN);

	U64 tmp = white_pawns;

	while(tmp) {
		Square target = bit_scan_forward(tmp);
		remove_lsb(tmp);

		if(ray_north[target] & black_pawns)
			continue;

		U64 file = files[file_of(target)];

		value += population_count(
			file & ray_south[target]
		) * 2;
	}

	tmp = black_pawns;

	while(tmp) {
		Square target = bit_scan_forward(tmp);
		remove_lsb(tmp);

		if(ray_south[target] & white_pawns)
			continue;

		U64 file = files[file_of(target)];

		value -= population_count(
			file & ray_north[target]
		) * 2;
	}

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
