extends Node2D

@onready var player = $Player
@onready var tilemap = $TileMap

var velocity := SGFixed.vector2(0, 0)
var resources := []

func _ready():
	create_tilemap_collision(tilemap, [0, 3])

func _exit_tree():
	for rid in resources:
		SGPhysics2DServer.free_rid(rid)

func _physics_process(delta):
	velocity.y = 0
	velocity.x = 0
	if Input.is_action_pressed("ui_up"):
		velocity.y = SGFixed.NEG_ONE
	elif Input.is_action_pressed("ui_down"):
		velocity.y = SGFixed.ONE
	if Input.is_action_pressed("ui_left"):
		velocity.x = SGFixed.NEG_ONE
	elif Input.is_action_pressed("ui_right"):
		velocity.x = SGFixed.ONE
	if velocity.x != 0 or velocity.y != 0:
		player.velocity = velocity
		player.move_and_slide()

func create_tilemap_collision(tilemap: TileMap, colliding_tiles: Array) -> void:
	var width = int(tilemap.tile_set.tile_size.x)
	var height = int(tilemap.tile_set.tile_size.y)
	var extents = SGFixed.vector2(SGFixed.from_int(width / 2), SGFixed.from_int(height / 2))
	var shape_transform = SGFixed.transform2d(0, extents)

	for tile_id in colliding_tiles:
		for tile_pos in tilemap.get_used_cells_by_id(0, tile_id):
			var shape = SGPhysics2DServer.shape_create(SGPhysics2DServer.SHAPE_RECTANGLE)
			SGPhysics2DServer.rectangle_set_extents(shape, extents)
			SGPhysics2DServer.shape_set_transform(shape, shape_transform)
			resources.push_back(shape)

			var object = SGPhysics2DServer.collision_object_create(SGPhysics2DServer.OBJECT_BODY, SGPhysics2DServer.BODY_STATIC)
			SGPhysics2DServer.collision_object_add_shape(object, shape)
			SGPhysics2DServer.collision_object_set_transform(object, SGFixed.transform2d(0,
				SGFixed.vector2(SGFixed.from_int(int(tile_pos.x) * width), SGFixed.from_int(int(tile_pos.y) * height))))

			# In order for collisions to be deterministic, the physics engine needs
			# to be able to sort the objects. Normally, it sorts by scene tree order,
			# but when there is no node, we need to set a unique data string to use
			# for sorting. This can also be used to identify the object in some way.
			SGPhysics2DServer.collision_object_set_data(object, "tilemap-%sx%s" % [int(tile_pos.x), int(tile_pos.y)])
			resources.push_back(object)

			SGPhysics2DServer.world_add_collision_object(SGPhysics2DServer.get_default_world(), object)
