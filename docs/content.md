# Runtime content and Lua

[Documentation index](README.md)

## File locations

Native JSON configuration is read from the process working directory. GameServer's DataLoader derives the executable directory using `GetModuleFileNameA` and resolves its asset directories there. Running from `bin/` with the executable in `bin/` satisfies both rules. Do not move only the executable and expect data loading to follow the old directory.

`bin/` contains content as well as build output. Cleaning it indiscriminately removes the game's runtime assets.

## Startup load order

DataLoader runs these steps in order and returns on a reported failure:

| Order | Content | Loader / store |
| --- | --- | --- |
| 1 | Data/ItemList.bin | LoadItemList / ItemList |
| 2 | InitialCharacters/*.json | LoadInitialCharacters / InitialCharacters |
| 3 | Data/SetItem.bin | LoadSetItem / SetItem |
| 4 | Data/SkillData.bin | LoadSkillData / SkillData |
| 5 | Data/Reinforce2.bin | LoadReinforce2 / ReinforceData |
| 6 | Data/Reinforce3.bin | LoadReinforce3 / ReinforceData |
| 7 | Scripts/Itens/ConsumeFunctions/*.lua | Item-type script registration |
| 8 | Scripts/Itens/ConsumeEffects/*.lua | Use-effect script registration |
| 9 | Data/Title.bin | LoadTitles / TitleList |
| 10 | Data/ScrollPos.bin | LoadTeleportPositions / MapData |
| 11 | Data/Map.bin | LoadMapData / MapData |
| 12 | NPCs/*.json | LoadNPCs / NPC entities |
| 13 | Mobs/*.json | LoadMobs / Mob entities |
| 14 | Data/ExpList.bin | LoadExpList / ExpList |
| 15 | Drops/*.json | LoadDropList / DropList |
| 16 | Data/Quest.bin | LoadQuestList / QuestList |

A LoadMobPos function for Data/MobPos.bin exists, but it is not called by this Startup sequence; active mob placements come from JSON Positions. Other binary files in Data/ are not necessarily consumed by startup. DropData/ is present on disk but is not the directory used by LoadDropList.

The folder enumerator returns success when no matching files are found. A zero-file log is therefore worth checking even if startup continues. Lua registration errors can be logged while the per-file procedure still reports success. Startup success alone does not prove every asset was loaded.

## Binary assets

Binary readers are implemented in DataLoader.cpp and decode records into the structs in ItemData.h, SkillData.h, ReinforceData.h, TitleList.h, MapData.h, ExpList.h, and Quests.h. These assets have format-specific headers, counts, offsets, and fixed fields; they are not serialized JSON or a general database export. Use those readers and the bundled pattern/project files under Data/ when changing formats. Files such as Dialog.bin and Recipes.bin being present do not imply their related gameplay systems are complete.

## JSON content

| Folder | Key fields and consumers |
| --- | --- |
| InitialCharacters | `index`, `class_info`, `level`, `status` (`str`, `dex`, `int`, `const`, `spirit`, `stats`), `sizes`, `gold`, `start_equips`, `start_inventory`, `start_skills`, `start_item_bar`; parsed by InitialCharacters/Character helpers. |
| NPCs | `Index`, `Name`, `Description`, appearance/effects, `Rotation`, `Sizes`, `Position`, `Equip`, `Inventory`, plus dialogue/options; parsed by NPC. |
| Mobs | `Index`, `Name`, appearance, Health, Level, RespawnTime, DropIndex, Skills, and Positions; parsed by Mob and DataLoader. |
| Drops | `Index` and `DropList` array with `item` and `rarity`; parsed by ProcessDropFile. |

JSON keys are case-sensitive in the Boost JSON lookups. The API's camel-case conventions do not apply to these content files. Use an existing file as a complete template, then inspect the corresponding parser for required fields.

Initial character files cover warrior, paladin, rifleman, dualgunner, warlock, and cleric. Native class identifiers come from CharacterClass; the templates also carry their explicit class_info values.

Each mob Positions entry provides start/destination X/Y, range, and wait values. DataLoader creates an instance per placement. `Index` is a content-definition identifier, while the entity registry supplies the live world ID.

Drop entries may also contain an `id`, but ProcessDropFile reads `item` and `rarity`. Rarity is mapped to the chance constants in DropList.h; do not assume an additional JSON chance property overrides them.

## Lua item scripts

The directory spelling is **Scripts/Itens/**. Function scripts declare `CONSUME_FUNCTION_ID`; effect scripts declare `CONSUME_EFFECT_ID`. The loader indexes each file by this positive numeric global, not its filename prefix.

When an inventory item is used:

1. UseItemHandler opens a Lua state and standard libraries.
2. The item's ItemType selects a consume-function script.
3. LuaBridge registers the UseItemHandler methods and the file is executed.
4. Its `run(controller)` function is called.
5. `bConsumeItem` indicates whether to consume the item if a nested action has not already set the flag.
6. The state closes, consumed amount is removed, and status is recalculated.

Chest scripts call OpenChest, which runs the effect script selected by the item's UseEffect.

Example based on the bundled health-potion script:

```lua
CONSUME_FUNCTION_ID = 700
bConsumeItem = false

function run(controller)
    controller:AddCharacterHealth()
    bConsumeItem = true
end
```

The healing amount comes from the current item's UseEffect. This interface does not accept an arbitrary healing amount.

| Lua method | Purpose |
| --- | --- |
| OpenChest() | Execute the current item's consume-effect script. |
| AddCharacterItem(itemId, amount) | Add an item if inventory space permits. |
| GetAvailableSlots() | Return available inventory slot count. |
| SendMessage(text) | Send a message to the current player. |
| GetCharacterClass() | Return the class grouping calculated from ClassInfo / 10. |
| AddCharacterHealth() | Add current item's UseEffect to HP. |
| AddCharacterMana() | Add current item's UseEffect to MP. |
| AddCharacterBuff() | Add a buff identified by UseEffect. |
| GetServerNation() | Return the game instance's nation ID. |
| GetCharacterNation() | Return the account's nation ID. |
| TeleportScroll() | Use the item packet's use data to select ScrollPos destination. |
| MoveUsedItemToSlot(slot) | Swap the used inventory item into another slot and refresh both. |

Scripts are executed during registration as well as use. Keep top-level code limited to definitions and metadata; player-dependent work belongs in run. Runtime use opens standard libraries, so these are trusted server-side scripts, with no sandbox boundary in the current code.

Restart GameServer after content changes to reload definitions and script registrations. Scripts are re-executed on use, but there is no general content hot-reload system.

Sources: `GameServer/DataLoader.cpp`, `UseItemHandler.cpp`, `ItemList.cpp`, `InitialCharacters.cpp`, `NPC.cpp`, `Mob.cpp`, `DropList.h`, and `bin/Scripts/Itens/`.
