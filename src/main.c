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

	Position *pos = get_position(
		"position fen 5r2/k5P1/8/8/8/8/8/2K5 w - - 0 1 moves g7f8q"
	);

	free(pos->state);
	free(pos);

	return 0;
}
