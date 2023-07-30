extends Node2D


func _on_character_position_changed(node: Node, new_position: Vector2):
	print(node.name, " position: ", new_position)
