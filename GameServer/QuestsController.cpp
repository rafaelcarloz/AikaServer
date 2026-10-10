#include "QuestsController.h"

#include "Logger.h"
#include "Player.h"
#include "Packets.h"

#include "EntityHandler.h"
#include "WebHandler.h"

#include "DateFunctions.h"

using namespace packets;

const int NPC_BASE_ID = 2047;

#pragma region "initialization"

void QuestsController::Initialize(long characterId, PPlayer player) {
	this->_characterId = characterId;
	this->_player = player;

	this->completedQuests = std::vector<uint16_t>();
	this->availableQuests = std::vector<uint16_t>();

	this->activeQuests = std::map<uint16_t, PlayerQuest>();
	this->questsDatabase = std::map<uint16_t, DatabaseQuest>();

	this->questDrops = std::vector<QuestAditionalDrop>();


	if (!this->GetQuestsData()) {
		return;
	}
}

bool QuestsController::Destroy() {
	return this->SaveQuestsData();
}

#pragma endregion
#pragma region "database handler"

bool QuestsController::GetQuestsData() {
	WebHandler webHandler;

	try {
		json::value response = json::value();

		if (!webHandler.GetCharacterQuests(this->_player->character.CharacterId, &response)) {
			return false;
		}

		for (auto& item : response.as_array()) {
			uint64_t characterQuestID = json::value_to<uint64_t>(item.at("characterQuestID"));

			uint16_t questId = json::value_to<int>(item.at("questID"));
			std::string questStatus = json::value_to<std::string>(item.at("questStatus"));

			if (questStatus == "COMPLETED") {
				this->completedQuests.push_back(questId);
			}
			else {
				PlayerQuest playerQuest = PlayerQuest();

				playerQuest.Index = questId;
				
				QuestData questData = QuestList::Get(questId);

				strcpy_s(playerQuest.Title, questData.Title);

				for (int i = 0; i < 5; i++) {
					PlayerQuestGoal goal = this->ParseQuestGoal(questData.Requirements[i]);

					if (goal.Type == 0) {
						continue;
					}

					goal.CompletedAmount = json::value_to<int>(item.at(Logger::Format("goalCompleted_%d", i + 1)));

					playerQuest.Goals.push_back(goal);
				}

				this->activeQuests.insert(std::make_pair(questId, playerQuest));
				this->AddQuestDrops(questId);
			}

			DatabaseQuest databaseQuest = DatabaseQuest();

			databaseQuest.ID = characterQuestID;
			databaseQuest.AcceptedOn = DateFunctions::StringToDate(json::value_to<std::string>(item.at("acceptedOn")));
			
			if (!(item.at("completedOn").is_null())) {
				databaseQuest.CompletedOn = DateFunctions::StringToDate(json::value_to<std::string>(item.at("completedOn")));
				databaseQuest.isCompleted = true;
			}

			this->questsDatabase.insert(std::make_pair(questId, databaseQuest));
		}

		return true;
	}
	catch (std::exception e) {
		Logger::Write(Error, "[%s] error: %s",__FUNCTION__, e.what());
	}

	return false;
}

bool QuestsController::SaveQuestsData() {
	WebHandler webHandler;
	json::value questData;

	try {
		json::array& _items = questData.emplace_array();

		for (uint16_t questId : this->completedQuests) {
			json::value itemData = json::value();
			json::object& _item = itemData.emplace_object();

			DatabaseQuest databaseQuest = this->GetDatabaseQuest(questId);

			if (databaseQuest.isCompleted) {
				continue;
			}

			_item.emplace("CharacterQuestID", databaseQuest.ID);
			_item.emplace("IsActive", 1);
			_item.emplace("QuestStatus", "COMPLETED");
			_item.emplace("CharacterID", this->_characterId);
			_item.emplace("QuestID", questId);

			for (int i = 0; i < 5; i++) {
				_item.emplace(Logger::Format("GoalCompleted_%d", i + 1), 255);
			}

			_item.emplace("AcceptedOn", DateFunctions::DateToString(databaseQuest.AcceptedOn));
			_item.emplace("CompletedOn", DateFunctions::DateToString(databaseQuest.CompletedOn));

			_items.emplace_back(_item);
		}

		for (auto& quest : this->activeQuests) {
			json::value itemData = json::value();
			json::object& _item = itemData.emplace_object();

			DatabaseQuest databaseQuest = this->GetDatabaseQuest(quest.first);

			_item.emplace("CharacterQuestID", databaseQuest.ID);
			_item.emplace("IsActive", 1);
			_item.emplace("QuestStatus", "ACTIVE");
			_item.emplace("CharacterID", this->_characterId);
			_item.emplace("QuestID", quest.first);

			for (int i = 0; i < 5; i++) {
				if (i >= quest.second.Goals.size()) {
					_item.emplace(Logger::Format("GoalCompleted_%d", i + 1), 0);
					continue;
				}

				_item.emplace(Logger::Format("GoalCompleted_%d", i + 1), quest.second.Goals[i].CompletedAmount);
			}

			_item.emplace("AcceptedOn", DateFunctions::DateToString(quest.second.AcceptedOn));

			_items.emplace_back(_item);
		}

		json::value response;

		if (!webHandler.PostCharacterQuests(this->_characterId, questData, &response)) {
			return false;
		}
	}
	catch (std::exception e) {
		Logger::Write(Error, "[%s] error: %s", __FUNCTION__, e.what());
		return false;
	}

	return true;
}

void QuestsController::UpdateQuestDatabaseInfo(PlayerQuest questData) {
	DatabaseQuest databaseQuest = DatabaseQuest();

	if (this->questsDatabase.find(questData.Index) != this->questsDatabase.end()) {
		this->questsDatabase[questData.Index].CompletedOn = questData.CompletedOn;
	}
	else {
		databaseQuest.ID = 0;
		databaseQuest.AcceptedOn = questData.AcceptedOn;
		databaseQuest.CompletedOn = questData.CompletedOn;

		this->questsDatabase.insert(std::make_pair(questData.Index, databaseQuest));
	}
}

DatabaseQuest  QuestsController::GetDatabaseQuest(uint16_t questId) {
	return (this->questsDatabase.find(questId) != this->questsDatabase.end()) ? this->questsDatabase[questId] : DatabaseQuest();
}

#pragma endregion
#pragma region "events"

void QuestsController::OnAcceptQuest(uint16_t questId) {
	PlayerQuest quest = this->activeQuests[questId];

	this->_player->SendClientMessage(Logger::Format("You accepted the quest [%s]", quest.Title));

	this->AddQuestDrops(questId);

	this->SendQuestUpdate(quest, true);
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

	this->RemoveQuestDrops(questId);

	int EffectQuestInProgress = this->_player->questsController->GetNpcQuestIcon(questData.EndNPC);
	this->_player->characterController->SendEntityEffect(questData.EndNPC, EffectQuestInProgress);
}

void QuestsController::OnCancelQuest(uint16_t questId) {
	this->RemoveQuestDrops(questId);
}

#pragma endregion
#pragma region "quest helpers"

PlayerQuestGoal QuestsController::ParseQuestGoal(QuestDetailData questDetail) {
	PlayerQuestGoal goal = PlayerQuestGoal();

	if (questDetail.TypeID == 0) {
		return goal;
	}

	int npcId;
	PGameEntity entity;

	goal.Type = questDetail.TypeID;

	switch (goal.Type)
	{

	case QuestDetailEngageNPC:
		npcId = NPC_BASE_ID + questDetail.Quantity1;

		entity = EntityHandler::GetEntity(npcId);

		goal.Index = std::atoi(entity->character.Name);
		goal.RequiredAmount = 1;
		break;

	case QuestDetailItem:
		goal.Index = questDetail.Item.Index;
		goal.RequiredAmount = questDetail.Item.Amount;
		break;

	default:
		goal.Index = questDetail.Quantity1;
		goal.RequiredAmount = questDetail.Quantity2;
		break;
	}

	return goal;
}

void QuestsController::AddQuestDrops(uint16_t questId) {
	QuestData questData = QuestList::Get(questId);

	for (int slot = 0; slot < 4; slot++) {
		if (questData.DropDetails[slot].TypeID != 2) {
			continue;
		}

		QuestAditionalDrop drop = QuestAditionalDrop();

		drop.QuestId = questId;
		drop.MobId = questData.DropDetails[slot].UNK_1;
		drop.DropRate = questData.DropDetails[slot].Quantity2;
		drop.DropItem = questData.DropDetails[slot].Item;

		this->questDrops.emplace_back(drop);
	}
}

void QuestsController::RemoveQuestDrops(uint16_t questId) {
	for (int i = 0; i < this->questDrops.size(); i++) {
		if (this->questDrops[i].QuestId != questId) {
			continue;
		}

		this->questDrops.erase(this->questDrops.begin()+i);
	}
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

			if (goal.Type == QuestDetailItem) {
				PItem questItem = this->_player->inventoryController.FindItemById(goal.Index);

				int amount = 0;

				if (questItem == nullptr) {
					amount = 0;
				}
				else {
					amount = questItem->Amount;
				}

				this->activeQuests[quest.first].Goals[i].CompletedAmount = amount;

				if (amount != goal.CompletedAmount) {
					this->SendQuestUpdate(this->activeQuests[quest.first]);
				}
			}

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
		int playerLevel = this->_player->character.Level+1;

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
			if (playerLevel < availabilityConditions.Quantity1) {
				return false;
			}
			break;

		case QuestDetailPran:
			if (availabilityConditions.Quantity1 > 0) {
				return false;
			}

			break;

		case QuestDetailItem:
			if (!this->_player->inventoryController.HasItem(availabilityConditions.Item.Index, availabilityConditions.Item.Amount)) {
				return false;
			}
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

	std::vector<Item> itemsAcceptReward = std::vector<Item>();

	for (int i = 0; i < 3; i++) {
		if (questData.AcceptRewards[i].TypeID == QuestDetailNone) {
			continue;
		}

		switch (questData.AcceptRewards[i].TypeID)
		{

		case QuestDetailItem:
			itemsAcceptReward.push_back(questData.AcceptRewards[i].Item);
			break;

		default:
			Logger::Write(Debug, "[%s] Quest Accept Reward not mapped : %d", __FUNCTION__, questData.AcceptRewards[i].TypeID);
			break;
		}
	}

	if (this->_player->inventoryController.GetAvailableSlotsCount() < itemsAcceptReward.size()) {
		this->_player->SendClientMessage("Inventory Full");
		return false;
	}
	 
	for (int i = 0; i < 5; i++) {
		PlayerQuestGoal goal = this->ParseQuestGoal(questData.Requirements[i]);

		if (goal.Type == 0) {
			continue;
		}

		goal.CompletedAmount = 0;

		playerQuest.Goals.push_back(goal);
	}

	playerQuest.IsCompleted = 0;
	playerQuest.AcceptedOn = time(0);

	this->activeQuests.insert(std::make_pair(questId, playerQuest));

	for (auto& rewardItem : itemsAcceptReward) {
		this->_player->inventoryController.PutItem(rewardItem, 0, true);
	}

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
	std::vector<Item> itemsRewardChoices = std::vector<Item>();

	for (int i = 0; i < 8; i++) {
		if (questData.Rewards[i].TypeID == QuestDetailNone) {
			continue;
		}

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
			itemsRewardChoices.push_back(questData.Rewards[i].Item);
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

	std::vector<Item> itemsRemove = std::vector<Item>();

	for (int i = 0; i < 5; i++) {
		if (questData.DeleteOnComplete[i].TypeID == QuestDetailNone) {
			continue;
		}

		switch (questData.DeleteOnComplete[i].TypeID)
		{

		case QuestDetailItem:
			itemsRemove.push_back(questData.DeleteOnComplete[i].Item);
			break;

		default:
			Logger::Write(Debug, "[%s] Quest Delete On Complete type not mapped : %d", __FUNCTION__, questData.Rewards[i].TypeID);
			break;
		}
	}

	PlayerQuest playerQuest = this->activeQuests[questId];
	playerQuest.CompletedOn = time(0);

	this->UpdateQuestDatabaseInfo(playerQuest);

	this->completedQuests.push_back(questId);
	this->activeQuests.erase(questId);

	this->SendQuestRemove(questId);

	if (rewardSlotSelected >= 0) {
		Item rewardItemSelected = itemsRewardChoices[rewardSlotSelected];
		itemsReward.push_back(rewardItemSelected);
	}

	for (auto& rewardItem : itemsReward) {
		this->_player->inventoryController.PutItem(rewardItem, 0, true);
	}

	for (auto& deleteItem : itemsRemove) {
		this->_player->inventoryController.DecreaseAmount(deleteItem.Index, deleteItem.Amount);
	}

	this->_player->characterController->AddExperience(expReward, false);
	this->_player->characterController->AddGold(goldReward, false);

	this->_player->characterController->SendAddExpGold(expReward, goldReward);

	this->OnDeliverQuest(questId);
}

bool QuestsController::CancelQuest(uint16_t questId) {
	return true;
}


void QuestsController::SendQuestUpdate(PlayerQuest quest, bool isNew) {
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
	packet.IsOld = !isNew;

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

void QuestsController::SendQuests() {
	for (auto& quest : this->activeQuests) {
		this->SendQuestUpdate(quest.second);
	}
}