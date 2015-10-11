#include "Facing.h"

int Facing::STEP_X[6] = {0, 0, 0, 0, -1, 1};
int Facing::STEP_Y[6] = {-1, 1, 0, 0, 0, 0};
int Facing::STEP_Z[6] = {0, 0, -1, 1, 0, 0};
int Facing::OPPOSITE_FACING[6] = {1, 0, 3, 2, 5, 4};
int Facing::ROTATE_OPPOSITE[4] = {2, 3, 0, 1};