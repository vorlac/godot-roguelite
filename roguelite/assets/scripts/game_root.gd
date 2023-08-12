extends Node2D
class_name Game

@export var player: Player
@export var hud: HudOverlay

func _ready():
	if not player.collected_item.is_connected(hud._on_collected):
		player.collected_item.connect(hud._on_collected)
