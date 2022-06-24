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

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	init_rays();

	Position *pos = init_position(
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"
	);

	printf(
		"Hello, byteboard!\nNodes in the start position: %lu",
		perft_test(pos, 3)
	);

	free(pos->state);
	free(pos);

	return 0;
}
