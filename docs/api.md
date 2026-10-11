# AikaWeb API reference

[Documentation index](README.md) · [Model fields](model-fields.md)

## Hosting and conventions

`Program.cs` registers EF Core against `GameDbConnection`, MVC controllers, and Swagger. Swagger is enabled outside the empty development-only block, so `/swagger` is available in every environment under the current code. The shared native client expects `https://localhost:8090`; the Visual Studio launch profile instead uses HTTPS `7213` and HTTP `5220`.

JSON responses use ASP.NET Core's default camel-case names. Request deserialization accepts the PascalCase property names used by the native server. Preserve existing spelling, including `currentHelthPoints`, `addinitionalConstituion`, `itemIdentication`, and `DaleteTime` in the deletion DTO.

Controllers have `[ApiController]`, so binding/validation failures can produce HTTP 400 before action logic. `TokenRequest` requires `id` and `pw`, with a maximum `id` length of 140. Most database exceptions are not translated into a stable application error contract.

There is no configured authentication scheme or `[Authorize]` protection on these controllers. `UseAuthorization()` alone does not secure character mutation routes. This describes the current implementation and matters when choosing the API's network exposure.

## Legacy authentication routes

These routes preserve client-facing `.asp` names although the implementation is ASP.NET Core.

| Method | Route | Input | Successful result / application errors |
| --- | --- | --- | --- |
| POST | `/member/aika_get_token.asp` | Form `id` = username, `pw` = password | Compares `AccountPassword` directly; creates a status-1 session; returns lowercase MD5 of the session ID. Returns text `0` if account absent, `-1` if password differs. |
| POST | `/servers/aika_get_chrcnt.asp` | Form `id` = username, `pw` = session token | Resolves latest active session; counts characters with status > 0; returns `CNT {count} 0 0 0<br>{nation} 0 0 0`. Returns `0`/`-1` for lookup/token failure. |
| POST | `/servers/aika_reset_flag.asp` | Form `id`, `pw` = session token | Updates session timestamp and returns session hash; lookup/token failures use `0`/`-1`. See query caveat below. |
| POST | `/servers/serv0{serverId}.asp` | Route server ID | Always returns `1 0`; does not query online player counts. |

The `0` and `-1` values are response bodies, not HTTP status codes. The reset-flag query uses `username.Equals(tokenRequest.id)` inside `Single`, without comparing the row's username. When those input strings match, it effectively selects all rows and fails if there is more than one. Do not assume this endpoint correctly chooses the user's session.

## Native-server session validation

`POST /session/data`, body:

```json
{"id":"example_user","pw":"session_token_from_get_token"}
```

On success, returns the [SessionActiveModel](model-fields.md#sessionactivemodel) row and changes the backing `Sessions` row to status `2` with a new `UpdatedOn` timestamp. The native client uses `sessionID`, `accountID`, `accountNation`, and `accountUsername`.

| Condition | Response |
| --- | --- |
| Empty username | 400 |
| Session query throws or finds no row | 400 with exception message |
| Username comparison fails | 404 |
| Token differs | 401 with the submitted token as body |
| Backing session missing | 401 |
| Valid session | 200 with session data |

The action selects the most recently updated row by username. It does not implement an explicit token expiration interval or duplicate-login policy. The native `WebHandler::ProcessResponse` accepts only HTTP 200 and expects JSON for all its endpoints.

## Servers and nations

| Method | Route | Result |
| --- | --- | --- |
| GET | `/servers/all` | ServerModel array, filtered to `ServerStatus > 0`; MainServer launches one game process for each row. |
| GET | `/nations/all` | NationModel array, filtered to `NationStatus > 0`. |
| GET | `/nations/{id}` | Find by ID; 200 with record, 204 if missing, 404 if lookup throws. Single-record lookup does not filter status. |

No creation/update API for server, nation, or account records is implemented. Manage these through database tooling.

## Characters

| Method | Route | Request / behavior |
| --- | --- | --- |
| GET | `/characters/{accountId}/all` | Active characters (`CharacterStatus > 0`) for account, ordered by `CharacterSlot`. |
| GET | `/characters/{characterId}` | Active character by ID; 404 on lookup failure. |
| POST | `/characters/add` | Full CharacterModel. Sets status 1, creation time/address. Rejects empty name, duplicate active name, or occupied active account slot through `success:false` response. |
| POST | `/characters/{characterId}` | Full CharacterModel. Preserves existing account, status, name, slot, numeric token, and creation metadata; detaches old record and marks submitted entity Modified. Returns 404 for zero/missing route ID. |
| POST | `/characters/{characterID}/deletion` | DeletionData below. Looks up the ID in the body. |
| POST | `/characters/{characterID}/token` | TokenData below. Looks up the ID in the body. |

Character mutation success envelope:

```json
{"success":true,"data":{"characterID":123},"message":""}
```

Creation returns the new ID. Other writes usually return only `success:true` with nullable envelope fields. Creation business failures still return HTTP 200 with `success:false` and a Portuguese message. Callers must check the envelope, not only the HTTP status.

DeletionData:

```json
{"CharacterID":123,"DeleteStatus":1,"DaleteTime":"2026-10-12T12:00:00"}
```

`DeleteStatus` 0 clears the scheduled date, 1 sets it, and 2 sets `CharacterStatus` to 0. The action does not itself enforce a waiting period. Unknown status values make no change but still return success. `DaleteTime` is the exact DTO spelling.

TokenData:

```json
{"CharacterID":123,"NumericToken":"1234","NumericErrors":0}
```

When NumericToken is null, its existing value is retained; NumericErrors is always updated. The route ID is not bound by either of these action signatures. Body IDs drive the mutation.

The character-save action does not explicitly reconcile the body's CharacterID with the route ID. Clients should send matching IDs and complete records: this is an entity replacement operation, not a partial patch.

## Character collections

Every collection POST accepts a JSON array of the model indicated below and returns the character mutation envelope. Primary key zero means Add; a nonzero key means Modified. These endpoints do not reconcile the route character ID with each body's CharacterID or enforce ownership. Omitting an existing record from the array does not delete it.

| Resource suffix after `/characters/{characterID}` | Methods | Model | GET selection/order |
| --- | --- | --- | --- |
| `/items` | GET, POST | ItemModel | ItemStatus > 0, ordered by InventorySlot. |
| `/items/{inventoryType}` | GET | ItemModel | Active items of that inventory type, ordered by slot. |
| `/skills` | GET, POST | SkillModel | SkillStatus > 0, ordered by SkillSlot. |
| `/hotbar` | GET, POST | HotBarModel | ItemStatus > 0, ordered by ItemSlot. |
| `/buffs` | GET, POST | BuffModel | BuffStatus > 0, descending BuffDuration. |
| `/titles` | GET, POST | TitleModel | TitleStatus > 0; writes refresh UpdatedOn. |
| `/quests` | GET, POST | CharacterQuestModel | IsActive > 0; writes refresh UpdatedOn. |

Example inventory write shape (use only against disposable development data):

```json
[
  {"CharacterItemID":0,"CharacterID":123,"ItemStatus":1,"InventoryType":1,"InventorySlot":0,"ItemListID":5100,"ItemAmount":1}
]
```

Fields omitted from this example receive their model defaults. Consult [model fields](model-fields.md) for complete entity contracts before saving real character state.

## Read-only local checks

With AikaWeb running at the documented endpoint:

```powershell
Invoke-RestMethod https://localhost:8090/servers/all
Invoke-RestMethod https://localhost:8090/nations/all
```

Use `/swagger` to inspect request schemas and routes. Token generation and session validation are state-changing operations even though their purpose is authentication.

Sources: `AikaWeb/Program.cs`, all four files in `AikaWeb/Controllers/`, `AikaWeb/Models/`, and `GameServer/WebHandler.cpp`.
