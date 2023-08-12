extends CanvasLayer
class_name HudOverlay 

@onready var score_label = %Score
@onready var reload_progress = %ReloadProgress


var score = 0:
	set(new_score):
		score = new_score
		_update_score_label()

func _ready():
	_update_score_label();

func _update_score_label():
	score_label.text = str(score)

func _on_collected(collectable) -> void:
	if collectable:
		score += 100;

func _on_reload_progress(progress) -> void:
	reload_progress.value = progress	

func _on_reloaded() -> void:
	reload_progress.value = 1
