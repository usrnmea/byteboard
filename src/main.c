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
#include "hash.h"
#include "search.h"
#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	init_hash_keys();

	init_rays();
	uci_loop();

	return 0;
}
