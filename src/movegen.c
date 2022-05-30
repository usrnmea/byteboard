#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "patterns.h"
#include "masks.h"
#include "position.h"
#include "evaluate.h"
#include "movegen.h"

#include <stdlib.h>
#include <assert.h>

/*
Initializes the list of moves.
The creation of the list of moves should be
done only through this function
*/
MoveList *init_move_list(void)
{
    MoveList *move_list = malloc(sizeof(MoveList));
    assert(move_list != NULL);

    move_list->last = move_list->move_list;

    return move_list;
}
