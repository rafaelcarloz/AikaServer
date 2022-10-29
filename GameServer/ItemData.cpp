#include <cmath>

#include "ItemData.h"
#include "ItemList.h"

#pragma region "Items Data"

bool Item::CanAgroup() {
	ItemList* itemList = ItemList::GetInstance();

	return itemList->Get(this->Index).CanAgroup;
}

bool Item::CanEquip() {
	ItemList* itemList = ItemList::GetInstance();

	int equipSlot = itemList->Get(this->Index).GetEquipSlot();

	return (equipSlot != 0);
}


BYTE Item::GetReinforce() {
	BYTE Reinforce = trunc((BYTE)this->Amount / 16);
	return Reinforce;
}

BYTE Item::GetEquipSlot() {
	ItemList* itemList = ItemList::GetInstance();

	return itemList->Get(this->Index).GetEquipSlot();
}


time_t Item::GetExpireDate() {
	DWORD expireTime = 0;

	memcpy(&expireTime, &this->Amount, sizeof(DWORD));

	return (time_t)expireTime;
}

void Item::SetExpireDate(time_t date) {
	memcpy(&this->Amount, &date, sizeof(DWORD));
}

bool Item::IsSealed() {
	return (this->GetExpireDate() == 0);
}

void Item::SetSealed(bool seal) {
	if (seal) {
		ZeroMemory(&this->Amount, sizeof(DWORD));
	}
	else {
		this->SetExpireDate(time(0) + (ItemList::Get(this->Index).Duration * HOUR_TO_SEC_MULTIPLYER));
	}
}

DWORD Item::GetEquipmentSellPrice() {
	return (DWORD)(ItemList::Get(this->Index).SellPrince * (this->MinDurability / this->MaxDurability));
}

#pragma endregion