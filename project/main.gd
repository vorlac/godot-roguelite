extends Node


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var rng := RandomNumberGenerator.new()
	var loaded_sprite_image := preload("res://icon.svg")
	
	for i in 2:
		var new_character: CPPNPC = CPPNPC.new()
		var sprite: Sprite2D = Sprite2D.new()
		sprite.texture = loaded_sprite_image
		new_character.add_child(sprite)
		self.add_child(new_character)
		new_character.position = Vector2(rng.randi_range(0, 600), rng.randi_range(0, 300))
		
	print("DONE")


func _physics_process(delta):
	print("_process Main!")
