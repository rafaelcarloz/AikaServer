# Project documentation

This documentation describes the source and runtime files in this checkout, analyzed on October 11, 2026. It covers the three server components: **AikaWeb**, **LoginServer (MainServer)**, and **GameServer**. Behavior is derived from code; a running database, game client, and end-to-end server session were not used to validate it.

## Guides

| Document | Contents |
| --- | --- |
| [Setup](../README.md#setup-instructions) | Prerequisites, package restore, database setup, builds, and startup. |
| [Architecture](architecture.md) | Repository map, component boundaries, startup, login, persistence, and threads. |
| [API reference](api.md) | Every controller route, request formats, response behavior, and compatibility details. |
| [Database](database.md) | Entity mappings, relationships, migration behavior, and active-session dependency. |
| [Model fields](model-fields.md) | Field-by-field reference extracted from all AikaWeb models. |
| [Game systems](game-systems.md) | Player lifecycle, combat, inventory, NPCs, quests, mobs, and implemented limits. |
| [Protocol](protocol.md) | Packet header, encryption, dispatch, and network behavior. |
| [Packet codes](packet-codes.md) | Numeric opcode inventory and whether a code is dispatched inbound. |
| [Content and Lua](content.md) | Runtime assets, load order, JSON formats, and scripting interface. |
| [Operations](operations.md) | Configuration, startup checks, shutdown, logging, backup, and troubleshooting. |
| [Development](development.md) | Source navigation, extension workflows, dependencies, and validation. |
| [Source inventory](source-inventory.md) | Source-file index for the three components. |

## Reading order

Start with setup and architecture. Use the API and database documents when changing persistence; use game systems, protocol, and content when changing gameplay. The operations guide records practical limits that affect deployment and recovery.

The repository contains a [GNU GPL version 3 license text](../LICENSE). Third-party packages and bundled libraries have their own license files.
