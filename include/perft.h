/**
 * \file perft.h
 */
#ifndef __PERFT_H__
#define __PERFT_H__

#include "bitboard.h"
#include "position.h"

/**
 * \brief Debugging function to walk the move generation tree of strictly legal
 * moves to count all the leaf nodes of a certain depth
 *
 * \param pos current position
 *
 * \param depth given depth
 *
 * \return number of generated nodes
 */
U64 perft(Position *pos, int depth);

/**
 * \brief This function is the copy of perft function but prints some debug
 * information
 *
 * \param pos current position
 *
 * \param depth given depth
 *
 * \return number of generated nodes
 */
U64 perft_test(Position *pos, int depth);

#endif
