extends Node2D

const FixedObject = preload("res://demos/broadphase_perf/FixedObject.tscn")

var avg_timings := {}
var count := 0

func _ready() -> void:
	randomize()
	
	avg_timings['query_physics'] = 0.0
	avg_timings['change_position'] = 0.0
	avg_timings['update_physics'] = 0.0
	
	var viewport_size = get_viewport().size
	
	# Create 100 objects.
	for i in range(100):
		var obj = FixedObject.instantiate()
		obj.fixed_position = SGFixed.vector2(
			SGFixed.from_int(randi() % int(viewport_size.x)),
			SGFixed.from_int(randi() % int(viewport_size.y)))
		add_child(obj)
		
#		var obj = FloatObject.instantiate()
#		add_child(obj)
#		obj.position = Vector2(
#			randi() % int(viewport_size.x),
#			randi() % int(viewport_size.y))

func print_timings(timings: Array, type: String, prefix: String) -> void:
	var sum := 0
	for timing in timings:
		sum += timing
	var avg = float(sum) / timings.size()
	
	var avg_timing = avg_timings[type]
	avg_timing = ((avg_timing * count) + avg) / float(count + 1)
	avg_timings[type] = avg_timing
	
	print ("%s %s -- TOTAL: %s  |  AVG: %.02f  |  CULM. AVG: %.02f" % [prefix, type, sum, avg, avg_timing])
	
func _physics_process(delta: float) -> void:
	var query_physics_timings := []
	var change_position_timings := []
	var update_physics_timings := []
	
	var timings := []
	for obj in get_children():
		var timing = obj.move_me()
		query_physics_timings.append(timing[0])
		change_position_timings.append(timing[1])
		update_physics_timings.append(timing[2])
	
	print (" ----- ")
	print_timings(query_physics_timings, 'query_physics', '(1)')
	print_timings(change_position_timings, 'change_position', '(2)')
	print_timings(update_physics_timings, 'update_physics', '(3)')
	count += 1
