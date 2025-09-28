#include "QuestsController.h"

#include "Logger.h"
#include "Player.h"
#include "Packets.h"

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

#pragma endregion



bool QuestsController::AvailableContains(uint16_t questId) {
	if (this->availableQuests.size() == 0) {
		return false;
	}

	return (std::find(availableQuests.begin(), availableQuests.end(), questId) != availableQuests.end());
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
			Logger::Write(Debug, "[%s] QuestID: %d with a non mapped requirement type: %d", __FUNCTION__, questData.Index, availabilityConditions.TypeID);
			break;
		}
	}

	return true;
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
	 
	for (int i = 0; i < 5; i++) {
		if (questData.Requirements[i].TypeID == 0) {
			continue;
		}

		playerQuest.Goals.push_back(questData.Requirements[i]);
	}

	playerQuest.IsCompleted = 0;

	this->activeQuests.insert(std::make_pair(questId, playerQuest));

	this->OnAcceptQuest(questId);

	return true;
}


void QuestsController::SendQuestUpdate(PlayerQuest quest) {
	PacketQuestInfo packet;

	ZeroMemory(&packet, sizeof(packet));

	packet.Header.Size = sizeof(packet);
	packet.Header.Index = this->_player->index;
	packet.Header.Code = PacketCode::PacketQuestInfo;

	packet.QuestIndex = quest.Index;

	for (int i = 0; i < quest.Goals.size(); i++) {
		packet.RequirimentType[i] = quest.Goals[i].TypeID;
		packet.RequirimentItem[i] = quest.Goals[i].Quantity1;
		packet.RequirimentAmount[i] = quest.Goals[i].Quantity2;
		packet.RequirimentComplete[i] = 0;
	}

	packet.IsCompleted = quest.IsCompleted;

	this->_player->SendPacket(&packet, packet.Header.Size);
}