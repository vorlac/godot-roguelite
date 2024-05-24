extends Node2D

const FloatObject = preload("res://demos/fixed_node_2d_perf/FloatObject.tscn")
const FixedObject = preload("res://demos/fixed_node_2d_perf/FixedObject.tscn")

var min_timing := 1000000.0
var max_timing := 0.0
var avg_timing := 0.0
var count := 0

func _ready() -> void:
	var viewport_size = get_viewport().size
	
	# Create 100 objects.
	for i in range(100):
		var obj = FixedObject.instantiate()
		add_child(obj)
		obj.fixed_position = SGFixed.vector2(
			SGFixed.from_int(randi() % int(viewport_size.x)),
			SGFixed.from_int(randi() % int(viewport_size.y)))
		
#		var obj = FloatObject.instantiate()
#		add_child(obj)
#		obj.position = Vector2(
#			randi() % int(viewport_size.x),
#			randi() % int(viewport_size.y))

func _physics_process(delta: float) -> void:
	var timings := []
	for obj in get_children():
		var timing = obj.move_me()
		timings.append(timing)
	
	var sum := 0
	for timing in timings:
		sum += timing
	var avg = float(sum) / timings.size()
	
	min_timing = min(min_timing, avg)
	max_timing = max(max_timing, avg)
	
	avg_timing = ((avg_timing * count) + avg) / float(count + 1)
	count += 1
	
	print ("TOTAL: %s  |  AVG: %.02f  |  CULM. AVG: %.02f  |  MIN: %.02f  |  MAX: %.02f" % [sum, avg, avg_timing, min_timing, max_timing])
