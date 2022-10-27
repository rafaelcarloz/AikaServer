#include "MiscData.h"
#include <math.h>

#pragma region "Position"

Position::Position(float x, float y) {
	this->X = x;
	this->Y = y;
}

WORD Position::Distance(Position pos) {
	Position dif;

	dif.X = this->X - (pos.X);
	dif.Y = this->Y - (pos.Y);

	return round(sqrt((dif.X * dif.X) + (dif.Y * dif.Y)));
}

WORD Position::Distance(Position pos1, Position pos2) {
	Position dif;

	dif.X = pos1.X - (pos2.X);
	dif.Y = pos1.Y - (pos2.Y);

	return round(sqrt((dif.X * dif.X) + (dif.Y * dif.Y)));
}

#pragma endregion