extends SGFixedNode2D

@onready var color_rect = $ColorRect

const SPEED = SGFixed.ONE * 10

var vector: SGFixedVector2

func _ready() -> void:
	color_rect.color = Color(randf(), randf(), randf(), 1.0)
	vector = SGFixed.vector2(SGFixed.NEG_ONE + (randi() % SGFixed.TWO), -65536 + (randi() % SGFixed.TWO)).normalized()
	vector.imul(SPEED)

func move_me() -> int:
	var timing = Time.get_ticks_usec()
	fixed_position.iadd(vector)
	timing = Time.get_ticks_usec() - timing
	
	var viewport_size = get_viewport().size
	if fixed_position.x < 0 or fixed_position.x > SGFixed.from_int(viewport_size.x):
		vector.x = -vector.x
	if fixed_position.y < 0 or fixed_position.y > SGFixed.from_int(viewport_size.y):
		vector.y = -vector.y
	
	return timing
