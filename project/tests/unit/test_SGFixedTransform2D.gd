extends "res://addons/gut/test.gd"

func test_deterministic_rotation():
	var t: SGFixedTransform2D

	# Make sure this gives an identity matrix.
	t = SGFixed.transform2d(0, SGFixed.vector2(0, 0))
	assert_eq(t.x.x, 65536)
	assert_eq(t.x.y, 0)
	assert_eq(t.y.x, 0)
	assert_eq(t.y.y, 65536)

	t = SGFixedTransform2D.new()
	t.x.x = 65535
	t.x.y = -366
	t.y.x = 366
	t.y.y = 65535
	assert_eq(t.get_rotation(), -364)

	var big_t = t.scaled(SGFixed.vector2(13107200, 13107200))
	var big_t_inv = big_t.affine_inverse()
	var rounda = big_t.xform(big_t_inv.xform(SGFixed.vector2(65536, 65536)))
	assert_almost_eq(rounda.x, 65536, 2000)
	assert_almost_eq(rounda.y, 65536, 2000)

	var small_t = t.scaled(SGFixed.vector2(131, 131))
	var small_t_inv = small_t.affine_inverse()
	var roundb = small_t.xform(small_t_inv.xform(SGFixed.vector2(65536, 65536)))
	assert_almost_eq(rounda.x, 65536, 2000)
	assert_almost_eq(rounda.y, 65536, 2000)

	t = SGFixedTransform2D.new()
	t.x.x = 65348
	t.x.y = -4956
	t.y.x = 4956
	t.y.y = 65348
	assert_eq(t.get_rotation(), -4937)

	t = SGFixed.transform2d(-290, SGFixed.vector2(0, 0))
	assert_eq(t.x.x, 65535)
	assert_eq(t.x.y, -290)
	assert_eq(t.y.x, 290)
	assert_eq(t.y.y, 65535)

	# Make sure this gives the same result on all platforms, even after
	# rotating a lot of times and accumulating a lot of error.
	t = SGFixed.transform2d(0, SGFixed.vector2(0, 0))
	for i in range(320):
		t = t.rotated(SGFixed.PI_DIV_4)
	assert_eq(t.x.x, 65536)
	assert_eq(t.x.y, 0)
	assert_eq(t.y.x, 0)
	assert_eq(t.y.y, 65536)
	assert_eq(t.get_rotation(), 0)
	var scale = t.get_scale()
	assert_eq(scale.x, 65536)
	assert_eq(scale.y, 65536)
