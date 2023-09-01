# Godot 4.1 GDExtension C++ Prototype
 This project is a rough prototype focusing on understanding how to implement most of the core logic of a game in C++ using GDExtension. Everything is done in the code aside from UI/dialog/menu design, and scene file generation. The current focus is just learning more about how to implement certain features / funtionality in the GDExtension library so there isn't much game logic. Since the documentation is very sparse when it comes to pure native development, a lot of what is implemnted in this side project is mostly focused on providing examples for a variety of misc functionality. Game logic will become a focus when most of the essentials are figured out through misc features/functionality.

***Disclaimer:*** I am very new to godot, I've only been using it for about 6 weeks at the time of writing this. There's a very good chance some of the design/implementation approaches used in this project are completely wrong. I just work through most feature additions until something works and stick with that until I find a better/cleaner way.

# High Level Project Structure

The project's main scene consists of a single `Main` node. The `Main` node is intended to handle all high level game management (level loading, scene swaps, saving, loading, UI, input handling, menus, signal propigation, etc)

The overall node heirarchy:

<div class="highlight highlight-html"> <pre>
<a href="./src/main.hpp" title="title">Main</a>
├── <a href="./src/ui/main_dialog.hpp" title="title">MainDialog</a>
│   ├── <a href="./project/assets/scenes/ui/dialogs/main_dialog.tscn" title="title">RichTextLabel (In-Game Console)</a>
│   └── <a href="./project/assets/scenes/ui/dialogs/main_dialog.tscn" title="title">MainSubViewport (Primary Game Viewport)</a>
│       └── <a href="./project/assets/scenes/ui/dialogs/main_dialog.tscn" title="title">Canvas Layer</a>
├── <a href="./src/core/level.hpp" title="title">Level</a>
│   ├── <a href="./src/nodes/character.cpp" title="title">Character</a>
│   │   ├── <a href="./src/nodes/camera.cpp" title="title">Camera</a>
│   │   ├── <a href="./src/nodes/character.cpp" title="title">Sprite2D</a>
│   │   └── <a href="./src/nodes/character.cpp" title="title">CollisionShape2D</a>
│   ├── <a href="./src/core/projectile_spawner.hpp" title="title">Projectile Spawner</a>
│   │   └── <a href="./src/nodes/projectile.cpp" title="title">Projectile</a>
│   ├── <a href="./src/core/level.hpp" title="title">Sprite2D (Background)</a>
└── <a href="./src/core/main.hpp" title="title">Main Canvas Layer (reference to Canvas Layer in MainDialog)</a>
</p>
<a href="./src/main.hpp" title="title">Console (Singleton)</a>
└── <a href="./project/assets/scenes/ui/dialogs/main_dialog.tscn" title="title">RichTextLabel (Reference from MainDialog)</a>
</pre> </div>

# Project Configuration
See the [wiki](https://github.com/vorlac/godot-roguelite/wiki) for development/tooling setup instructions
