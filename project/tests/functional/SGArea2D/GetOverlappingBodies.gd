extends Node2D

@onready var area = $Area2D
@onready var static_body1 = $StaticBody1
@onready var static_body2 = $StaticBody2

func do_get_overlapping_bodies() -> Array:
	return area.get_overlapping_bodies()
