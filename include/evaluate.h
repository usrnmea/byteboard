/**
 * \file
 */
#ifndef __EVALUATE_H__
#define __EVALUATE_H__

#include "bitboard.h"
#include "position.h"

/// Position evaluation.
typedef enum Evaluation {
	NO_EVAL = 0,		///< No value
	DRAW = 0,		///< Draw

	WHITE_WIN = 10000,	///< Checkmate black
	BLACK_WIN = -10000,	///< Checkmate white
} Evaluation;

/// Extended move structure containing, in addition to the move,
/// its evaluation.
typedef struct ExtMove {
	Move move;		///< Move
	Evaluation eval;	///< Evaluation(Use #NO_EVAL by default)
} ExtMove;

/// Returns the value of the #PieceType
extern Evaluation piece_type_value[PIECE_TYPE_NB];

/**
 * \brief Main function for position evaluation.
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_position(const Position *position);

/**
 * \brief Evaluates the position on the material on both sides
 *
 * \param position position
 *
 * \brief value
 */
Evaluation evaluate_material(const Position *position);

/**
 * \brieft Evaluates pawns in the four central squares
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_central_pawns(const Position *position);

#endif
