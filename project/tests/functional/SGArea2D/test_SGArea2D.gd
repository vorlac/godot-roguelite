extends "res://addons/gut/test.gd"

func test_get_overlapping_bodies() -> void:
	var GetOverlappingBodies = load("res://tests/functional/SGArea2D/GetOverlappingBodies.tscn")

	var result := []

	# Run 5 times to attempt to check if this is deterministic.
	for i in range(5):
		var scene = GetOverlappingBodies.instantiate()
		add_child(scene)

		result = scene.do_get_overlapping_bodies()
		assert_eq(result.size(), 2)
		assert_eq(result[0], scene.static_body1)
		assert_eq(result[1], scene.static_body2)

		# Add/remove one of the bodies to change the order in the scene tree.
		scene.remove_child(scene.static_body1)
		scene.add_child(scene.static_body1)
		scene.static_body1.sync_to_physics_engine()

		result = scene.do_get_overlapping_bodies()
		assert_eq(result.size(), 2)
		assert_eq(result[0], scene.static_body2)
		assert_eq(result[1], scene.static_body1)

		remove_child(scene)
		scene.queue_free()

func test_get_overlapping_areas() -> void:
	var GetOverlappingAreas = load("res://tests/functional/SGArea2D/GetOverlappingAreas.tscn")

	var scene = GetOverlappingAreas.instantiate()
	add_child(scene)

	# Ensure only the monitorable area is returned.
	var result = scene.do_get_overlapping_areas()
	assert_eq(result.size(), 1)
	assert_eq(result[0], scene.area2)

	remove_child(scene)
	scene.queue_free()

func test_get_overlapping_area_shape_collisions() -> void:
	var GetOverlappingShapeCollisions = load("res://tests/functional/SGArea2D/GetOverlappingShapeCollisions.tscn")

	var scene = GetOverlappingShapeCollisions.instantiate()
	add_child(scene)

	var result: Array

	result = scene.detector_area1.get_overlapping_area_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_area)
	assert_eq(result[0].shape, scene.colliding_area_shape1)

	result = scene.detector_area2.get_overlapping_area_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_area)
	assert_eq(result[0].shape, scene.colliding_area_shape2)

	remove_child(scene)
	scene.queue_free()

func test_get_overlapping_body_shape_collisions() -> void:
	var GetOverlappingShapeCollisions = load("res://tests/functional/SGArea2D/GetOverlappingShapeCollisions.tscn")

	var scene = GetOverlappingShapeCollisions.instantiate()
	add_child(scene)

	var result: Array

	result = scene.detector_area1.get_overlapping_body_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_body)
	assert_eq(result[0].shape, scene.colliding_body_shape1)

	result = scene.detector_area2.get_overlapping_body_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_body)
	assert_eq(result[0].shape, scene.colliding_body_shape2)

	remove_child(scene)
	scene.queue_free()

func test_get_overlapping_area_polygon_collisions() -> void:
	var GetOverlappingPolygonCollisions = load("res://tests/functional/SGArea2D/GetOverlappingPolygonCollisions.tscn")

	var scene = GetOverlappingPolygonCollisions.instantiate()
	add_child(scene)

	var result: Array

	result = scene.detector_area1.get_overlapping_area_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_area)
	assert_eq(result[0].shape, scene.colliding_area_shape1)

	result = scene.detector_area2.get_overlapping_area_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_area)
	assert_eq(result[0].shape, scene.colliding_area_shape2)

	remove_child(scene)
	scene.queue_free()

func test_get_overlapping_body_polygon_collisions() -> void:
	var GetOverlappingPolygonCollisions = load("res://tests/functional/SGArea2D/GetOverlappingPolygonCollisions.tscn")

	var scene = GetOverlappingPolygonCollisions.instantiate()
	add_child(scene)

	var result: Array

	result = scene.detector_area1.get_overlapping_body_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_body)
	assert_eq(result[0].shape, scene.colliding_body_shape1)

	result = scene.detector_area2.get_overlapping_body_collisions()
	assert_eq(result.size(), 1)
	assert_eq(result[0].collider, scene.colliding_body)
	assert_eq(result[0].shape, scene.colliding_body_shape2)

	remove_child(scene)
	scene.queue_free()

