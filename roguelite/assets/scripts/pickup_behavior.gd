extends Area2D
class_name Pickup

func _on_body_entered(body):
	if body is Player:
		body.pick_up_item(self)
		queue_free()
