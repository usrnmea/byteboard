#include "position.h"
#include "bitboard_mapping.h"
#include "bitboard.h"
#include "piece.h"
#include "patterns.h"

#include <assert.h>
#include <string.h>

Move str_to_move(Position *pos, char *str)
{
	assert(pos != NULL);
	assert(str != NULL);
	assert(strlen(str) == 4 || strlen(str) == 5);

	size_t size = strlen(str);

	assert(size == 4 || size == 5);

	Square source_sq = (str[0] - 'a') + (str[1] - '1') * 8;
	Square target_sq = (str[2] - 'a') + (str[3] - '1') * 8;

	Piece piece = piece_on(pos, source_sq);
	Color color = color_of_piece(piece);

	Move move;

	move.source = source_sq;
	move.destination = target_sq;
	move.color = color;
	move.moved_piece_type = type_of_piece(piece);

	if (size == 4) {
		Move last_move = pos->state->previous_move;

		Square dst = last_move.destination;
		Square src = last_move.source;

		U64 target_bb = square_to_bitboard(target_sq);
		U64 king_moves = king_move_pattern(source_sq);

		uint32_t is_castling = !(king_moves & target_bb);

		if (type_of_piece(piece) == KING && is_castling) {
			move.move_type = CASTLING;
		}

		else if (
			last_move.moved_piece_type == PAWN
			&& type_of_piece(piece) == PAWN
			&& (dst > src ? dst - src : src - dst) == 16
			&& target_sq == (dst + 8 - (16 * color))
		)
		{
			move.move_type = EN_PASSANT;
		}

		else {
			move.move_type = COMMON;
		}

		move.promotion_piece_type = NO_PIECE_TYPE;
	}

	else if (size == 5) {
		move.move_type = PROMOTION;

		PieceType promotion_pt = NO_PIECE_TYPE;

		if (str[4] == 'n')
			promotion_pt = KNIGHT;
		else if (str[4] == 'b')
			promotion_pt = BISHOP;
		else if (str[4] == 'r')
			promotion_pt = ROOK;
		else if (str[4] == 'q')
			promotion_pt = QUEEN;

		move.promotion_piece_type = promotion_pt;
	}

	return move;
}

void move_to_str(Move move, char *str)
{
	assert(move.moved_piece_type != NO_PIECE_TYPE);
	assert(move.color < COLOR_NB);
	assert(move.destination < SQ_NB);
	assert(move.source < SQ_NB);

	assert(str != NULL);

	Square source = move.source;
	Square destination = move.destination;

	char source_rank = '1' + source / 8;
	char destination_rank = '1' + destination / 8;

	char source_file = 'a' + source - source / 8 * 8;
	char destination_file = 'a' + destination - destination / 8 * 8;

	str[0] = source_file;
	str[1] = source_rank;
	str[2] = destination_file;
	str[3] = destination_rank;

	if (move.move_type == PROMOTION) {
		PieceType pt = move.promotion_piece_type;

		if (pt == KNIGHT)
			str[4] = 'n';
		else if (pt == QUEEN)
			str[4] = 'q';
		else if (pt == BISHOP)
			str[4] = 'b';
		else if (pt == ROOK)
			str[4] = 'r';
	}
}
