/**
 * \file uci.h
 */
#ifndef __UCI_H__
#define __UCI_H__

#include "position.h"

/**
 * \brief Converts a given string to a move.
 *
 * \param str string
 *
 * \return Converted move #Move
 */
Move str_to_move(char *str);

/**
 * \brief Converts a given move to a string.
 *
 * \param move #Move
 *
 * \return Converted C string
 */
char *move_to_str(Move move);

#endif
