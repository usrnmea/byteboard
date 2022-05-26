#include "bitboard.h"
#include "bitboard_mapping.h"
#include "piece.h"
#include "position.h"

#include <stdlib.h>

Position* init_position(const char *fen)
{
	return NULL;
}

U64 attacked_by(const Position *pos, Square target, Color attackers_color)
{
	return EMPTY;
}

U64 pieces(const Position *pos, PieceType piece_type, Color color)
{
	return EMPTY;
}

Piece piece_on(const Position *pos, Square target)
{
	return NO_PIECE;
}

void move_piece(Position *pos, Piece piece, Square source, Square destination)
{
}

void set_piece(Position *pos, Piece piece, Square target)
{
}

void remove_piece(Position *pos, Piece piece, Square target)
{
}

void do_castling(Position *pos, Castling castling)
{
}

void do_move(Position *pos, Move move)
{
}

void retract_move(Position *pos)
{
}

