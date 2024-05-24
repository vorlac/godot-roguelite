extends Node2D


@onready var area1 = $Area2D1
@onready var area2 = $Area2D2
@onready var area3 = $Area2D3

func do_get_overlapping_areas() -> Array:
	return area1.get_overlapping_areas()
