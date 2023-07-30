extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func _on_character_move_speed_changed(node: Node, new_speed: float):
	print(node.name, " new speed: ", new_speed)
