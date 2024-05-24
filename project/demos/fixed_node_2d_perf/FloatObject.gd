extends Node2D

@onready var color_rect = $ColorRect

const SPEED = 10

var vector: Vector2

func _ready() -> void:
	color_rect.color = Color(randf(), randf(), randf(), 1.0)
	vector = Vector2(-1.0 + (randf() * 2.0), -1.0 + (randf() * 2.0)).normalized()
	vector *= SPEED

func move_me() -> int:
	var timing = Time.get_ticks_usec()
	position += vector
	timing = Time.get_ticks_usec() - timing
	
	var viewport_size = get_viewport().size
	if position.x < 0 or position.x > viewport_size.x:
		vector.x = -vector.x
	if position.y < 0 or position.y > viewport_size.y:
		vector.y = -vector.y
	
	return timing
