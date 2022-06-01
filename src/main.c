#include <stdio.h>
#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "rays.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"

int main(void)
{
	init_rays();

	printf("Hello, byteboard!\n");

	return 0;
}
