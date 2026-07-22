# Mother1NESRecomp

Static recompilation scaffold for Mother 1 / EarthBound Zero (NES) using
NESRecomp.

This project contains Mother-specific configuration and game hooks plus a
Dear ImGui launcher wired through the shared `recomp-ui` submodule. It does not
include a ROM or ROM-derived generated code.

## ROM

You must supply your own legally obtained ROM. The configured ROM has:

- Mapper: 4 / MMC3
- Payload CRC32: `00F0FBBC`
- Battery-backed SRAM: yes
- Controllers: one player

Generate the local, ignored `generated/` directory before configuring the
game build:

```bat
nesrecomp\build\recompiler\Release\NESRecomp.exe C:\path\to\your\game.nes --game game.toml
```

## Build

Fetch pinned submodules first:

```bat
setup.bat
```

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

## Third-party material

Mother, EarthBound Beginnings, Nintendo, and the associated game artwork and
assets are trademarks or copyrighted material of Nintendo and their respective
owners, including APE Inc. and Pax Softnica. This project is unofficial and is
not affiliated with or endorsed by Nintendo or those companies.

`boxart.tga` is adapted from the North American EarthBound Beginnings cover
used for Nintendo Switch Online. It is included only for identification and
launcher presentation. Source:
<https://images.launchbox-app.com/9085238f-b09f-4960-a251-61e3ebed990f.jpg>
