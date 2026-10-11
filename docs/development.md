# Development guide

[Documentation index](README.md) · [Source inventory](source-inventory.md)

## Build environment

Use the [setup instructions](../README.md#setup-instructions) for the current Windows toolchain. The native x64 configurations specify v142; Win32 configurations specify v143. Debug x64 native outputs go into bin/. The solution maps AikaWeb to Any CPU while its csproj sets PlatformTarget to x64.

| Component | Declared dependencies |
| --- | --- |
| AikaWeb | .NET 6; Microsoft.EntityFrameworkCore.SqlServer/Tools 6.0.20; Swashbuckle.AspNetCore 6.2.3. |
| MainServer | Boost 1.87.0 and container/json/system vc142 packages; nlohmann.json 3.11.2. |
| GameServer | Boost 1.80.0 and container/json/system vc142 packages; Lua 5.4.2; nlohmann.json 3.11.2; bundled LuaBridge. |
| Local EF tool | dotnet-ef 6.0.10 from AikaWeb/.config/dotnet-tools.json. |

The native projects also link Windows system libraries through pragma directives, including Winsock, WinHTTP, multimedia timing, and ntdll. nlohmann/json is declared, but Boost JSON is the parser used by the inspected runtime paths. MainServer shares source files with GameServer and carries a project reference; changes to shared networking/HTTP helpers need both builds.

AikaWeb's BaseOutputPath depends on SolutionDir and removes framework/runtime suffixes. CLI builds outside the solution can resolve output differently; specify a solution directory when you need to reproduce the shared output layout, and check the actual build output. Source and generated files coexist under AikaWeb; verify clean-build behavior rather than trusting checked-in binaries.

## Source navigation

| Task | Start here |
| --- | --- |
| API route/request/response | AikaWeb/Controllers and Models. |
| Database schema | Models, Data/EntityHandler.cs, migration and snapshot. |
| Login protocol | MainServer/PacketHandler.cpp and ClientConnection.cpp. |
| Server launch | MainServer/Program.cpp and GameServer/Program.cpp. |
| Packet dispatch/layout | GameServer/PacketHandler.cpp and Packets.h. |
| Character load/save | CharacterController.cpp, Player.cpp, WebHandler.cpp. |
| Item moves/stacking | InventoryController.cpp. |
| Lua consume behavior | UseItemHandler.cpp and bin/Scripts/Itens/. |
| Combat/status | SkillHandler.cpp, GameEntity.cpp, StatusController.cpp, BuffsController.cpp. |
| NPC/quest behavior | NPCHandler.cpp, NPC.cpp, QuestsController.cpp, Quests.h. |
| Entity updates/visibility | EntityHandler.cpp, ServerRoutines.cpp, VisibleController.cpp, Mob.cpp. |
| Game definitions | DataLoader.cpp and corresponding data-store headers/implementations. |

## Changing persistence

1. Identify the model fields and exact existing JSON spelling.
2. Update EF models/context as needed, then generate and review a migration against a disposable database.
3. Update controllers, native WebHandler methods, and both native serialization/deserialization paths.
4. Check SQL/native type widths and null handling. Several SQL long values are currently read as native int.
5. Test creation, loading, updating, disconnect save, and reconnect with the new field.
6. Update API, database, and model-field documentation.

Do not silently rename misspelled fields: they are already wire and schema contracts. A collection POST adds/modifies the supplied records; if deactivation is intended, send the appropriate status change rather than omitting a row.

## Adding gameplay

Follow an existing packet handler into the responsible controller. Validate state, packet lengths, IDs, and array bounds before using client data. Connect the feature to persistence and world update routines only where needed. A PacketCode enum entry by itself does not activate the feature; add dispatch and verify observable client behavior.

For a new skill, inspect the attack dispatcher, SkillData definitions, status effects, buffs, and damage/spawn notifications. Some skill handlers are empty, so a similarly named method is not necessarily a complete template.

For content-only changes, inspect the appropriate reader and a working asset, preserve IDs/field spelling, restart the instance, and validate load counts plus actual gameplay. For Lua, define a positive registration ID and run(controller); see [content](content.md).

## Validation workflow

No dedicated automated test suite or runnable CI workflow was found for the three documented server components. The following is a suggested development validation sequence, not a claim that these checks were executed for this documentation task:

1. Restore dependencies and build the changed component plus consumers of shared code.
2. Restore a disposable GameDatabase and inspect vwSessionsActive behavior.
3. Start AikaWeb and check server/nation lists and Swagger schemas.
4. Start native servers and check asset counts, sockets, and nation initialization.
5. With a compatible client, obtain a token, log in, create/select a character, and enter the world.
6. Exercise the changed system and invalid inputs; for networking changes also validate split/coalesced packets.
7. Disconnect and reconnect, comparing the relevant database records and restored native state.

Repository searches useful for navigating changes:

```powershell
rg -n 'HttpGet|HttpPost' .\AikaWeb\Controllers
rg -n 'case PacketCode::' .\GameServer\PacketHandler.cpp
rg -n 'SaveCharacter|PostCharacter|GetCharacter' .\GameServer
rg -n 'CONSUME_FUNCTION_ID|CONSUME_EFFECT_ID' .\bin\Scripts
git diff --check
```

## Maintenance priorities grounded in current code

- Establish a reproducible fresh database path for the active-session object.
- Add protocol framing/body validation and complete partial-send handling.
- Make shared registry iteration and thread/object lifetime explicit.
- Handle API business-error envelopes and save failures consistently.
- Protect credentials, session issuance, mutation endpoints, and development commands.
- Add targeted integration checks for login, load/save, and shutdown behavior.

These are documented implementation gaps, not changes made by this documentation work. Prefer focused fixes and meaningful regression tests over mirroring existing implementation details.

## Documentation maintenance

The field and opcode appendices were extracted from current declarations during this task. They are static Markdown, with no automatic regeneration hook. When changing a model or enum/dispatch switch, update the corresponding reference. Check links and route coverage, and keep operational facts separate from suggested improvements.

The repository's license text is [GPL version 3](../LICENSE). Consult bundled dependency license files when redistributing their code or binaries.
