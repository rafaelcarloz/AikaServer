# Game systems

[Documentation index](README.md)

This is a map of implemented handlers and data structures. A declaration or packet enum does not establish a complete game feature; the limits below distinguish active paths from scaffolding.

## Entities and world state

`GameEntity` holds base and current Character records, position/rotation, combat timestamps, attacker/target pointers, status, buffs, and visibility controllers. It sends spawn/despawn, movement, action, damage, and effect messages. `Player` adds account/session data, a socket, character selection, inventory, quests, and current NPC interaction. `Mob` adds movement/respawn/attack state. `NPC` provides dialogue, options, shop, and quest data.

Entity ID classification constants are:

| Kind | Classified range | Automatic allocation loop |
| --- | --- | --- |
| Player | 1–2000 | 1–1999 |
| NPC | 2048–3047 | 2048–3046 |
| Mob | 3048–9147 | 3048–9146 |

Allocation loops use `< END`, while classification accepts `<= END`. These are implementation ranges, not verified capacity limits. Registry removal erases the raw pointer from the map without deleting the pointed object. IDs are local to each GameServer process.

## Player and character lifecycle

`PlayerData.h` declares WaitingLogin, CharList, Numeric, Waiting, and Playing states. The login handler validates username/token through AikaWeb and loads up to three character previews. Character slot indices are 0–2.

`OnNumericTokenRequest` supports registration, checking, and changing the numeric token. Five or more recorded errors block selection in this handler. Valid selection initializes the CharacterController, loads character, skills, hotbar, titles, inventory, and quests, then sends world-entry data. The client entry packet invokes `EnterIngame`, setting Playing and `IsIstantiated` and initializing combat/regeneration timestamps.

Character creation checks face IDs 10–69, hair IDs 7700–7779, and slots up to 2. It selects a base template through the face item's class, creates the character through AikaWeb, and posts starter data. Location choice 0 uses (3450, 690); choice 1 uses (3470, 935). The API also checks name and slot uniqueness. A commented-out success check in the native creation path means HTTP 200 business failures are not robustly handled before accessing the returned character ID.

Disconnect unspawns and saves instantiated characters, then closes the socket. See [persistence](database.md#save-sequence) for the sequence and partial-save behavior.

## Native character capacities

| Data | Capacity in Character struct |
| --- | --- |
| Name | 16-byte character array |
| Equipment | 16 items and 16 database IDs |
| Inventory | 64 items and 64 database IDs |
| Skills | 60 entries |
| Hotbar | 24 entries |
| Titles | 96 entries |

Database/API strings and arrays are not uniformly constrained to these sizes. Any new persistence or packet path must validate before indexing or copying into native structures.

## Gameplay module map

| Module | Main implemented responsibility |
| --- | --- |
| `CharacterController` | Load/save character state, enter world, respawn/unspawn, teleport, experience/level/gold updates, skill acquisition, active titles, and mob rewards. |
| `InventoryController` | Equipment/inventory deserialization, slot access, move/swap, stack/split, delete, consume amounts, add/find items, and inventory serialization. |
| `StatusController` | Derive attributes, damage/defense, HP/MP, equipment/set/buff effects, critical/dodge/hit modifiers, regeneration, and damage over time. |
| `SkillHandler` | Attack-target dispatch, basic attacks, return/teleport behavior, and selected warrior/warlock skill implementations. Several named handlers are placeholders. |
| `BuffsController` | Buff duration/expiry, effect recalculation, database load/serialization, and refresh packets. |
| `VisibleController` | Nearby entity membership and packet broadcasting to visible players. Uses a distance threshold of 40 in EvaluateEntity. |
| `Mob` | JSON-defined mob state, movement between positions, attack behavior, death, respawn, and base experience. |
| `NPCHandler` | NPC dialogue/options, shop display, quest listing/acceptance/delivery, and closing interactions. |
| `QuestsController` | Database progress, available/active/completed quests, goals, quest-specific drops, reward selection, delivery, cancellation, and client updates. |
| `UseItemHandler` | Execute Lua consume functions/effects, inventory consumption, healing, mana, buffs, chests, teleport scrolls, and messages. |
| `CommandHandler` | Three backtick-prefixed development commands through chat packets. |
| Definition stores | `ItemList`, `SkillData`, `SetItem`, `ReinforceData`, `TitleList`, `MapData`, `MobPos`, `ExpList`, `DropList`, `QuestList`, and `InitialCharacters`. |

## Combat and movement

The movement handler directly adopts packet Destination and broadcasts movement with the supplied move type/speed. Rotation and action handlers also relay changes. This path does not establish authoritative movement, collision, or speed validation.

Attack-target packets enter SkillHandler, which finds targets and dispatches skill-specific logic. GameEntity applies physical/magical damage, sends damage information, reduces life/mana, and handles death. StatusController derives modifiers from base character data, equipment, sets, and buffs. Consult the formulas in the corresponding implementation files when changing balance; this documentation does not imply all enum-defined skills are implemented.

Revival uses a saved position for a player's own nation and another fixed location for visitors. Current HP is restored to a fraction of calculated maximum life in the handler. Mob kills trigger experience/drop and quest-goal paths through CharacterController.

## Inventory and item use

Inventory operations are dispatched by PacketHandler and delegated to InventoryController. NPC buying/selling, item unsealing, hotbar updates, and skill acquisition/reset have dedicated packet handlers. Runtime item metadata comes from ItemList.bin; persistent item instances come from CharacterItems.

Consume functions are selected by item type and consume effects by UseEffect. Lua bindings and script registration are covered in [content](content.md#lua-item-scripts). Equipped/inventory capacities and slot enums must be respected independently of API collection sizes.

## Quests and NPCs

Quest definitions come from Quest.bin; player progress comes from CharacterQuests. QuestsController tracks available, active, and completed sets, up to five persisted goal counters, acceptance/completion timestamps, and additional quest drops. It uses `COMPLETED` as the persisted completed-state string. A background routine evaluates instantiated players every 2 seconds plus work time.

NPCHandler uses loaded NPC options/dialogues and quest availability to present actions. Quest rewards can require a client choice. Development commands can accept/deliver quests directly and are not a substitute for normal NPC progression tests.

## Development chat commands

The first character is a backtick. Each command takes one positive integer argument:

| Command text | Action |
| --- | --- |
| `` `addquest 123 `` | Calls AcceptQuest(123). |
| `` `deliverquest 123 `` | Calls DeliverQuest(123). |
| `` `addeffect 123 `` | Sends the effect to the current player's entity. |

Command matching is case-insensitive. No role/administrator permission check appears in CommandControl. `OnChatPacket` otherwise returns without broadcasting normal chat.

## Current feature limits

- PartyController.cpp only includes its header; party-related packet definitions do not constitute an implemented party system.
- Guilds, trading, personal shops, auctions, fishing, and other features have enum/structure entries but are not dispatched by the inspected inbound switch.
- The anti-hack status packet is accepted without action in the dispatch switch.
- Version and maxConnections are loaded from JSON; the inspected startup/accept paths do not enforce them as admission checks.
- Background updates use detached threads and shared pointers/maps; lifecycle synchronization needs explicit attention when extending systems.

Use the [packet code inventory](packet-codes.md) to distinguish enum definitions from active inbound cases, and read each handler before claiming end-to-end support.
