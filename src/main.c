#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"
#include "perft.h"
#include "search.h"
#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	init_rays();

	Position *pos = init_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R3K2R w KQ - 0 10"
	);

	str_to_move(pos, "e1g1");

	free(pos->state);
	free(pos);

	return 0;
}
