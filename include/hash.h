/**
 * \file
 */
#ifndef __HASH_H__
#define __HASH_H__

#include "position.h"
#include "bitboard.h"

/// Random side key
extern U64 side_key;

/// Unique position hash key
extern U64 hash_key;

/// Random piece keys
U64 piece_keys[12][64];

/// Random en passant keys
U64 en_passant_keys[64];

/// Random castling keys
U64 castling_keys[16];

/**
 * \brief Generates random u32 number
 *
 * \return random u32 number
 */
uint32_t get_random_U32_number(void);

/**
 * \brief Generates random u64 number
 *
 * \return random u64 number
 */
U64 get_random_U64_number(void);

/**
 * \brief Initializes all hash keys with random numbers
 */
void init_hash_keys(void);

/**
 * \brief Generates unique hash key for the position
 *
 * \param pos current position
 *
 * \return #U64 hash key
 */
U64 generate_hash_key(const Position *pos);

#endif
