#include "PlayerData.h"

DWORD Title::GetLevelValue(BYTE slot) {
	BYTE x = 0;

	x = (slot * 4) + (Level - 1);

	if (Level > 1) {
		return (1 << x) + this->GetLevelValue(slot);
	}
	else {
		return (1 << x);
	}
}