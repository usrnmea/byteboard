/**
 * \file
 */
#ifndef __RAYS_H__
#define __RAYS_H__

#include "bitboard.h"
#include "bitboard_mapping.h"

/**
 * \brief  A function that generates a ray between two given squares.
 *
 * \param first first target square
 *
 * \param second second target square
 *
 * \return A bitboard containing a west ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 in_between(Square first, Square second);

/**
 * \brief  A function that generates a west ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a west ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_west(Square square);

/**
 * \brief  A function that generates a south ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a south ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_south(Square square);

/**
 * \brief  A function that generates a east ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a east ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_east(Square square);

/**
 * \brief  A function that generates a north ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a north ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_north(Square square);

/**
 * \brief  A function that generates a diagonal from one given square, including
 * square.
 *
 * \param square target square
 *
 * \return A bitboard containing a diagonal.
 *
 * \see https://www.chessprogramming.org/Diagonals
 */
U64 get_ray_diagonal(Square square);

/**
 * \brief  A function that generates a anti-diagonal from one given square,
 * including square.
 *
 * \param square target square
 *
 * \return A bitboard containing a anti-diagonal.
 *
 * \see https://www.chessprogramming.org/Anti-Diagonals
 */
U64 get_ray_anti_diagonal(Square square);

/**
 * \brief  A function that generates a northeast ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a northeast ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_north_east(Square square);

/**
 * \brief  A function that generates a southwest ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a southwest ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_south_west(Square square);

/**
 * \brief  A function that generates a northwest ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a northwest ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_north_west(Square square);

/**
 * \brief  A function that generates a southeast ray from one given square.
 *
 * \param square target square
 *
 * \return A bitboard containing a southeast ray.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_south_east(Square square);
#endif
