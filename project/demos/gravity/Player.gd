extends SGCharacterBody2D

@onready var color_rect = $ColorRect
@onready var debug_label = $CanvasLayer/DebugLabel

const SPEED = 196608
const GRAVITY = 32768
const JUMP = 65536*15

func _ready():
	up_direction = SGFixed.vector2(0, -65536)

func _physics_process(delta: float) -> void:
	var x_motion: int = SGFixed.from_float(Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left"))
	velocity.x = SGFixed.mul(x_motion, SPEED)

	if Input.is_action_just_pressed("ui_up"):
		velocity.y = -JUMP

	velocity.y += GRAVITY
	move_and_slide()

	debug_label.text = ""
	for slide_idx in get_slide_count():
		var col = get_slide_collision(slide_idx)
		debug_label.text += "[collider = %s, normal = %s, remainder = %s]\n" % [col.collider.name, col.normal.to_float(), col.remainder.to_float()]

	if is_on_floor():
		color_rect.color = Color(0.0, 0.0, 1.0)
		var floor_normal = get_floor_normal()
		#print ("Floor normal: (%s, %s)" % [floor_normal.x, floor_normal.y])
		#print ("Floor angle: %s" % SGFixed.to_float(get_floor_angle()))
	elif is_on_ceiling():
		color_rect.color = Color(1.0, 0.0, 1.0)
		print (" !! CEILING !!")
	elif is_on_wall():
		color_rect.color = Color(0.0, 1.0, 0.0)
	else:
		color_rect.color = Color(1.0, 1.0, 1.0)

