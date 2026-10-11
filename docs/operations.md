# Operations and configuration

[Documentation index](README.md) · [Setup](../README.md#setup-instructions)

## Configuration reference

| Location / setting | Current value | Effect |
| --- | --- | --- |
| AikaWeb appsettings: GameDbConnection | localhost / GameDatabase / integrated security / Encrypt=False | SQL Server connection. Development settings duplicate the connection string. |
| AikaWeb launch profile | HTTPS 7213, HTTP 5220 | IDE launch defaults; differs from native API client. |
| GameServer/WebHandler.h: _domain, _port, _https | localhost, 8090, true | Compiled endpoint shared by both native servers. Changing it requires rebuilding both. |
| MainServer.json: serverSettings.address | 127.0.0.1 | Login bind address. |
| MainServer.json: serverSettings.port | 8831 | Login TCP port. The duplicate `porta` field is not read by LoadConfiguration. |
| MainServer.json: version / maxConnections | 100 / 10 in source configuration | Loaded values; accept/login paths do not demonstrate enforcement. Runtime copies can differ. |
| GameServer.json: version / maxConnections | 100 / 10 | Loaded values with the same enforcement limitation. |
| Both JSON files: internalServerSettings | 127.0.0.1:8800 | Present in configuration; inspected loaders do not read this section. |
| Servers database rows | Per row | GameServer address/port, server type, and nation. Only ServerStatus > 0 rows are launched. |
| GameServer default launch | 127.0.0.1:8822, nation 1, type 1 | Used when no launch argument is supplied. |

Runtime JSON in `bin/` is separate from the source copies. Verify the files actually read by the process. Only Debug x64 explicitly sets native OutDir to the shared bin/ directory; other build configurations need their output layout checked before running.

GameServer labels server type 1 as PvP and other values as PvE in its console title. This label alone is not evidence that every PvP/PvE rule is implemented.

## Startup checks

1. Restore/provision SQL Server and verify the connection account can query the required tables and active-session object.
2. Verify Servers and Nations contain the expected IDs, statuses, addresses, and distinct ports. Native nation parsing assumes several nullable database fields can be converted to concrete values; inspect the returned record if initialization fails.
3. Start AikaWeb at HTTPS localhost:8090 using the [setup command](../README.md#5-start-aikaweb).
4. Check `/swagger`, `/servers/all`, and `/nations/all`. A successful list endpoint proves neither token resolution nor character loading; check those with development data separately.
5. Build the native executables, preserve runtime assets, and run MainServer from bin/.
6. Check the login socket log and each game instance's content counts, socket endpoint, and nation title.

For port checks in PowerShell:

```powershell
Test-NetConnection localhost -Port 8090
Test-NetConnection 127.0.0.1 -Port 8831
# Replace 8822 with an actual Servers row's port.
Test-NetConnection 127.0.0.1 -Port 8822
```

These establish reachability, not protocol correctness. They also create short-lived native client connections when checking login/game ports.

## Launch scripts

`bin/startup.bat` starts AikaWeb/Debug/AikaWeb at `https://127.0.0.1:8090`, waits two seconds, then starts MainServer. It assumes the current directory is bin/ and the API has already been built there. The script has no API readiness check. The setup guide uses `localhost` to match WebHandler and the development certificate name.

MainServer's launch command is built as a shell string without robust path/argument escaping. Checkout paths containing spaces and unusual server data can affect process launch. Check that each expected process actually starts; launch the individual GameServer manually for diagnosis if necessary.

## Logging

Native Logger prints timestamps and colored categories to the console: Status, ConnectionsTrafic, Packets, Warnings, Error, and Debug. Unknown game packets can be dumped as hexadecimal. No persistent rotating runtime log implementation was found in Logger.cpp; build log files under x64/ are compiler output.

AikaWeb configures Debug-level logging in the checked-in settings. MainServer logs accepted login tokens, and GameServer logs rejected tokens. Treat captured console logs as potentially containing credentials/session data.

## Shutdown and saves

The GameServer console callback explicitly handles `CTRL_CLOSE_EVENT` and invokes Close. Close requests player disconnection/saving, then marks the server inactive and closes the listening socket. It does not implement a coordinated join of all detached worker threads, and no equivalent explicit Ctrl+C/Ctrl+Break handling appears in that callback.

MainServer has a Close method but no shutdown handler registered in Program.cpp. MainServer does not manage termination of already launched GameServer processes. Do not assume closing the launcher closes game instances.

Keep AikaWeb and SQL Server running while disconnecting players and saving game state. For a controlled maintenance window, disconnect clients, confirm saves and game process termination, then stop MainServer/AikaWeb as appropriate. Forced termination can skip persistence, and a console-close callback is not a guaranteed transactional shutdown.

## Recovery and backup

Back up SQL Server through SQL Server tooling and preserve matching bin/ content/settings. Verify backups by restoring to a separate development database. The checked-in backup is a starting artifact, not an automatically updated copy of current runtime data.

After a partial save or crash, compare character and child records rather than assuming the full snapshot committed. There is no inspected retry queue, transaction spanning all character calls, automated restore, or process supervisor.

## Troubleshooting matrix

| Symptom | Inspect |
| --- | --- |
| Missing v142 or NuGet import | VS installer components, native package restores, root packages/ path. |
| LuaBridge header missing | GameServer include directory `$(SolutionDir)lib`. |
| API works in browser but native server fails | HTTPS host/port/certificate, WebHandler.h compiled values, WinHTTP response/error logs. |
| Token created but login lookup fails | Actual vwSessionsActive definition/data; migration-only provisioning does not populate it. |
| No game process launched | Servers rows/status, `/servers/all`, API availability, process-launch path quoting. |
| Bind failure | Configured address, occupied port, duplicate server rows/ports, socket log's Windows error number. |
| Nation parsing failure | Missing record or null marshal/alliance values expected as non-null by SetNationData. |
| Missing JSON config | Current working directory and bin/ configuration copies. |
| Empty/missing game content | Executable-relative asset paths, file counts, required binaries, Lua registration globals. |
| Intermittent packet issues | TCP split/coalesced reads, body-size checks, fixed buffers, and partial sends. |
| State lost on exit | API/database availability during saves, SaveCharacter's failing stage, forced shutdown. |

## Deployment boundary

The checkout describes a Windows development environment. It contains no verified container/Linux deployment path, automated service installation, or public-hosting configuration. Current code compares stored passwords directly, derives tokens from session IDs, exposes mutation endpoints without authorization attributes, and allows development commands without role checks. Those concrete behaviors need to be addressed before treating it as a protected public service; this documentation does not claim production readiness.
