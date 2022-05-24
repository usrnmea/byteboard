/**
 * \file
 */
#ifndef __RAYS_H__
#define __RAYS_H__

#include "bitboard.h"
#include "bitboard_mapping.h"

extern U64 ray_between[SQ_NB][SQ_NB];	/*!< Calculated rays from
					get_ray_between() for each pair of squares
					(if possible)
					(not including squares)*/

extern U64 ray_horizontal[SQ_NB];	/*!< Calculated horizontal rays for
					each square(include square)*/
extern U64 ray_vertical[SQ_NB];	/*!< Calculated vertical rays from for
				each square(include square)*/

extern U64 ray_west[SQ_NB];	/*!< Calculated rays from get_ray_west() for
				each square (not including square)*/
extern U64 ray_south[SQ_NB];	/*!< Calculated rays from get_ray_south() for
				each square (not including square)*/
extern U64 ray_east[SQ_NB];	/*!< Calculated rays from get_ray_east() for
				each square (not including square)*/
extern U64 ray_north[SQ_NB];	/*!< Calculated rays from get_ray_north() for
				each square (not including square)*/

extern U64 ray_diagonal[SQ_NB];		/*!< Calculated rays from
					get_ray_diagonal() for each square
					(include square)*/
extern U64 ray_anti_diagonal[SQ_NB];	/*!< Calculated rays from
					get_ray_anti_diagonal() for
					each square (include square)*/

extern U64 ray_north_west[SQ_NB];	/*!< Calculated rays from
					get_ray_north_west() for each square
					(not including square)*/
extern U64 ray_north_east[SQ_NB];	/*!< Calculated rays from
					get_ray_north_east() for each square
					(not including square)*/
extern U64 ray_south_west[SQ_NB];	/*!< Calculated rays from
					get_ray_south_west() for each square
					(not including square)*/
extern U64 ray_south_east[SQ_NB];	/*!< Calculated rays from
					get_ray_south_east() for each square
					(not including square)*/

/**
 * \brief Calculates values for #ray_between #ray_west #ray_south #ray_east
 * #ray_north #ray_north_west #ray_north_east #ray_south_west #ray_south_west
 * #ray_horizontal #ray_vertical #ray_diagonal #ray_anti_diagonal
 */
void init_rays(void);

/**
 * \brief  A function that generates a ray between two given squares.
 * Squares must be on the same diagonal/vertical/horizontal.
 *
 * \param from first target square
 *
 * \param to second target square
 *
 * \return A bitboard containing a ray between two squares.
 *
 * \see https://www.chessprogramming.org/Rays
 */
U64 get_ray_between(Square from, Square to);

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
