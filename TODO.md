# TODO

### Stats / Resource Monitoring
* print resource usage (fps, frametime, object count, etc) in UI labels.
* truncate console text output once it exceeds ~1024 lines.

### Resource loading
* see if loading from path can be default rather than loading cached resource file.
    * cached files don't always play nice with certain native code changes (i.e. changing `GDCLASS` baseclass)

### Menu Dialogs / HUD
* create main menu
* create options menu
* create pause menu
* in-game HUD

### Level Creation
* look into procedural level generation
    * completely dynamic? pieced together using prebuilt scene chunks?

### Enemies
* implement placeholer dynamic enemy spawns
 * health
 * movement
 * attacks
 * weapons

### Projectiles
* design dynamic projectile system
    * custom paths
    * tweening
    * speed
    * size
    * acceleration
    * TTL
    * distance
    * auto tracking
    * rayscan / lazer
    * pattern
    * accuracy / randomness in spray

### Weapons
* implement weapon inventory
    * weapon swapping
    * weapon pickups / replacement
    * randomized properties?

### Items
* implement basic item pickup system
    * usable items
    * passive modifiers

### Score / Stats tracking
* player health
* score / kills
* survival time
* dungeon depth
* rooms cleared per dungeon / floor
* currency
    * apply toward perstent upgrades / weapons / skill tree

### Player Character
##### Inputs / Movement
* refine controls
##### Animations
* implement basic animations for movement / state change

### Graphics
* Look into shaders
* particle effects (projectiles? dashing?)

### Audio
* Hook up some basic sfx / music
