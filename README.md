# Mother1NESRecomp

Static recompilation scaffold for Mother 1 / EarthBound Zero (NES) using
NESRecomp.

This project contains Mother-specific generated code, game hooks, and a
Dear ImGui launcher wired through the shared `recomp-ui` submodule. It does not
include a ROM or official box art.

## ROM

You must supply your own ROM. The currently configured ROM is:

- `E:\Downloads\EarthBound_Zero_Mother_1-u_202305.nes`
- Mapper: 4 / MMC3
- Payload CRC32: `00F0FBBC`
- Battery-backed SRAM: yes
- Controllers: one player

## Build

Fetch pinned submodules first:

```bat
setup.bat
```

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

To regenerate:

```bat
F:\Projects\nesrecomp\nesrecomp\build\recompiler\Release\NESRecomp.exe E:\Downloads\EarthBound_Zero_Mother_1-u_202305.nes --game game.toml
```
