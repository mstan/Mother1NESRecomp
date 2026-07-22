# Mother1NESRecomp

> _This recompilation is a **byproduct of developing
> [nesrecomp](https://github.com/mstan/nesrecomp)** — the games are the proving ground, the framework is the goal.
> **These are in-development previews, not finished ports — expect rough
> edges**, and depth will keep landing over months, not days. My time for any
> one title is limited, so I ask for your patience. Contributions are welcome —
> testing, issues, and PRs to the game or framework all help and will
> accelerate this game's polish. More on the why at:
> [Recomp + AI: 5 Months Later »](https://1379.tech/recomp-ai-5-months-later/)_

> **Development status:** This is largely untested beyond its initial boot and
> basic-play bring-up. It was created at a community member's request as a base
> for another developer to continue, not as a finished or supported port.
> Expect incomplete behavior, bugs, and regressions.

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
