# TODO

### Console / Stats / Resource Monitoring
* print resource usage (fps, frametime, object count, etc) in UI labels.
* make `rl::utils::console` generic so it can write to any widget type
    * register any number of any widget type as a "console" object
* make `rl::utils::console` static initialized and threaded.
* add support for formatted/colored text & fixed width text style for console widgets
* truncate text output once it exceeds ~1024 lines.

### Resource loading
* look into common vulkan/shader exceptions on startup
    * related to resources built on different machines? different builds of editor?
* see if loading from path can be default rather than loading precached binary resource file.

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

# Resources

### Handle inputs properly while the editor is running and standalone
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-property-gui-disable-input
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-property-handle-input-locally
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-property-gui-disable-input
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-exclusive
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-mouse-passthrough
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-mouse-passthrough-polygon

### Window / viewport configuration
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-method-grab-focus
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-method-has-focus
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-method-is-embedded
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-min-size
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-size
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-property-world-2d
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-method-get-camera-2d
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-method-push-input
* https://docs.godotengine.org/en/stable/classes/class_viewport.html#class-viewport-method-push-unhandled-input

# In game menu UI
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-wrap-controls

# Editor mode enhancements:
* https://docs.godotengine.org/en/stable/classes/class_window.html#class-window-property-transient
