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

U64 perft(Position *pos, int depth)
{
	if (depth == 0) {
		return 1ULL;
	}

	U64 nodes = 0;
	MoveList *move_list = generate_all_moves(pos);

	for (int i = 0; i < ml_len(move_list); i++) {
		do_move(pos, move_list->move_list[i].move);
		nodes += perft(pos, depth - 1);
		undo_move(pos);
	}

	free(move_list);

	return nodes;
}
