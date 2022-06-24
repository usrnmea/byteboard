/**
 * \file uci.h
 */
#ifndef __UCI_H__
#define __UCI_H__

#include "position.h"

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

#endif
