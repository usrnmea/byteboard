#include "position.h"
#include "bitboard_mapping.h"
#include "bitboard.h"
#include "piece.h"
#include "patterns.h"
#include "uci.h"
#include "search.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static char piece_symbol[PIECE_TYPE_NB + 1] = {
	' ', 'p', 'n', 'b', 'r', 'q', 'k'
};

Move str_to_move(Position *pos, char *str)
{
	assert(pos != NULL);
	assert(str != NULL);
	assert(strlen(str) == 4 || strlen(str) == 5);

	size_t size = strlen(str);

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

		if(pt < KNIGHT || pt > QUEEN)
			str[4] = '\0';
		else
			str[4] = piece_symbol[pt];

	} else {
		str[4] = '\0';
	}
}

Position *get_position(char *str)
{
	assert(strlen(str) > 8);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';

	str += 9;

	char *curr_char = str;
	Position *pos = NULL;

	if (strncmp("startpos", str, 8) == 0) {
		pos = init_position(STARTPOS);
	}

	else {
		curr_char = strstr(str, "fen");

		if (curr_char != NULL) {
			// Now pointer on the fen
			curr_char += 4;

			pos = init_position(curr_char);
		}

		else
			pos = init_position(STARTPOS);
	}

	curr_char = strstr(str, "moves");

	if (curr_char != NULL) {
		curr_char += 6;

	        while (*curr_char) {
			Move move;

			if (curr_char[4] == ' ' || strlen(curr_char) == 4) {
				char command[5] = {
					curr_char[0], curr_char[1],
					curr_char[2], curr_char[3],
					'\0'
				};

				move = str_to_move(pos, command);
				do_move(pos, move);

				curr_char += 4;

				if (curr_char[0] == ' ')
					curr_char += 1;

			}

			else {
				char command[6] = {
					curr_char[0], curr_char[1],
					curr_char[2], curr_char[3],
					curr_char[4], '\0'
				};

				move = str_to_move(pos, command);
				do_move(pos, move);

				curr_char += 5;

				if (curr_char[0] == ' ')
					curr_char += 1;
			}
	        }
	}

	return pos;
}

ExtMove get_go(Position *pos, char *str)
{
	assert(pos != NULL);
	assert(strlen(str) > 2);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';

	uint32_t depth = 7;

	char *curr_depth = NULL;

	if (strstr(str, "depth")) {
		curr_depth = strstr(str, "depth");
		depth = atoi(curr_depth + 6);
	}

	ExtMove best_move = find_best(pos, depth);

	return best_move;
}

void uci_loop()
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char input[2000];

	Position *pos = init_position(STARTPOS);
	ExtMove best_move;

	while (1) {
		memset(input, 0, sizeof(input));
		fflush(stdout);

		if (!fgets(input, 2000, stdin))
			continue;

		else if (input[0] == '\n')
			continue;

		else if (strncmp(input, "isready", 7) == 0) {
			printf("readyok\n");
			continue;
		}

		else if (strncmp(input, "position", 8) == 0) {
			pos = get_position(input);
		}

		else if (strncmp(input, "ucinewgame", 10) == 0) {
			pos = get_position(STARTPOS);
		}

		else if (strncmp(input, "go", 2) == 0) {
			best_move = get_go(pos, input);

			char move[6] = ".....";
			move_to_str(best_move.move, move);

			printf("bestmove %s\n", move);
		}

		else if (strncmp(input, "quit", 4) == 0) {
			break;
		}

		else if (strncmp(input, "uci", 3) == 0) {
			printf("id name byteboard\n");
			printf("id author Shark && Duck\n");
			printf("uciok\n");
		}
	}

	free(pos->state);
	free(pos);
}
