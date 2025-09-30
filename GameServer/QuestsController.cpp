#include "QuestsController.h"

#include "Logger.h"
#include "Player.h"
#include "Packets.h"

#include "EntityHandler.h"

using namespace packets;

#pragma region "initialization"

void QuestsController::Initialize(long characterId, PPlayer player) {
	this->_characterId = characterId;
	this->_player = player;

	this->completedQuests = std::vector<uint16_t>();
	this->availableQuests = std::vector<uint16_t>();

	this->activeQuests = std::map<uint16_t, PlayerQuest>();
}

#pragma endregion
#pragma region "database handler"

bool QuestsController::GetQuestsData() {
	return true;
}

bool QuestsController::SaveQuestsData() {
	return true;
}

#pragma endregion
#pragma region "events"

void QuestsController::OnAcceptQuest(uint16_t questId) {
	PlayerQuest quest = this->activeQuests[questId];

	this->_player->SendClientMessage(Logger::Format("You accepted the quest [%s]", quest.Title));

	this->SendQuestUpdate(quest);
}

void QuestsController::OnFinishQuest(uint16_t questId) {
	PlayerQuest quest = this->activeQuests[questId];

	QuestData questData = QuestList::Get(questId);

	const int EFFECT_QUEST_COMPLETED = 4;

	if (this->_player->visibleController->ContainsVisibleEntity(questData.EndNPC)) {
		this->_player->characterController->SendEntityEffect(questData.EndNPC, EFFECT_QUEST_COMPLETED);
	}

	this->SendQuestUpdate(quest);
}

void QuestsController::OnDeliverQuest(uint16_t questId) {
	QuestData questData = QuestList::Get(questId);

	this->_player->questsController->Evaluate();

	int EffectQuestInProgress = this->_player->questsController->GetNpcQuestIcon(questData.EndNPC);
	this->_player->characterController->SendEntityEffect(questData.EndNPC, EffectQuestInProgress);
}

#pragma endregion



bool QuestsController::AvailableContains(uint16_t questId) {
	if (this->availableQuests.size() == 0) {
		return false;
	}

	return (std::find(availableQuests.begin(), availableQuests.end(), questId) != availableQuests.end());
}

bool QuestsController::ActiveContains(uint16_t questId) {
	if (this->activeQuests.size() == 0) {
		return false;
	}

	return (this->activeQuests.find(questId) != this->activeQuests.end());
}

uint16_t QuestsController::GetNpcQuestIcon(uint16_t npcId) {
	uint16_t effectId = 0;

	for (uint16_t& quest : this->availableQuests) {
		if (this->CompletedContains(quest)) {
			continue;
		}

		auto questData = QuestList::Get(quest);

		uint16_t npcStartId = questData.StartNPC;

		if (npcStartId != npcId) {
			continue;
		}

		if (questData.IsPlotQuest == 1) {
			effectId = 8;
		}
		else {
			effectId = 2;
		}
	}

	for (auto& quest : this->activeQuests) {
		auto questData = QuestList::Get(quest.first);

		uint16_t npcStartId = questData.StartNPC;
		uint16_t npcEndId = questData.EndNPC;

		if (npcId != npcStartId && npcId != npcEndId) {
			continue;
		}

		if (quest.second.IsCompleted && npcId == npcEndId) {
			return 4;
		}

		if (npcId != npcStartId) {
			continue;
		}

		effectId = 3;
	}

	return effectId;
}


bool QuestsController::CompletedContains(uint16_t questId) {
	if (this->completedQuests.size() == 0) {
		return false;
	}

	return (std::find(completedQuests.begin(), completedQuests.end(), questId) != completedQuests.end());
}

#pragma region "evaluation routines"

void QuestsController::Evaluate() {
	QuestList* questList = QuestList::GetInstance(); 

	for (auto& entity : questList->GetQuests()) {
		uint16_t questId = entity.first;

		if (this->AvailableContains(questId)) {
			continue;
		}

		if (this->EvaluateQuestAvailability(entity.second)) {
			this->availableQuests.push_back(questId);
		}
	}

	this->EvaluateQuestsGoals();
}

void QuestsController::EvaluateQuestsGoals() {
	if (this->activeQuests.size() == 0) {
		return;
	}

	for (auto& quest : this->activeQuests) {
		if (quest.second.IsCompleted == 1) {
			continue;
		}

		int goalsCount = quest.second.Goals.size();
		int goalsCompleted = 0;

		for (int i = 0; i < goalsCount; i++) {
			auto goal = quest.second.Goals[i];

			if (goal.CompletedAmount < goal.RequiredAmount) {
				continue;
			}

			goalsCompleted += 1;
		}

		if (goalsCompleted < goalsCount) {
			continue;
		}

		this->activeQuests[quest.first].IsCompleted = 1;

		this->OnFinishQuest(quest.first);
	}
}

bool QuestsController::EvaluateQuestAvailability(uint16_t questId) {
	auto questData = QuestList::Get(questId);

	return this->EvaluateQuestAvailability(questData);
}

bool QuestsController::EvaluateQuestAvailability(QuestData questData) {
	for (int i = 0; i < 5; i++) {
		QuestDetailData availabilityConditions = questData.PreConditions[i];
		int playerLevel = this->_player->character.Level;

		if (questData.PreConditions[i].TypeID == QuestDetailNone) {
			continue;
		}

		switch (availabilityConditions.TypeID)
		{

		case QuestDetailQuest:
			if (!this->CompletedContains(availabilityConditions.Quantity1)) {
				return false;
			}
			break;

		case QuestDetailLevel:
			if (playerLevel < availabilityConditions.Quantity1 || playerLevel > availabilityConditions.Quantity2) {
				return false;
			}
			break;

		case QuestDetailPran:
			return false;
			break;

		default:
			//Logger::Write(Debug, "[%s] QuestID: %d with a non mapped requirement type: %d", __FUNCTION__, questData.Index, availabilityConditions.TypeID);
			break;
		}
	}

	return true;
}

#pragma endregion
#pragma region "quest goals"

void QuestsController::EvaluateGoalMonsterKill(uint16_t mobIndex) {
	for (auto& quest : this->activeQuests) {

		if (quest.second.IsCompleted == 1) {
			continue;
		}

		for (int i = 0; i < quest.second.Goals.size(); i++) {
			auto goal = quest.second.Goals[i];

			if (goal.Type != QuestDetailMonster) {
				continue;
			}

			if (goal.CompletedAmount >= goal.RequiredAmount) {
				continue;
			}

			if (goal.Index != mobIndex) {
				continue;
			}

			this->activeQuests[quest.first].Goals[i].CompletedAmount += 1;

			this->SendQuestUpdate(this->activeQuests[quest.first]);
		}
	}
}

#pragma endregion


bool QuestsController::AcceptQuest(uint16_t questId) {
	QuestData questData = QuestList::Get(questId);

	if (questData.Index <= 0) {
		return false;
	}

	PlayerQuest playerQuest = PlayerQuest();

	playerQuest.Index = questId;

	strcpy_s(playerQuest.Title, questData.Title);

	const int NPC_BASE_ID = 2047;
	int npcId;
	PGameEntity entity;
	 
	for (int i = 0; i < 5; i++) {
		if (questData.Requirements[i].TypeID == 0) {
			continue;
		}

		PlayerQuestGoal goal = PlayerQuestGoal();

		goal.Type = questData.Requirements[i].TypeID;

		switch (questData.Requirements[i].TypeID)
		{

		case QuestDetailEngageNPC:
			npcId = NPC_BASE_ID + questData.Requirements[i].Quantity1;

			entity = EntityHandler::GetEntity(npcId);

			goal.Index = std::atoi(entity->character.Name);
			goal.RequiredAmount = 1;
			break;

		case QuestDetailItem:
			goal.Index = questData.Requirements[i].Item.Index;
			goal.RequiredAmount = questData.Requirements[i].Item.Amount;
			break;

		default:
			goal.Index = questData.Requirements[i].Quantity1;
			goal.RequiredAmount = questData.Requirements[i].Quantity2;
			break;
		}

		goal.CompletedAmount = 0;

		playerQuest.Goals.push_back(goal);
	}

	playerQuest.IsCompleted = 0;

	this->activeQuests.insert(std::make_pair(questId, playerQuest));

	this->OnAcceptQuest(questId);

	return true;
}

bool QuestsController::DeliverQuest(uint16_t questId, int8_t rewardSlotSelected) {
	QuestData questData = QuestList::Get(questId);

	if (questData.Index <= 0) {
		return false;
	}

	if (questData.IsItemChoiceReward() && rewardSlotSelected < 0) {
		this->SendQuestRewardChoice(questId);
		return false;
	}

	if (rewardSlotSelected >= 8) {
		return false;
	}

	uint16_t expReward = 0;
	uint16_t goldReward = 0;

	std::vector<Item> itemsReward = std::vector<Item>();

	for (int i = 0; i < 8; i++) {
		switch (questData.Rewards[i].TypeID)
		{

		case QuestDetailItem:
			itemsReward.push_back(questData.Rewards[i].Item);
			break;

		case QuestDetailExp:
			expReward += questData.Rewards[i].Quantity1;
			break;

		case QuestDetailGold:
			goldReward += questData.Rewards[i].Quantity1;
			break;

		case QuestDetailItemChoice:
			if (rewardSlotSelected != i) {
				break;
			}

			Item rewardItemSelected = questData.Rewards[rewardSlotSelected].Item;
			itemsReward.push_back(rewardItemSelected);
			break;

		default:
			Logger::Write(Debug, "[%s] Quest Reward not mapped : %d", __FUNCTION__, questData.Rewards[i].TypeID);
			break;
		}
	}

	if (this->_player->inventoryController.GetAvailableSlotsCount() < itemsReward.size()) {
		this->_player->SendClientMessage("Inventory Full");
		return false;
	}

	this->completedQuests.push_back(questId);
	this->activeQuests.erase(questId);

	this->SendQuestRemove(questId);

	for (auto& rewardItem : itemsReward) {
		this->_player->inventoryController.PutItem(rewardItem);
	}

	this->_player->characterController->AddExperience(expReward, false);
	this->_player->characterController->AddGold(goldReward, false);

	this->_player->characterController->SendAddExpGold(expReward, goldReward);

	this->OnDeliverQuest(questId);
}

void QuestsController::SendQuestUpdate(PlayerQuest quest) {
	PacketQuestInfo packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->_player->index;
	packet.Header.Code = PacketCode::PacketQuestInfo;

	packet.QuestIndex = quest.Index;

	for (int i = 0; i < quest.Goals.size(); i++) {
		packet.RequirimentType[i] = quest.Goals[i].Type;
		packet.RequirimentItem[i] = quest.Goals[i].Index;
		packet.RequirimentAmount[i] = quest.Goals[i].RequiredAmount;
		packet.RequirimentComplete[i] = quest.Goals[i].CompletedAmount;
	}

	packet.IsCompleted = quest.IsCompleted;

	this->_player->SendPacket(&packet, packet.Header.Size);
}

void QuestsController::SendQuestRemove(uint16_t questId) {
	PacketQuestRemove packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->_player->index;
	packet.Header.Code = PacketCode::PacketQuestRemove;

	packet.QuestIndex = questId;

	this->_player->SendPacket(&packet, packet.Header.Size);
}

void QuestsController::SendQuestRewardChoice(uint16_t questId) {
	PacketQuestRewardSelect packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->_player->index;
	packet.Header.Code = PacketCode::PacketQuestRewardSelect;

	packet.QuestIndex = questId;

	this->_player->SendPacket(&packet, packet.Header.Size);
}