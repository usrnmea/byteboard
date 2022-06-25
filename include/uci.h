/**
 * \file uci.h
 */
#ifndef __UCI_H__
#define __UCI_H__

#include "position.h"
#include "evaluate.h"

/// Start position macro
#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/**
 * \brief Converts a given string to a move.
 *
 * \param pos current position
 *
 * \param str string
 *
 * \return Converted move #Move
 */
Move str_to_move(Position *pos, char *str);

/**
 * \brief Converts a given move to a string.
 *
 * \param move #Move
 *
 * \param str out parameter, pointer to the converted string
 */
void move_to_str(Move move, char *str);

/**
 * \brief Converts a given UCI string to the position.
 *
 * \param str string in the UCI format
 *
 * \return pointer to the #Position
 */
Position *get_position(char *str);

/**
 * \brief Starts calculating the best move at the given position within the
 * given depth.
 *
 * \param pos current position
 *
 * \param str string in the UCI format
 *
 * \return best move #ExtMove
 */
ExtMove get_go(Position *pos, char *str);

#endif
