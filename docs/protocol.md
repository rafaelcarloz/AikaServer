# Native network protocol

[Documentation index](README.md) · [Packet code inventory](packet-codes.md)

## Transport and layout

MainServer and GameServer use Windows Winsock IPv4 TCP sockets. `GameServer/ServerSocekt.cpp` implements socket startup, binding, listening, accepting, and closing. Its filename is spelled `ServerSocekt.cpp` in the project.

Packets and character data use `#pragma pack(push,1)`. Handlers reinterpret received bytes as C++ structures rather than decoding a portable schema. Multi-byte integers use the native Windows representation (little-endian on the supported x64 setup). Preserve packing, field sizes, enum widths, and fixed array sizes when changing wire data.

## Packet header

`PacketHeader` is 12 bytes:

| Offset | Bytes | Field | Type |
| --- | --- | --- | --- |
| 0 | 2 | Size | WORD |
| 2 | 1 | Key | BYTE |
| 3 | 1 | ChkSum | BYTE |
| 4 | 2 | Index | WORD |
| 6 | 2 | Code | PacketCode backed by WORD |
| 8 | 4 | Time | DWORD |

`PacketSignalData` appends a DWORD to this header. The shared default server/client ID constant is `0x7535`. Packet definitions live in `Packets.h`, while nested data types come from PlayerData.h, ItemData.h, and MiscData.h.

## Encryption and dispatch

`Encryptation.cpp` supplies the project-specific encode/decode routines and a fixed 512-byte table. Incoming bytes are decrypted before the packet-code switch. Outgoing structs are copied and encrypted before `send`. GameServer's send path clears the two bytes at offset 2 and four bytes at offset 8 before encoding.

This is the existing client-compatible packet transformation, not a TLS channel. HTTPS protects the native-to-AikaWeb link separately.

MainServer dispatches code `0x81` to token validation and returns `0x82` with account index, tick time, and nation. GameServer dispatches the cases listed in [packet-codes.md](packet-codes.md); many additional codes are definitions only. Unknown GameServer codes are logged and dumped when LOG_PACKETS is enabled. Header-size mismatch currently has no active rejection logic in GameServer's dispatcher.

## Receive behavior and limits

Both client receive paths use an 8,000-byte stack buffer and one blocking `recv` per iteration. Reads under 12 bytes are ignored after incrementing the packet counter. MainServer strips an initial four-byte prefix on its first large read (>1116 bytes); GameServer uses >60 bytes for this special case.

The inspected receive paths do not implement persistent TCP-frame accumulation or a loop over multiple coalesced packets. TCP can split or merge writes, so a receive call is not guaranteed to correspond to a complete application packet. Most handlers cast directly to the target struct without checking that the complete body is available.

Outgoing send paths make a single send call and treat any positive result as success; they do not complete partially sent buffers. These are concrete limits to consider when diagnosing intermittent protocol failures or adding new handlers.

## Login packet examples

| Structure | Content after header |
| --- | --- |
| CheckTokenPacket | Username[32], Token[32], reserved bytes[1040]. |
| ResponseLoginPacket | DWORD account index, time, nation, and reserved DWORD. |
| PacketLoginRequest | AccountIndex, Username[32], Time, MacAddress[14], Version, reserved fields, Token[32], and additional reserved bytes. |

Use the full structures in Packets.h for client compatibility. Fixed char arrays may not be terminated by untrusted received data; their size is not permission to read beyond the array.

## Extending a packet

1. Choose or verify the client opcode and exact field layout.
2. Add/update the packed structure and PacketCode entry.
3. Validate header/body length, indexes, state, and referenced entities before reading fields or mutating state.
4. Add the PacketControl dispatch case and subsystem handler.
5. Encode responses through the existing send helpers.
6. Validate with a compatible client and partial/coalesced TCP inputs; update this documentation and the opcode inventory.

Sources: `GameServer/Packets.h`, `Encryptation.cpp`, `Player.cpp`, `PacketHandler.cpp`, `ServerSocekt.cpp`, `MainServer/PacketHandler.cpp`, and `MainServer/ClientConnection.cpp`.
