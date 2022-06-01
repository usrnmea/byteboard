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
 * \brief Evaluates the position on the material on both sides
 *
 * \param position
 *
 * \brief value
 */
Evaluation evaluate_material(const Position *position);

#endif
