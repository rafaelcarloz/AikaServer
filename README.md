# AikaServer

AikaServer is organized into three subprojects that handle database access, player login, and gameplay.

## Documentation

See the [full project documentation](docs/README.md) for architecture, the complete API and database references, game systems, network packets, runtime content and Lua, operations, and development workflows.

## Subprojects

| Subproject | Responsibility |
| --- | --- |
| **AikaWeb** | Web API for database handling. |
| **LoginServer** | Server that handles player login. |
| **GameServer** | Server that handles game logic and player interactions. |

### AikaWeb

AikaWeb provides a web API for database operations. It contains the project's database-handling functionality.

### LoginServer

LoginServer handles player login and contains the functionality responsible for players signing in.

In this repository, LoginServer is named **MainServer**: its source is in `MainServer/`, and its executable is `MainServer.exe`.

### GameServer

GameServer handles gameplay, including game logic and player interactions.

## Development

Choose the subproject that matches the functionality you want to work on:

- **Database handling:** AikaWeb
- **Player login:** LoginServer
- **Gameplay and player interactions:** GameServer

## Setup instructions

These steps describe a local Windows setup. Run commands from the repository root unless a step says otherwise.

### 1. Install prerequisites

- Visual Studio 2022 with the **Desktop development with C++** and **ASP.NET and web development** workloads.
- The **MSVC v142 (VS 2019) C++ x64/x86 build tools** and a Windows 10 SDK, available through Visual Studio Installer. The native projects use v142 for x64 builds.
- The **.NET 6 SDK**, matching AikaWeb's `net6.0` target.
- **SQL Server** and **SQL Server Management Studio (SSMS)** for restoring and managing the database.
- **NuGet CLI** on your PATH if using the package restore commands below.

### 2. Restore dependencies and build

Restore the native packages and AikaWeb dependencies:

```powershell
nuget restore .\MainServer\packages.config -PackagesDirectory .\packages
nuget restore .\GameServer\packages.config -PackagesDirectory .\packages
dotnet restore .\AikaWeb\AikaWeb.csproj
```

The native package definitions include Boost, nlohmann JSON, and Lua. Keep the versions specified by each project; MainServer and GameServer use different Boost versions.

Open `AikaServer.sln` in Visual Studio and select **Debug | x64**. In **GameServer > Properties > C/C++ > General > Additional Include Directories**, add `$(SolutionDir)lib` while preserving inherited entries. This allows the compiler to find the bundled `LuaBridge/LuaBridge.h`; the project currently does not specify this include path.

Build **GameServer** and **MainServer**. This configuration writes both executables to `bin/`, alongside the runtime data. AikaWeb can be run separately using the command in step 5.

### 3. Set up the database

`Database/GameDatabase` is a SQL Server backup file with no extension. In SSMS:

1. Connect to your SQL Server instance.
2. Select **Databases > Restore Database > Device**, then add the backup file. Select **All Files** if the file picker hides it.
3. Restore it as **GameDatabase**. On the **Files** page, choose data and log locations valid for your SQL Server instance. The SQL Server service must be able to read the backup location.
4. Check the restored `Servers` and `Nations` records. Each server needs a usable address, a unique port, a server type, and a matching nation.

For an empty database instead, the repository includes an Entity Framework migration:

```powershell
Set-Location .\AikaWeb
dotnet tool restore
dotnet ef database update
Set-Location ..
```

Configure the connection string in step 4 before running this alternative. The migration creates the schema but does not seed gameplay data. It also creates `vwSessionsActive` as an empty table without a view definition or population mechanism; token resolution needs additional database logic. See the [database guide](docs/database.md#active-session-object) before choosing this path. Populate the required server, nation, and account records before using the servers.

### 4. Configure database access and server settings

AikaWeb reads `ConnectionStrings:GameDbConnection`. Both `AikaWeb/appsettings.json` and `AikaWeb/appsettings.Development.json` currently contain:

```text
Data Source=localhost;Initial Catalog=GameDatabase;Integrated Security=True;Encrypt=False
```

Update the instance name and authentication settings to match your database. For example, SQL Server Express commonly uses `localhost\SQLEXPRESS` (escape the backslash as `\\` inside JSON). With integrated security, the Windows account running AikaWeb needs access to GameDatabase. Keep both configuration files consistent, or override the connection string in the PowerShell session used to run AikaWeb:

```powershell
$env:ConnectionStrings__GameDbConnection = 'Data Source=localhost;Initial Catalog=GameDatabase;Integrated Security=True;Encrypt=False'
```

Review the runtime configuration files in `bin/`:

- `MainServer.json`: login address and port, client version, and maximum connections. The checked-in login endpoint is `127.0.0.1:8831`.
- `GameServer.json`: client version and maximum connections. GameServer's listening address and port come from the server data passed by MainServer.

MainServer and GameServer share the API client in `GameServer/WebHandler.h`, which targets **https://localhost:8090**. Use that endpoint when starting AikaWeb. The default Visual Studio launch profile uses different ports (`7213` and `5220`).

### 5. Start AikaWeb

Trust the local development HTTPS certificate, then start the API:

```powershell
dotnet dev-certs https --trust
dotnet run --project .\AikaWeb\AikaWeb.csproj --no-launch-profile -- --urls https://localhost:8090
```

Keep this terminal open. Visit `https://localhost:8090/swagger` and check `GET /servers/all` before starting MainServer. The response must contain the server records you intend to launch.

### 6. Start LoginServer and GameServer

In another PowerShell terminal, run MainServer from the `bin/` directory:

```powershell
Set-Location .\bin
.\MainServer.exe
```

MainServer requests `/servers/all` from AikaWeb and launches `GameServer.exe` once for each returned server record. Keep both executables in the same directory.

Run the native servers with `bin/` as their working directory: JSON configuration uses the working directory, while GameServer assets are resolved relative to the executable directory. Keep both executables, `lua.dll`, and the `Data/`, `InitialCharacters/`, `NPCs/`, `Mobs/`, `Drops/`, and `Scripts/` directories in place.

For an individual GameServer during development, run this from `bin/` instead of launching the same instance through MainServer:

```powershell
.\GameServer.exe
```

Without arguments, GameServer uses `127.0.0.1:8822`, nation ID `1`, and server type `1`. The corresponding nation must exist in the database.

### Troubleshooting

- **Missing v142 toolset or NuGet imports:** install the v142 build tools and restore both native package lists into the root `packages/` directory.
- **Database connection errors:** check the SQL Server instance, the effective connection string, and the permissions of the account running AikaWeb.
- **API connection errors or no GameServer instances:** confirm AikaWeb is listening on HTTPS port `8090`, its certificate is trusted, and `/servers/all` returns usable records.
- **Configuration or game data loading errors:** start the native executables from `bin/` and verify the runtime files are present.
- **Socket bind errors:** check that the configured login port and database server ports are available and distinct.
