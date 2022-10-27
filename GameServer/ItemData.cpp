#include <cmath>

#include "ItemData.h"
#include "ItemList.h"

#pragma region "Items Data"

BYTE Item::GetReinforce() {
	BYTE Reinforce = trunc((BYTE)this->Amount / 16);
	return Reinforce;
}

bool Item::CanAgroup() {
	ItemList* itemList = ItemList::GetInstance();

	return itemList->Get(this->Index).CanAgroup;
}

#pragma endregion