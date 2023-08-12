extends Character
class_name  Player

signal collected_item(item)

func _ready():
	pass 

func _process(delta):
	pass

func pick_up_item(item):
	collected_item.emit(item)
