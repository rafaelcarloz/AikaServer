#include "StatusController.h"
#include "ReinforceData.h"
#include "ItemList.h"

#include "ServerInstance.h"
#include "Packets.h"
#include "SetItem.h"

using namespace packets;

void StatusController::Initialize(uint16_t entityId, PCharacter character, Character baseCharacter) {
	this->_character = character;
	this->_baseCharacter = baseCharacter;

	this->_entityId = entityId;

	this->Recalculate();
}

#pragma region Status

void StatusController::Recalculate() {
	this->RecalculateEffects();

	ZeroMemory(&this->volatileStatus, sizeof(CharacterStatus));
	ZeroMemory(&this->aditionalStatus, sizeof(CharacterAditionalStatus));
	
	this->volatileStatus = this->_baseCharacter.Status;

	this->CalculateStatusPoints();
	this->CalculateMoveSpeed();
	this->CalculateCritical();

	this->CalculateCriticalDamage();
	this->CalculateArmorPenetration();
	this->CalculateDouble();
	this->CalculatePvP();
	this->CalculateSkillHability();
	this->CalculateHealing();
	this->CalculateResistences();
	this->CalculateHithateAndDodge();
	this->CalculateCooldown();

	this->CalculateEquipmentDamage();
	this->CalculateDamage();

	this->CalculateEquipmentDefense();
	this->CalculateDefense();

	this->CalculateEquipmentsLifePoints();
	this->CalculateLifePoints();


	this->SendCurrentStatus();
}


void StatusController::CalculateMoveSpeed() {
	this->aditionalStatus.MoveSpeed += (40 + this->GetEffect(EF_RUNSPEED));
	this->aditionalStatus.MoveSpeed += this->GetEffect(EF_RELIQUE_RUNSPEED);
}

void StatusController::CalculateStatusPoints() {
	this->volatileStatus.Attributes.Constitution += (this->_character->Status.Attributes.Constitution +
		this->GetEffect(EF_CON));
	this->volatileStatus.Attributes.Dexterity += (this->_character->Status.Attributes.Dexterity +
		this->GetEffect(EF_DEX));
	this->volatileStatus.Attributes.Intelligence += (this->_character->Status.Attributes.Intelligence +
		this->GetEffect(EF_INT));
	this->volatileStatus.Attributes.Spirit += (this->_character->Status.Attributes.Spirit +
		this->GetEffect(EF_SPI));
	this->volatileStatus.Attributes.Strength += (this->_character->Status.Attributes.Strength +
		this->GetEffect(EF_STR));
}

void StatusController::CalculateCritical() {
	this->volatileStatus.CriticalRate += (UINT16)this->GetEffect(EF_CRITICAL);

	this->volatileStatus.CriticalRate += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.2f);
	this->volatileStatus.CriticalRate += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.2f);

	this->volatileStatus.CriticalRate += (UINT16)this->GetEffect(EF_RELIQUE_CRITICAL);
}

void StatusController::CalculateCriticalDamage() {
	this->aditionalStatus.CriticalDamage += (UINT16)this->GetEffect(EF_CRITICAL_POWER);

	this->aditionalStatus.CriticalDamage += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.3f);
	this->aditionalStatus.CriticalDamage += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.2f);
}

void StatusController::CalculateArmorPenetration() {
	this->aditionalStatus.ArmorPenetration += (UINT16)this->GetEffect(EF_PIERCING_RESISTANCE1);

	this->aditionalStatus.ArmorPenetration += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.2f);

	
	this->aditionalStatus.MagicPenetration += (UINT16)this->GetEffect(EF_PIERCING_RESISTANCE2);

	this->aditionalStatus.MagicPenetration += (UINT16)round(this->volatileStatus.Attributes.Intelligence * 0.2f);
}

void StatusController::CalculateDouble() {
	this->aditionalStatus.DoubleRate += (UINT16)this->GetEffect(EF_DOUBLE);
	this->aditionalStatus.DoubleRate += (UINT16)this->GetEffect(EF_RELIQUE_DOUBLE);

	this->aditionalStatus.DoubleRate += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.2f);
	this->aditionalStatus.DoubleRate += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.1f);
}

void StatusController::CalculatePvP() {
	this->aditionalStatus.PvPDamage += (UINT16)this->GetEffect(EF_ATK_NATION2);

	this->aditionalStatus.PvPDefense += (UINT16)this->GetEffect(EF_DEF_NATION2);

	this->volatileStatus.HonorPoints = this->_character->Status.HonorPoints;
	this->volatileStatus.KillPoints = this->_character->Status.KillPoints;
}

void StatusController::CalculateSkillHability() {
	this->aditionalStatus.SkillHability += (UINT16)this->GetEffect(EF_SKILL_DAMAGE);

	this->aditionalStatus.SkillHability += (UINT16)round(this->volatileStatus.Attributes.Spirit * 3);
	this->aditionalStatus.SkillHability += (UINT16)round(this->volatileStatus.Attributes.Strength * 2);
}

void StatusController::CalculateHealing() {
	this->aditionalStatus.HealingRate += (UINT16)this->GetEffect(EF_SKILL_DAMAGE6);

	this->aditionalStatus.HealingRate += (UINT16)round(this->volatileStatus.Attributes.Intelligence * 0.4);
	this->aditionalStatus.HealingRate += (UINT16)round(this->volatileStatus.Attributes.Constitution * 0.3);
	this->aditionalStatus.HealingRate += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.2);
	this->aditionalStatus.HealingRate += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.1);
}

void StatusController::CalculateResistences() {
	this->aditionalStatus.ResistCritical += (UINT16)this->GetEffect(EF_RESISTANCE6);
	this->aditionalStatus.ResistCritical += (UINT16)round(this->volatileStatus.Attributes.Constitution * 0.3);
	this->aditionalStatus.ResistCritical += (UINT16)round(this->volatileStatus.Attributes.Spirit * 0.2);

	this->aditionalStatus.ResistCriticalDamage += (UINT16)this->GetEffect(EF_CRITICAL_DEFENCE);
	this->aditionalStatus.ResistCriticalDamage += (UINT16)round(this->volatileStatus.Attributes.Constitution * 0.2);

	this->aditionalStatus.ResistDouble += (UINT16)this->GetEffect(EF_RESISTANCE7);
	this->aditionalStatus.ResistDouble += (UINT16)round(this->volatileStatus.Attributes.Constitution * 0.2);

	this->aditionalStatus.ResistRate += (UINT16)this->GetEffect(EF_STATE_RESISTANCE);
	this->aditionalStatus.ResistRate += (UINT16)this->GetEffect(EF_RELIQUE_STATE_RESISTANCE);
	this->aditionalStatus.ResistRate += (UINT16)round(this->volatileStatus.Attributes.Intelligence * 0.1);
}

void StatusController::CalculateHithateAndDodge() {
	this->volatileStatus.HitRate += (UINT16)this->GetEffect(EF_HIT);
	this->volatileStatus.HitRate += (UINT16)this->GetEffect(EF_RELIQUE_HIT);
	this->volatileStatus.HitRate += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.5);
	this->volatileStatus.HitRate += (UINT16)round(this->volatileStatus.Attributes.Strength * 0.4);

	this->volatileStatus.DodgeRate += (UINT16)this->GetEffect(EF_PARRY);
	this->volatileStatus.DodgeRate += (UINT16)this->GetEffect(EF_PRAN_PARRY);
	this->volatileStatus.DodgeRate += (UINT16)this->GetEffect(EF_RELIQUE_PARRY);
	this->volatileStatus.DodgeRate += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 0.2);
	this->volatileStatus.DodgeRate += (UINT16)round(this->volatileStatus.Attributes.Spirit * 0.3);
}

void StatusController::CalculateCooldown() {
	this->aditionalStatus.CooldownRate += (UINT16)this->GetEffect(EF_COOLTIME);
	this->aditionalStatus.CooldownRate += (UINT16)this->GetEffect(EF_RELIQUE_COOLTIME);

	this->aditionalStatus.CooldownRate += (UINT16)round(this->volatileStatus.Attributes.Intelligence * 0.2);
}



void StatusController::CalculateEquipmentDamage() {
	PItem equipedWeapon = &this->_character->Equipaments[6];

	if (equipedWeapon->Index == 0) {
		return;
	}

	ItemList* itemList = ItemList::GetInstance();

	BYTE reinforceLevel = equipedWeapon->GetReinforce();

	if (reinforceLevel == 0) {
		this->volatileStatus.Damage.MagDamage +=
			itemList->Get(equipedWeapon->Index).mDamage;
		this->volatileStatus.Damage.PhysDamage +=
			itemList->Get(equipedWeapon->Index).pDamage;
		return;
	}

	reinforceLevel--;

	ReinforceData* reinforceData = ReinforceData::GetInstance();

	this->volatileStatus.Damage.MagDamage +=
		reinforceData->GetReinforce2(equipedWeapon->Index).AttributeMagic[reinforceLevel];
	this->volatileStatus.Damage.PhysDamage +=
		reinforceData->GetReinforce2(equipedWeapon->Index).AttributePhysic[reinforceLevel];
}

void StatusController::CalculateDamage() {
	this->volatileStatus.Damage.PhysDamage += (UINT16)this->GetEffect(EF_DAMAGE1);
	this->volatileStatus.Damage.PhysDamage += (UINT16)this->GetEffect(EF_PRAN_DAMAGE1);

	this->volatileStatus.Damage.PhysDamage += (UINT16)round(this->volatileStatus.Attributes.Strength * 1.8f);
	this->volatileStatus.Damage.PhysDamage += (UINT16)round(this->volatileStatus.Attributes.Dexterity * 1.8f);

	this->volatileStatus.Damage.PhysDamage *= 1 + (this->GetEffect(EF_PER_DAMAGE1) / 100);
	this->volatileStatus.Damage.PhysDamage *= 1 + (this->GetEffect(EF_RELIQUE_PER_DAMAGE1) / 100);

	this->volatileStatus.Damage.PhysDamage *= 1 - (this->GetEffect(EF_DECREASE_PER_DAMAGE1) / 100);


	this->volatileStatus.Damage.MagDamage += (UINT16)this->GetEffect(EF_DAMAGE2);
	this->volatileStatus.Damage.MagDamage += (UINT16)this->GetEffect(EF_PRAN_DAMAGE2);

	this->volatileStatus.Damage.MagDamage += (UINT16)round(this->volatileStatus.Attributes.Intelligence * 2.5f);

	this->volatileStatus.Damage.MagDamage *= 1 + (this->GetEffect(EF_PER_DAMAGE2) / 100);
	this->volatileStatus.Damage.MagDamage *= 1 + (this->GetEffect(EF_RELIQUE_PER_DAMAGE2) / 100);

	this->volatileStatus.Damage.MagDamage *= 1 - (this->GetEffect(EF_DECREASE_PER_DAMAGE2) / 100);
}


void StatusController::CalculateEquipmentDefense() {
	for (int i = 0; i < 16; i++) {
		if (i == 6) {
			continue;
		}

		PItem equipedArmor = &this->_character->Equipaments[i];

		if (equipedArmor->Index == 0) {
			continue;
		}

		ItemList* itemList = ItemList::GetInstance();

		BYTE reinforceLevel = equipedArmor->GetReinforce();

		if (reinforceLevel == 0) {
			this->volatileStatus.Damage.MagDefense +=
				itemList->Get(equipedArmor->Index).mDefense;
			this->volatileStatus.Damage.PhysDefense +=
				itemList->Get(equipedArmor->Index).pDefense;
			continue;
		}

		reinforceLevel--;

		ReinforceData* reinforceData = ReinforceData::GetInstance();

		this->volatileStatus.Damage.MagDefense +=
			reinforceData->GetReinforce2(equipedArmor->Index).AttributeMagic[reinforceLevel];
		this->volatileStatus.Damage.PhysDefense +=
			reinforceData->GetReinforce2(equipedArmor->Index).AttributePhysic[reinforceLevel];
	}
}

void StatusController::CalculateDefense() {
	this->volatileStatus.Damage.PhysDefense += (UINT16)this->GetEffect(EF_RESISTANCE1);
	this->volatileStatus.Damage.PhysDefense += (UINT16)this->GetEffect(EF_PRAN_RESISTANCE1);

	this->volatileStatus.Damage.PhysDefense += (UINT16)round(this->volatileStatus.Attributes.Strength * 1.3f);

	this->volatileStatus.Damage.PhysDefense *= 1 + (this->GetEffect(EF_PER_RESISTANCE1) / 100);
	this->volatileStatus.Damage.PhysDefense *= 1 + (this->GetEffect(EF_RELIQUE_PER_RESISTANCE1) / 100);


	this->volatileStatus.Damage.MagDefense += (UINT16)this->GetEffect(EF_RESISTANCE2);
	this->volatileStatus.Damage.MagDefense += (UINT16)this->GetEffect(EF_PRAN_RESISTANCE2);

	this->volatileStatus.Damage.MagDefense += (UINT16)round(this->volatileStatus.Attributes.Spirit * 1.3f);

	this->volatileStatus.Damage.MagDefense *= 1 + (this->GetEffect(EF_PER_RESISTANCE2) / 100);
	this->volatileStatus.Damage.MagDefense *= 1 + (this->GetEffect(EF_RELIQUE_PER_RESISTANCE2) / 100);

	if (this->GetEffect(EF_UNARMOR) > 0) {
		this->volatileStatus.Damage.PhysDefense = 0;
		this->volatileStatus.Damage.MagDefense = 0;
	}
}


void StatusController::CalculateEquipmentsLifePoints() {
	for (int slot = 2; slot < 8; slot++) {
		if (slot == 6) {
			continue;
		}

		PItem item = &this->_character->Equipaments[slot];

		if (item->Index == 0) {
			continue;
		}

		int reinforceLevel = item->GetReinforce();

		if (reinforceLevel == 0) {
			continue;
		}

		reinforceLevel--;

		ReinforceData* reinforceData = ReinforceData::GetInstance();

		int incrementValue = reinforceData->GetReinforce3(item->Index).HealthIncrementPoints[reinforceLevel];

		this->volatileStatus.Life.MaxHP += incrementValue;
		this->volatileStatus.Life.MaxMP += incrementValue;
	}
}

void StatusController::CalculateLifePoints() {
	this->volatileStatus.Life.MaxHP += HPIncrementPerLevel[(BYTE)((BYTE)this->_character->ClassInfo / 10)]
		* (this->_character->Level + 1);

	this->volatileStatus.Life.MaxHP += this->volatileStatus.Attributes.Constitution * 25;

	this->volatileStatus.Life.MaxHP += this->GetEffect(EF_HP);

	this->volatileStatus.Life.MaxHP *= 1 + (this->GetEffect(EF_HP_CHECK) / 100);
	this->volatileStatus.Life.MaxHP *= 1 + (this->GetEffect(EF_PER_HP) / 100);
	this->volatileStatus.Life.MaxHP *= 1 + (this->GetEffect(EF_MARSHAL_PER_HP) / 100);
	this->volatileStatus.Life.MaxHP *= 1 + (this->GetEffect(EF_RELIQUE_PER_HP) / 100);


	this->volatileStatus.Life.MaxMP += MPIncrementPerLevel[(BYTE)((BYTE)this->_character->ClassInfo / 10)]
		* (this->_character->Level + 1);

	this->volatileStatus.Life.MaxMP += this->volatileStatus.Attributes.Constitution * 25;

	this->volatileStatus.Life.MaxMP += this->GetEffect(EF_HP);

	this->volatileStatus.Life.MaxMP *= 1 + (this->GetEffect(EF_HP_CHECK) / 100);
	this->volatileStatus.Life.MaxMP *= 1 + (this->GetEffect(EF_PER_HP) / 100);
	this->volatileStatus.Life.MaxMP *= 1 + (this->GetEffect(EF_MARSHAL_PER_HP) / 100);
	this->volatileStatus.Life.MaxMP *= 1 + (this->GetEffect(EF_RELIQUE_PER_HP) / 100);

	this->volatileStatus.Life.CurHP = this->_character->Status.Life.CurHP;
	this->volatileStatus.Life.CurMP = this->_character->Status.Life.CurMP;

	if (volatileStatus.Life.CurHP > volatileStatus.Life.MaxHP) {
		volatileStatus.Life.CurHP = volatileStatus.Life.MaxHP;
		_character->Status.Life.CurHP = volatileStatus.Life.MaxHP;
	}

	if (volatileStatus.Life.CurMP > volatileStatus.Life.MaxMP) {
		volatileStatus.Life.CurMP = volatileStatus.Life.MaxMP;
		_character->Status.Life.CurMP = volatileStatus.Life.MaxMP;
	}
}


void StatusController::RestaureLifePoints() {
	this->_character->Status.Life.CurHP = volatileStatus.Life.MaxHP;
	this->_character->Status.Life.CurMP = volatileStatus.Life.MaxMP;

	volatileStatus.Life.CurHP = volatileStatus.Life.MaxHP;
	volatileStatus.Life.CurMP = volatileStatus.Life.MaxMP;
}


void StatusController::SendRefreshStatusScore() {
	PacketCharacterStatus packet;

	ZeroMemory(&packet, sizeof(PacketCharacterStatus));

	packet.Header.Size = sizeof(PacketCharacterStatus);
	packet.Header.Index = 0x7535;
	packet.Header.Code = 0x10A;

	packet.Damage = this->volatileStatus.Damage;

	packet.CriticalRate = this->volatileStatus.CriticalRate;
	packet.DodgeRate = this->volatileStatus.DodgeRate;
	packet.HitRate = this->volatileStatus.HitRate;

	packet.DoubleRate = this->aditionalStatus.DoubleRate;
	packet.ResistHate = this->aditionalStatus.ResistRate;

	packet.SpeedMove = this->aditionalStatus.MoveSpeed;

	ServerInstance* serverInstance = ServerInstance::GetInstance();
	serverInstance->SendPacketToClient(this->_entityId, &packet, packet.Header.Size);
}

void StatusController::SendRefreshAttributes() {
	PacketCharacterAttributtes packet;

	ZeroMemory(&packet, sizeof(PacketCharacterAttributtes));

	packet.Header.Size = sizeof(PacketCharacterAttributtes);
	packet.Header.Index = 0x7535;
	packet.Header.Code = 0x109;

	packet.Attributes = this->volatileStatus.Attributes;

	packet.SkillsPoint = this->volatileStatus.SkillPoints;
	packet.StatusPoint = this->volatileStatus.Attributes.Status;

	ServerInstance* serverInstance = ServerInstance::GetInstance();
	serverInstance->SendPacketToClient(this->_entityId, &packet, packet.Header.Size);
}

void StatusController::SendCurrentLife() {
	PacketCharacterLife packet;

	ZeroMemory(&packet, sizeof(PacketCharacterLife));

	packet.Header.Size = sizeof(PacketCharacterLife);
	packet.Header.Index = this->_entityId;
	packet.Header.Code = 0x103;

	packet.Life = this->volatileStatus.Life;

	PGameEntity gameEntity = EntityHandler::GetEntity(this->_entityId);
	gameEntity->visibleController->SendToVisible(&packet, packet.Header.Size);
}


void StatusController::SendCurrentStatus() {
	if (EntityHandler::GetEntityType(this->_entityId) != EntityPlayer) {
		return;
	}

	this->SendRefreshAttributes();
	this->SendRefreshStatusScore();
	this->SendCurrentLife();
}

#pragma endregion
#pragma region Effects

uint32_t StatusController::GetEffect(uint16_t effectId) {
	return this->_mobEffects[effectId];
}

void StatusController::AddEffect(uint16_t effectId, uint32_t effectValue) {
	this->_mobEffects[effectId] += effectValue;
}

void StatusController::SetEffect(uint16_t effectId, uint32_t effectValue) {
	this->_mobEffects[effectId] = effectValue;
}



void StatusController::RecalculateEffects() {
	ZeroMemory(&this->_mobEffects, sizeof(this->_mobEffects));

	this->CalculateItemEffects();
}

void StatusController::CalculateEquipedItemEffect(BYTE slot) {
	PItem item = &this->_character->Equipaments[slot];

	if (item->Index == 0) {
		return;
	}

	ItemList* itemList = ItemList::GetInstance();
	SetItem* setItem = SetItem::GetInstance();

	ItemFromList itemFromList = itemList->Get(item->Index);

	for (int i = 0; i < 3; i++) {
		this->AddEffect(item->Effects.Index[i], item->Effects.Value[i]);
		this->AddEffect(itemFromList.EF[i], itemFromList.EFV[i]);
	}

	if (itemFromList.SetItemId > 0) {
		this->_setEffects[setItem->GetSetIdByItemId(item->Index)] += 1;
	}
}

void StatusController::CalculateSetEffects() {
	for (auto& effect : this->_setEffects) {
		if (effect.second <= 0) {
			continue;
		}

		SetItem* setItem = SetItem::GetInstance();

		try {
			SetItemFromList _set = setItem->Get(effect.first);

			

			for (int i = 0; i < 6; i++) {
				if (_set.EffectSlot[i] > effect.second) {
					continue;
				}

				this->AddEffect(_set.Effect[i], _set.EffectValue[i]);
			}
		}
		catch (std::exception e) {
			continue;
		}
	}
}

void StatusController::CalculateItemEffects() {
	_setEffects.clear();

	for (BYTE slot = 1; slot < 16; slot++) {
		this->CalculateEquipedItemEffect(slot);
	}

	this->CalculateSetEffects();
}

#pragma endregion