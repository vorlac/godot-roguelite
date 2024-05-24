extends "res://addons/gut/test.gd"

func test_normalize():
	var v: SGFixedVector2
	
	# Normal kind of vector
	v = SGFixed.vector2(65536, 65536).normalized()
	assert_eq(v.x, 46341)
	assert_eq(v.y, 46341)
	assert_eq(v.length(), 65536)
	
	# Super tiny X.
	v = SGFixed.vector2(1, 0).normalized()
	assert_eq(v.x, 65536)
	assert_eq(v.y, 0)
	assert_eq(v.length(), 65536)
	
	# Super tiny Y.
	v = SGFixed.vector2(256, 0).normalized()
	assert_eq(v.x, 65536)
	assert_eq(v.y, 0)
	assert_eq(v.length(), 65536)
	
	# Super tiny X, with super huge Y.
	v = SGFixed.vector2(255, 45000000).normalized()
	assert_eq(v.x, 0)
	assert_eq(v.y, 65536)

	# Super tiny Y, with super huge X.
	v = SGFixed.vector2(45000000, 255).normalized()
	assert_eq(v.x, 65536)
	assert_eq(v.y, 0)

	# Super tiny X, with Y just on the edge of being super huge.
	v = SGFixed.vector2(255, 1482909).normalized()
	assert_gt(v.y, 0, "Y axis of the vector flipped")
	assert_gt(v.x, 0, "X axis of the vector flipped")
	assert_eq(v.x, 11)
	assert_eq(v.y, 65535)
	assert_eq(v.length(), 65535)
	
	# Smallish values that (due to imprecision) resist normalization.
	v = SGFixed.vector2(334, -667).normalized()
	assert_true(v.is_normalized())
