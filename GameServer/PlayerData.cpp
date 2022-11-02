#include "PlayerData.h"

DWORD Title::GetLevelValue(BYTE slot, BYTE level) {
	BYTE x = 0;

	x = (slot * 4) + (level - 1);

	if (level > 1) {
		return (1 << x) + this->GetLevelValue(slot, level-1);
	}
	else {
		return (1 << x);
	}
}