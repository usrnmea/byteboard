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

void add_common_moves(
	MoveList *move_list,
	const Piece piece,
	const Square source,
	U64 destinations
)
{
	assert(move_list != NULL);
	assert(source < SQ_NB);
	assert(piece != NO_PIECE);

	Move move = {
		.move_type = COMMON, .moved_piece_type = type_of_piece(piece),
		.promotion_piece_type = NO_PIECE_TYPE, .color = color_of_piece(piece),
		.source = source
	};

	ExtMove ext_move = {.move = move, .eval = NO_EVAL};

	while (destinations) {
		ext_move.move.destination = bit_scan_forward(destinations);

		ml_add(move_list, ext_move);

		remove_lsb(destinations);
	}
}

void add_castlings(
	MoveList *move_list,
	const Square source,
	U64 destinations
)
{
	assert(move_list != NULL);
	assert(source < SQ_NB);

	Move move = {
		.move_type = CASTLING, .moved_piece_type = KING,
		.promotion_piece_type = NO_PIECE_TYPE,
		.color = source == SQ_E8, .source = source
	};

	ExtMove ext_move = {.move = move, .eval = NO_EVAL};

	while (destinations) {
		ext_move.move.destination = bit_scan_forward(destinations);

		ml_add(move_list, ext_move);

		remove_lsb(destinations);
	}
}
