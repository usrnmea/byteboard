#include <stdio.h>
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "movegen.h"

int main(void)
{
	init_rays();
	Position *pos_1 = init_position("r3k2r/2q5/8/8/8/B6B/PP1PP2P/R3K1QR w KQkq - 0 1");
	possible_castlings(pos_1, BLACK, SQ_E8);
	free(pos_1->state);
	free(pos_1);

	printf("Hello, byteboard!\n");

	return 0;
}
