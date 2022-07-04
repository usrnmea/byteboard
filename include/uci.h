/**
 * \file uci.h
 */
#ifndef __UCI_H__
#define __UCI_H__

#include "position.h"
#include "evaluate.h"

/// Start position macro
#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/// Structure with fields for time control
typedef struct TimeInfo {
	uint8_t quit;
	uint8_t time_set;
	uint8_t stopped;

	int moves_to_go;
	int inc;

	int start_time;
	int stop_time;

	int move_time;
	int time_uci;
} TimeInfo;

/// Structure for time control
extern TimeInfo time_info;

/**
 * \brief Gets current time in milliseconds
 *
 * \return time in ms
 */
int get_time_ms(void);

/**
 * \brief Waits for user input\GUI input in STDIN
 *
 * \return 1 or 0, depends on input
 */
int input_waiting(void);

/**
 * \brief Reads user\GUI input from STDIN
 */
void read_input(void);

/**
 * \brief Bridge function to interact between search and user\GUI input
 */
void communicate(void);

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

/**
 * \brief Converts a given UCI string to the position.
 *
 * \param str string in the UCI format
 *
 * \return pointer to the #Position
 */
Position *get_position(char *str);

/**
 * \brief Starts calculating the best move at the given position within the
 * given depth.
 *
 * \param pos current position
 *
 * \param str string in the UCI format
 *
 * \return best move #ExtMove
 */
ExtMove get_go(Position *pos, char *str);

/**
 * \brief Main UCI loop.
 */
void uci_loop();

#endif
