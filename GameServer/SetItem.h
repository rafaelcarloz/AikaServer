#pragma once

#ifndef _SET_ITEM_H_
#define _SET_ITEM_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <map>


typedef struct SetItemFromList {
	char Name[64];
	char NameEnglish[64];
	WORD UNK_1[2];
	WORD RequiredEquips[12];
	WORD EffectSlot[10];
	WORD UNK_2[8];
	WORD Effect[6];
	WORD EffectValue[6];
}*PSetItemFromList;


class SetItem {
private:
	static SetItem* _instance;

	std::map<WORD, SetItemFromList> _items;
	std::map<WORD, WORD> _itemsBySett;

	WORD _itemCount = 0;
protected:
	SetItem() {}
	~SetItem() {}
public:
	SetItem(SetItem& other) = delete;
	void operator=(const SetItem&) = delete;

	static SetItem* GetInstance();

	bool Add(const SetItemFromList item);
	bool Get(const WORD itemId, SetItemFromList* item);

	SetItemFromList Get(const WORD itemId);
	SetItemFromList GetByItemId(const WORD itemId);
	WORD GetSetIdByItemId(const WORD itemId);

	SetItemFromList operator[](const WORD itemId);
};

#endif
