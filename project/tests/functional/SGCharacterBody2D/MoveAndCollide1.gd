extends Node2D

@onready var static_body1: SGStaticBody2D = $StaticBody1
@onready var static_body2: SGStaticBody2D = $StaticBody2
@onready var kinematic_body: SGCharacterBody2D = $KinematicBody
@onready var start_transform: SGFixedTransform2D = kinematic_body.fixed_transform.copy()

func reset_kinematic_body() -> void:
	kinematic_body.fixed_transform = start_transform
	kinematic_body.sync_to_physics_engine()

func do_move_and_collide() -> SGKinematicCollision2D:
	var vector = SGFixed.vector2(0, -SGFixed.TWO)
	return kinematic_body.move_and_collide(vector)

func _on_Button_pressed() -> void:
	reset_kinematic_body()
	do_move_and_collide()
