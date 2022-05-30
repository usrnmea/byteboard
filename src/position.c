#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

Castling castling_masks[SQ_NB] = {
	WHITE_OOO, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	ALL_WHITE, NO_CASTLING, NO_CASTLING, WHITE_OO,

	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	NO_CASTLING, NO_CASTLING, NO_CASTLING, NO_CASTLING,

	BLACK_OOO, NO_CASTLING, NO_CASTLING, NO_CASTLING,
	ALL_BLACK, NO_CASTLING, NO_CASTLING, BLACK_OO,
};

/**
 * \brief Initializes the first part of FEN. Returns True if everything went
 * well, false otherwise.
 *
 * \param position
 *
 * \param board Part of a FEN pointing to the first part of FEN
 *
 * \see https://www.chessprogramming.org/Forsyth-Edwards_Notation
 */
static bool init_board(Position *position, char* board)
{
	if(board == NULL || position == NULL)
		return false;

	char *rank = strtok(board, "/");

	for(int rank_number = 0; rank_number < 8; rank_number++) {
		if(rank == NULL)
			return false;

		// The index of the current character in the rank variable
		uint32_t index = 0;

		for(int square_number = 0; square_number < 8; index++) {
			if (isdigit(rank[index])) {
				square_number += rank[index] - '0';

				if(square_number > 8)
					return false;

				continue;
			}

			Color color = WHITE;

			if(islower(rank[index]))
				color = BLACK;

			PieceType piece_type;

			switch(tolower(rank[index])) {
				case 'p':
					piece_type = PAWN;
					break;
				case 'n':
					piece_type = KNIGHT;
					break;
				case 'k':
					piece_type = KING;
					break;
				case 'b':
					piece_type = BISHOP;
					break;
				case 'r':
					piece_type = ROOK;
					break;
				case 'q':
					piece_type = QUEEN;
					break;
				default:
					return false;
			}

			/// FEN stores ranks in reverse order for us
			Square target = (7 - rank_number) * 8 + square_number;

			set_piece(
				position, make_piece(color, piece_type), target
			);

			square_number += 1;
		}

		rank = strtok(NULL, "/");
	}

	return true;
}

/**
 * \brief Initializes the second part of FEN. Returns True if everything went
 * well, false otherwise.
 *
 * \param position
 *
 * \param board Part of a FEN pointing to the second part of FEN
 *
 * \see https://www.chessprogramming.org/Forsyth-Edwards_Notation
 */
static bool init_turn(Position *position, char *turn)
{
	if(position == NULL || turn == NULL)
		return false;

	if(strlen(turn) != 1 || (*turn != 'b' && *turn != 'w'))
		return false;

	position->state->previous_move = (Move) {
		.color = !(*turn == 'b'),
		.move_type = COMMON,
		.source = SQ_NONE,
		.destination = SQ_NONE,
		.moved_piece_type = NO_PIECE_TYPE,
		.promotion_piece_type = NO_PIECE_TYPE,
	};

	return true;
}

/**
 * \brief Initializes the third part of FEN. Returns True if everything went
 * well, false otherwise.
 *
 * \param position
 *
 * \param castlings Part of a FEN pointing to the third part of FEN
 *
 * \see https://www.chessprogramming.org/Forsyth-Edwards_Notation
 */
static bool init_castlings(Position *pos, char *castlings)
{
	if(pos == NULL || castlings == NULL)
		return false;

	if(strlen(castlings) > 4)
		return false;

	pos->state->castling = NO_CASTLING;

	if(castlings[0] == '-')
		return true;

	for(size_t i = 0; i < strlen(castlings); i++) {
		switch(castlings[i]) {
			case 'k':
				pos->state->castling |= BLACK_OO;
				break;
			case 'q':
				pos->state->castling |= BLACK_OOO;
				break;
			case 'K':
				pos->state->castling |= WHITE_OO;
				break;
			case 'Q':
				pos->state->castling |= WHITE_OOO;
				break;
			default:
				return false;
		}
	}

	return true;
}

/**
 * \brief Initializes the fourth part of FEN. Returns True if everything went
 * well, false otherwise.
 *
 * \param position
 *
 * \param en_passant Part of a FEN string pointing to the fouth part of FEN
 *
 * \see https://www.chessprogramming.org/Forsyth-Edwards_Notation
 */
static bool init_en_passant(Position *pos, char *en_passant)
{
	if(pos == NULL || en_passant == NULL)
		return false;

	if(en_passant[0] != '-') {
		if(strlen(en_passant) != 2)
			return false;

		if(!isdigit(en_passant[1]) || !islower(en_passant[0]))
			return false;

		Square target = (
			(en_passant[1] - '1') * 8 + en_passant[0] - 'a'
		);

		Color color = pos->state->previous_move.color;

		pos->state->previous_move = (Move) {
			.destination = target - 8 + (16 * !color),
			.source = SQ_NONE,
			.move_type = EN_PASSANT,
			.moved_piece_type = PAWN,
		};
	}

	return true;
}

/**
 * \brief Checks if the board is correct. A board is considered illegal if the
 * pawns on it are on the first/last rank, or the number of the king on each
 * side is not equal to zero.
 *
 * \param position
 */
static bool is_board_correct(const Position *position)
{
	if(position == NULL)
		return false;

	const Board *board = &position->board;

	if(
		board->WhiteKing == EMPTY
		|| board->BlackKing == EMPTY
	)
		return false;

	U64 white_king = board->WhiteKing;
	U64 black_king = board->BlackKing;

	remove_lsb(white_king);
	remove_lsb(black_king);

	if(
		white_king != EMPTY
		|| black_king != EMPTY
	)
		return false;

	if(
		board->WhitePawns & (RANK_1 | RANK_8)
		|| board->BlackPawns & (RANK_1 | RANK_8)
	)
		return false;

	return true;
}

Position* init_position(const char *fen)
{
	if(fen == NULL)
		return NULL;

	char fen_tmp[strlen(fen) + 1];
	strcpy(fen_tmp, fen);

	char *board = strtok(fen_tmp, " ");
	char *turn = strtok(NULL, " ");
	char *castlings = strtok(NULL, " ");
	char *en_passant = strtok(NULL, " ");

	Position *position = calloc(1, sizeof(Position));
	position->state = calloc(1, sizeof(PositionState));

	PositionState *state = position->state;

	Color color;

	if(
		!init_board(position, board)
		|| !init_turn(position, turn)
		|| !init_castlings(position, castlings)
		|| !init_en_passant(position, en_passant)
		|| !is_board_correct(position)
	)
		goto err;

	color = !state->previous_move.color;

	state->captured_piece = NO_PIECE;
	state->move_50_rule = 0;

	state->allies = EMPTY;
	state->enemies = EMPTY;

	for(PieceType pt = PAWN; pt <= PIECE_TYPE_NB; pt++) {
		state->allies |= pieces(position, make_piece(!color, pt));
		state->enemies |= pieces(position, make_piece(color, pt));
	}

	state->occupied = state->allies | state->enemies;

	return position;

err:
	free(position->state);
	free(position);

	return NULL;
}

U64 pieces(const Position *pos, Piece piece)
{
	assert(pos != NULL);

	return pos->board.pieces[
		color_of_piece(piece) * 6 + type_of_piece(piece) - 1
	];
}

void set_piece(Position *pos, Piece piece, Square target)
{
	assert(pos != NULL);
	assert(piece != NO_PIECE);
	assert(target < SQ_NB);

	const Color color = color_of_piece(piece);
	const PieceType piece_type = type_of_piece(piece);

	pos->board.pieces[
		(color * 6) + piece_type - 1
	] |= square_to_bitboard(target);
}

void remove_piece(Position *pos, Piece piece, Square target)
{
	assert(pos != NULL);
	assert(piece != NO_PIECE);
	assert(target < SQ_NB);

	pos->board.pieces[
		((color_of_piece(piece)) * 6) + type_of_piece(piece) - 1
	] &= ~(square_to_bitboard(target));
}

void move_piece(Position *pos, Piece piece, Square source, Square destination)
{
	assert(pos != NULL);
	assert(piece != NO_PIECE);
	assert(source < SQ_NB);
	assert(destination < SQ_NB);

	remove_piece(pos, piece, source);
	set_piece(pos, piece, destination);
}

Piece piece_on(const Position *pos, Square target)
{
	assert(pos != NULL);
	assert(target < SQ_NB);

	const U64 bb = square_to_bitboard(target);

	return (
		!!(bb & ~pos->state->occupied)		* NO_PIECE
		| !!(pieces(pos, W_PAWN  ) & bb)	* W_PAWN
		| !!(pieces(pos, W_KNIGHT) & bb)	* W_KNIGHT
		| !!(pieces(pos, W_BISHOP) & bb)	* W_BISHOP
		| !!(pieces(pos, W_ROOK  ) & bb)	* W_ROOK
		| !!(pieces(pos, W_QUEEN ) & bb)	* W_QUEEN
		| !!(pieces(pos, W_KING  ) & bb)	* W_KING
		| !!(pieces(pos, B_PAWN  ) & bb)	* B_PAWN
		| !!(pieces(pos, B_KNIGHT) & bb)	* B_KNIGHT
		| !!(pieces(pos, B_BISHOP) & bb)	* B_BISHOP
		| !!(pieces(pos, B_ROOK  ) & bb)	* B_ROOK
		| !!(pieces(pos, B_QUEEN ) & bb)	* B_QUEEN
		| !!(pieces(pos, B_KING  ) & bb)	* B_KING
	);
}

U64 attacked_by(const Position *pos, Square target, Color attackers_color)
{
	assert(pos != NULL);
	assert(attackers_color < COLOR_NB);
	assert(target < SQ_NB);

	const U64 occupied = pos->state->occupied;

	const U64 pawns = pieces(pos, make_piece(attackers_color, PAWN));
	const U64 knights = pieces(pos, make_piece(attackers_color, KNIGHT));
	const U64 king = pieces(pos, make_piece(attackers_color, KING));

	const U64 queens = pieces(pos, make_piece(attackers_color, QUEEN));
	const U64 rooks = pieces(
		pos, make_piece(attackers_color, ROOK)
	) | queens;
	const U64 bishops = pieces(
		pos, make_piece(attackers_color, BISHOP)
	) | queens;

	return (
		(pawn_attack_pattern[!attackers_color](target)	& pawns)
		| (knight_move_pattern(target)			& knights)
		| (bishop_attacks_mask(target, occupied)	& bishops)
		| (rook_attacks_mask(target, occupied)		& rooks)
		| (king_move_pattern(target)			& king)
	);
}

void do_castling(Position *pos, Castling castling)
{
	assert(pos != NULL);
	assert(
		castling == WHITE_OO || castling == WHITE_OOO
		|| castling == BLACK_OO || castling == BLACK_OOO
	);

	const Color color = !!(castling & (ALL_BLACK));

	Square king_src = SQ_E1 + (56 * color);
	Square rook_src;

	Square king_dst = king_src + 2;
	Square rook_dst = SQ_F1 + (56 * color);

	if(castling & QUEEN_SIDE) {
		king_dst -= 4;
		rook_dst -= 2;

		rook_src = SQ_A1 + (56 * color);
	} else {
		rook_src = SQ_H1 + (56 * color);
	};

	move_piece(pos, make_piece(color, KING), king_src, king_dst);
	move_piece(pos, make_piece(color, ROOK), rook_src, rook_dst);
}

void do_move(Position *pos, Move move)
{
	assert(pos != NULL);
	assert(move.destination != SQ_NONE && move.source != SQ_NONE);
	assert(move.moved_piece_type != NO_PIECE_TYPE);
	assert(pos->state->move_50_rule != 50);

	PositionState *state = malloc(sizeof(PositionState));

	assert(state != NULL);

	Color color = move.color;

	state->captured_piece = (
		move.move_type == EN_PASSANT
		? make_piece(!color, PAWN) : piece_on(pos, move.destination)
	);

	state->move_50_rule = pos->state->move_50_rule + 1;

	if(state->captured_piece) {
		remove_piece(
			pos,
			state->captured_piece,
			move.destination
		);

		state->move_50_rule = 0;
	}

	if(move.move_type == CASTLING) {
		Castling castling = WHITE_OO << (2 * color);

		do_castling(
			pos,
			castling << !!(
				square_to_bitboard(move.destination) & FILE_C
			)
		);
	} else {
		move_piece(
			pos,
			make_piece(color, move.moved_piece_type),
			move.source,
			move.destination
		);
	}

	if(move.moved_piece_type == PAWN) {
		if(move.move_type == EN_PASSANT) {
			remove_piece(
				pos,
				make_piece(!color, PAWN),
				move.destination - 8 + (color * 16)
			);
		} else if(move.move_type == PROMOTION) {
			set_piece(
				pos,
				make_piece(
					color,
					move.promotion_piece_type
				),
				move.destination
			);

			remove_piece(
				pos,
				make_piece(color, PAWN),
				move.destination
			);
		}

		state->move_50_rule = 0;
	}

	state->previous_move = move;
	state->previous_state = pos->state;

	state->castling = pos->state->castling;
	state->castling &= ~(
		castling_masks[move.source] | castling_masks[move.destination]
	);

	pos->state = state;

	state->allies = EMPTY;
	state->enemies = EMPTY;

	for(PieceType pt = PAWN; pt <= PIECE_TYPE_NB; pt++) {
		state->allies |= pieces(pos, make_piece(color, pt));
		state->enemies |= pieces(pos, make_piece(!color, pt));
	}

	state->occupied = state->allies | state->enemies;
}

void undo_move(Position *pos)
{
	assert(pos != NULL);

	Move last_move = pos->state->previous_move;

	Color allies_color = last_move.color;

	Square source = last_move.source;
	Square destination = last_move.destination;

	Piece piece = make_piece(allies_color, last_move.moved_piece_type);
	Piece captured = pos->state->captured_piece;

	if (last_move.move_type == CASTLING) {
		bool king_side = destination > source;

		Square rook_destination = destination + 1 - 2 * king_side;

		move_piece(
			pos,
			make_piece(allies_color, KING),
			destination,
			source
		);

		move_piece(
			pos,
			make_piece(allies_color, ROOK),
			rook_destination,
			rook_destination + 2 - 5 * !king_side
		);

		remove_piece(
			pos,
			make_piece(allies_color, KING),
			destination
		);

		remove_piece(
			pos,
			make_piece(allies_color, ROOK),
			rook_destination
		);
	}

	if (last_move.move_type == COMMON) {
		move_piece(pos, piece, destination, source);
		if (captured) {
			set_piece(pos, captured, destination);
		}
	}

	if (last_move.move_type == EN_PASSANT) {
		move_piece(pos, piece, destination, source);
		set_piece(pos, captured, destination - 8 + (16 * allies_color));
	}

	if (last_move.move_type == PROMOTION) {
		set_piece(pos, piece, source);
		remove_piece(
			pos,
			make_piece(allies_color, last_move.promotion_piece_type),
			destination
		);
		if (captured) {
			set_piece(pos, piece, destination);
		}
	}

	PositionState *temp = pos->state;
	pos->state = pos->state->previous_state;
	free(temp);
}

