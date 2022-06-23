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

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	init_rays();

	Position *pos = init_position(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	);

	printf(
		"Hello, byteboard!\nNodes in the start position: %lu",
		perft_test(pos, 2)
>>>>>>> 34a0ee4cd72fc40be2c87c569a3bb61e96d2bdd7
	);

	free(pos->state);
	free(pos);

	return 0;
}
