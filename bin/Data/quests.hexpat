struct QuestDetailData {
    u32 TypeID;
    u32 Quantity1;
    u32 Quantity2;
    u32 UNK_1;
    
    u16 ItemID1;
    u16 ItemID2;
    u32 UNK_2;
    u16 UNK_3;
    u16 UNK_4;
    u32 UNK_5;
    u32 Quantity;
};

struct QuestData {
    u16 Index;
    u16 UNK_1;
    u32 StartNPC;
    u32 EndNPC;
    u32 UNK_2;
    char Title[64];
    u16 DialogStart;
    u16 DialogUnfinished;
    u16 DialogEnd;
    u16 Summary;
    u8 UNK_3[1155];
    u8 Level;
    u16 UNK_VAL;
    u8 IsPlotQuest;
    u8 UNK_Effect1;
    u8 UNK_Effect2;
    u8 UNK_Effect3;
    u16 UNK_7;
    QuestDetailData PreConditions[5];
    QuestDetailData Requirements[5];
    QuestDetailData GenericRewards[2];
    QuestDetailData Rewards[14];
    QuestDetailData DropDetails[4];
};

QuestData questArray[200] @ 0x00;