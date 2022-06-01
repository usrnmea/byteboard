#include "evaluate.h"

Evaluation piece_type_value[PIECE_TYPE_NB] = {
	10, 35, 35, 52, 100, 0
};

Evaluation evaluate_material(const Position *pos)
{
	return DRAW;
}
