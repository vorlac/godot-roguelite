extends Test

func _on_test_custom_signal_example(delta_time):
	print("DeltaTime value sent from C++ to TEST::GDScript: ", delta_time)
