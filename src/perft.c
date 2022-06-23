#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "perft.h"

#include <stdlib.h>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x80 ? '1' : '0')


void print_bitboard(U64 *bitboard)
{
    for(uint8_t counter = 0; counter != 8; counter++)
        printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(*((uint8_t *) bitboard + (7 - counter))));
} // Temporary

U64 perft(Position *pos, int depth)
{

	U64 nodes = 0;
	MoveList *move_list = generate_all_moves(pos);

	U64 n_moves = ml_len(move_list);

	if (depth == 1) {
		return n_moves;
	}

	for (int i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);
		nodes += perft(pos, depth - 1);
		undo_move(pos);
	}

	free(move_list);

	return nodes;
}

U64 perft_test(Position *pos, int depth)
{
	if (depth == 0) {
		return 1ULL;
	}

	U64 nodes = 0;
	MoveList *move_list = generate_all_moves(pos);

	for (int i = 7; i < ml_len(move_list); i++) {

		U64 cummulative_nodes = nodes;

		do_move(pos, move_list->move_list[i].move);
		nodes += perft(pos, depth - 1);
		undo_move(pos);

		U64 old_nodes = nodes - cummulative_nodes;

		// Print move

		Square source = move_list->move_list[i].move.source;
		Square destination = move_list->move_list[i].move.destination;

		char source_rank = '1' + source / 8;
		char destination_rank = '1' + destination / 8;

		char source_file = 'a' + source - source / 8 * 8;
		char destination_file = 'a' + destination - destination / 8 * 8;

		printf("    move: %c%c", source_file, source_rank);
		printf(
			"%c%c    nodes: %llu\n",
			destination_file,
			destination_rank,
			old_nodes
		);
	}

	printf("\n\n");

	free(move_list);

	return nodes;
}
