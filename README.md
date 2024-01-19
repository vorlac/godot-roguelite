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
<a href="./src/main.hpp" title="Main">Main</a>
├── <a href="./src/ui/main_dialog.hpp" title="main_dialog.hpp">Main Canvas Layer<a> (reference to <a href=https://github.com/vorlac/godot-roguelite/blob/main/project/scenes/ui/main_dialog.tscn#L100-L113 title="title">Canvas Layer</a> in MainDialog)
├── <a href="./src/ui/main_dialog.hpp" title="main_dialog.hpp">MainDialog</a>
│   ├── <a href="https://github.com/vorlac/godot-roguelite/blob/main/project/scenes/ui/main_dialog.tscn#L132-L159" title="main_dialog.tscn">RichTextLabel (In-Game Console)</a>
│   └── <a href="https://github.com/vorlac/godot-roguelite/blob/main/project/scenes/ui/main_dialog.tscn#L100-L113" title="main_dialog.tscn">MainSubViewport (Primary Game Viewport)</a>
│       └── <a href="https://github.com/vorlac/godot-roguelite/blob/main/project/scenes/ui/main_dialog.tscn#L100-L113" title="main_dialog.tscn">Canvas Layer</a>
├── <a href="./src/entity/level.hpp" title="level.hpp">Level</a>
│    ├── <a href="./src/entity/character/player.cpp" title="player.cpp">Player</a> (GDCLASS object inheritance example)
│    │   ├── <a href="./project/scenes/characters/player.tscn" title="player.tscn">Godot Player Scene</a>
│    │   ├── <a href="./src/entity/controller/character_controller.cpp" title="character_controller.cp">CharacterController</a>
│    │   ├── <a href="./src/entity/camera.cpp" title="camera.cpp">Camera</a>
│    │   └── <a href="https://github.com/vorlac/godot-roguelite/blob/main/src/entity/character/character.hpp#L85" title="character.hpp">FiringPoint (Marker2D reference from Godot Player Scene)</a>
│    └── <a href="./src/entity/projectile/projectile_spawner.hpp" title="projectile_spawner.hpp">Projectile Spawner</a>
│        └── <a href="./src/entity/projectile/projectile.hpp" title="projectile.hpp">Projectile</a>
<a href="./src/singletons/console.hpp" title="console.hpp">Console (Singleton example)</a>
└── <a href="https://github.com/vorlac/godot-roguelite/blob/main/project/scenes/ui/main_dialog.tscn#L132-L159" title="main_dialog.tscn">RichTextLabel (Reference from MainDialog)</a>
</pre> </div>

# Setup Guide

See the [wiki](https://github.com/vorlac/godot-roguelite/wiki) for a detailed guide that will walk you through the steps required to get this project running and debugging on linux or windows. Make sure to thank [PixelRain](https://github.com/Mangonels) in the [Godot discord](https://discord.com/channels/212250894228652034/342047011778068481) for his contributions to the wiki.

# Using this project as a base for your project

This repo is set up as a template (using the "use this template"->"create a new repository" button on the main landing page of this repo). If you use that option rather than just cloning this repo directly it will create a copy of the repo (not a clone or fork) in your github account with all history stripped. This project should be relatively plug and play when it comes to reusing the parts you want to keep and removing/replacing the parts you don't. If you create a repo from the template you also have the option of making the project private when it creates the repo in your github account. 

Once the template project is created you _should_ be able to just swap out the code in `src/` and godot project files in `project/` with your own C++ source code and godot project files. If everything was replaced correctly/consistently, the cmake build should detect the changes in the `src/` directory (file additions/removals/renames) and know to reconfigure the project if/when changes are made.

More information about using this repo as a base for your own project in the [templating](https://github.com/vorlac/godot-roguelite/wiki/Templating-%E2%80%90-Referencing-godot%E2%80%90roguelite-as-a-template-for-your-own-project) section of the wiki. 

# Note about cloning

If you just want to clone/try this project locally, it's important that you truly clone the repo and not just download it as a zip. The repo contains submodules that are required for the build to function properly, which will only be included if the repo is cloned (or templated). The zip download option won't correctly package the repos located in each extern/<submodule> directory which will lead to the buildscripts failing. 

# Contributing

This repo and/or the [gdextension developer documentation](https://github.com/vorlac/gdextension-docs/blob/main/main.md) I recently started working on is open to anyone that wants to contribute. Feel free to submit PRs for any changes that implement new features, fix bugs, or improve upon any existing features, functionality, or design.

# Additional Resources

The projects below are also worth checking out for some additional GDExtension examples to use as references

- https://github.com/paddy-exe/GDExtensionSummator
- https://github.com/asmaloney/GDExtensionTemplate
- https://github.com/godotengine/godot-cpp-template
- https://github.com/godotengine/godot-cpp/tree/master/test/src

# Licensing

#### Code

- All code in this project is licensed as [MIT](./LICENSE)

#### Assets

- All art assets (`project/assets/art/*`) are licensed as [Creative Commons Zero, CC0](./project/assets/art/LICENSE)
- All audio assets (`project/assets/audio/*`) are licensed as [Creative Commons Zero, CC0](./project/assets/audio/LICENSE)
- Art & audio assets credit: [Kenney](www.kenney.nl)
- Asset pack purchase link: [All-in-1 Asset Pack](https://kenney.itch.io/kenney-game-assets)
