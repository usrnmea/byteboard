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

	WHITE_WIN = 100000,	///< Checkmate black
	BLACK_WIN = -100000,	///< Checkmate white
} Evaluation;

/// Extended move structure containing, in addition to the move,
/// its evaluation.
typedef struct ExtMove {
	Move move;		///< Move
	Evaluation eval;	///< Evaluation(Use #NO_EVAL by default)
} ExtMove;

/// Game phases. Depending on the phase of the game, one or another score can
/// take on very different meanings
typedef enum GamePhase {
	MIDDLEGAME,
	ENDGAME,
	GAME_PHASES_NB,
} GamePhase;

/// Returns the value of the #PieceType
extern Evaluation piece_type_value[GAME_PHASES_NB][PIECE_TYPE_NB];

/**
 * \brief returns phase value for tapered evaluation
 *
 * \param position position
 *
 * \return phase
 *
 * \see https://www.chessprogramming.org/Tapered_Eval
 */
int32_t get_phase(const Position *position);

/**
 * \brief Main function for position evaluation.
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_position(const Position *position);

/**
 * \brief Main function for midgame evaluation.
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_midgame(const Position *pos);

/**
 * \brief Main function for endgame evaluation.
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_endgame(const Position *pos);

/**
 * \brief Evaluates the position on the material on both sides
 *
 * \param position position
 *
 * \param gp game phase
 *
 * \brief value
 */
Evaluation evaluate_material(const Position *position, GamePhase gp);

/**
 * \brief Evaluates pieces mobility
 *
 * \param position position
 *
 * \param gp game phase
 *
 * \brief value
 */
Evaluation evaluate_mobility(const Position *position, GamePhase gp);

/**
 * \brief Evaluates pawns in the four central squares
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_central_pawns(const Position *position);

/**
 * \brief Evaluates passed pawns
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_passed_pawns(const Position *pos);

/**
 * \brief Evaluates doubled pawns
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation evaluate_doubled_pawns(const Position *position);

/**
 * \brief Evaluates king position
 *
 * \param position position
 *
 * \param game_phase game phase
 *
 * \return evaluation
 */
Evaluation evaluate_king_position(
	const Position *position, GamePhase game_phase
);

/**
 * \brief Evaluates turn
 *
 * \param position position
 *
 * \return evaluation
 */
Evaluation tempo(const Position *position);

#endif
