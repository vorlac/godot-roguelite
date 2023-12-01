# Godot 4.2 GDExtension C++ Prototype

This project is a rough prototype focusing on understanding how to implementa majority of a game's core logic/scripting in C++ using GDExtension. Everything is done in the code aside from UI/dialog/menu design, and scene file generation.

The current focus is aimed towards learning more about how certain features and funtionality can be implemented in native code leveraging the [godot-cpp](https://github.com/godotengine/godot-cpp) GDExtension bindings so there isn't much game logic implemented yet. Since the documentation is pretty sparse when it comes to pure native development, most of the functionality implemnted in this project is focused on providing a variety of general examples of misc functionality that others will likely find useful.

Game logic will become a focus when most of the essentials are figured out and implented throughout the project.

# Overview

This project is meant to show others working with GDExtension how to:

- structure a project
- implement specific features and functionality entirely in C++
- incorporate 3rd party libraries into their GDExtension code/library
- use CMake, VCPKG, and submodules to configure the project automatically
- configure VSCode (linux & windows) / Visual Studio 2022 (windows) for
  - good code navigation for godot engine sources, godot-cpp sources, and the gdextension project sources.
  - good autocomplete / intellisense configuration
  - preconfigured debugger launch profiles to run the project standalone or by launching the editor (both launch settings debuggable)
  - good debugger visualizations for internal godot data structures by leveraging the .natvis file provided with godot-cpp

# High Level Project Structure

The project's [main scene](./project/main.tscn) consists of a single `Main` node. The `Main` node is intended to handle all high level game management (level loading, scene swaps, saving, loading, UI, input handling, menus, signal propigation, etc)

The overall node hierarchy:

<div class="highlight highlight-html"> <pre>
<a href="./src/main.hpp" title="title">Main</a>
├── <a href="./src/ui/main_dialog.hpp" title="title">Main Canvas Layer (reference to Canvas Layer in MainDialog)</a>
├── <a href="./src/ui/main_dialog.hpp" title="title">MainDialog</a>
│   ├── <a href="./project/scenes/ui/main_dialog.tscn" title="title">RichTextLabel (In-Game Console)</a>
│   └── <a href="./project/scenes/ui/main_dialog.tscn" title="title">MainSubViewport (Primary Game Viewport)</a>
│       └── <a href="./project/scenes/ui/main_dialog.tscn" title="title">Canvas Layer</a>
├── <a href="./src/entity/level.hpp" title="title">Level</a>
│    ├── <a href="./src/entity/character/player.cpp" title="title">Player</a>
│    │   ├── <a href="./project/scenes/characters/player.tscn" title="title">(Scene)</a>
│    │   ├── <a href="./src/entity/controller/character_controller.cpp" title="title">CharacterController</a>
│    │   ├── <a href="./src/entity/camera.cpp" title="title">Camera</a>
│    │   └── <a href="./src/entity/character/character.hpp" title="title">FiringPoint (Marker2D)</a>
│    └── <a href="./src/entity/projectile/projectile_spawner.hpp" title="title">Projectile Spawner</a>
│        └── <a href="./src/entity/projectile/projectile.hpp" title="title">Projectile</a>
<a href="./src/singletons/console.hpp" title="title">Console (Singleton)</a>
└── <a href="./project/scenes/ui/main_dialog.tscn" title="title">RichTextLabel (Reference from MainDialog)</a>
</pre> </div>

# Setup Guide

See the [wiki](https://github.com/vorlac/godot-roguelite/wiki) for a detailed guide that will walk you through the steps required to get this project running and debugging on linux or windows.

# Contributing

This repo and/or the [gdextension developer documentation](https://github.com/vorlac/gdextension-docs/blob/main/main.md) I recently started working on is open to anyone that wants to contribute. Feel free to submit PRs for any changes that implement new features, fix bugs, or improve upon any existing features, functionality, or design.

# Additional Resources

The projects below are also worth checking out for some additional GDExtension examples to use as references

- https://github.com/paddy-exe/GDExtensionSummator
- https://github.com/asmaloney/GDExtensionTemplate
- https://github.com/godotengine/godot-cpp/tree/master/test/src

# Licensing

#### Code

- All code in this project is licensed as [MIT](./LICENSE)

#### Assets

- All art assets (`project/assets/art/*`) are licensed as [Creative Commons Zero, CC0](./project/assets/art/LICENSE)
- All audio assets (`project/assets/audio/*`) are licensed as [Creative Commons Zero, CC0](./project/assets/audio/LICENSE)
- Art & audio assets credit: [Kenney](www.kenney.nl)
- Asset pack purchase link: [All-in-1 Asset Pack](https://kenney.itch.io/kenney-game-assets)
