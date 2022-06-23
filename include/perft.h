/**
 * \file perft.h
 */
#ifndef __PERFT_H__
#define __PERFT_H__

#include "bitboard.h"

/**
 * \brief Debugging function to walk the move generation tree of strictly legal moves
 * to count all the leaf nodes of a certain depth
 *
 * \param depth given depth
 *
 * \return number of generated nodes
 */
U64 perft(int depth);

#endif
