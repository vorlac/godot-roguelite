extends "res://addons/gut/test.gd"

func test_float_conversion():
	var a: float = 2.4
	var b: int = SGFixed.from_float(a)
	var c: float = SGFixed.to_float(b)

	assert_eq(b, 157286)
	assert_eq("%.6f" % c, "2.399994")

func test_int_conversion():
	var a: int = 2
	var b: int = SGFixed.from_int(a)
	var c: float = SGFixed.to_int(b)

	assert_eq(b, 65536*2)
	assert_eq(c, 2.0)

func test_multiplication():
	var a: int = SGFixed.from_float(2.5)
	var b: int = SGFixed.from_float(2.5)
	var c: int = SGFixed.mul(a, b)

	assert_eq(c, 409600)

func test_division():
	var a: int = SGFixed.from_int(15)
	var b: int = SGFixed.from_int(2)
	var c: int = SGFixed.div(a, b)

	assert_eq(c, 491520)

func test_exp():
	var exp_0 = SGFixed.exp(0)
	assert_eq(exp_0, SGFixed.ONE)

	var exp_1 = SGFixed.exp(SGFixed.ONE)
	assert_eq(exp_1, SGFixed.E)

	var exp_2 = SGFixed.exp(SGFixed.TWO)
	assert_eq(exp_2, 484249 - 5)

	# 1.5
	var exp_15 = SGFixed.exp(98304)
	assert_eq(exp_15, 293707)

func test_log():
	# The error is shown as an addition to the correct value.

	var log_exp_0 = SGFixed.log(SGFixed.ONE)
	assert_eq(log_exp_0, 0)

	var log_exp_1 = SGFixed.log(SGFixed.E)
	assert_eq(log_exp_1, SGFixed.ONE + 1)

	var log_exp_2 = SGFixed.log(484249)
	assert_eq(log_exp_2, SGFixed.TWO + 1)

	# 1.5
	var log_exp_15 = SGFixed.log(293712)
	assert_eq(log_exp_15, 98304 + 1)

func test_power():
	var a: int = SGFixed.pow(65536, 131072)
	var b: int = SGFixed.pow(131072, 131072)
	var c: int = SGFixed.pow(131072, 32768)
	var d: int = SGFixed.pow(-32768, 32768)
	var e: int = SGFixed.pow(32768, 131072)
	var f: int = SGFixed.pow(6553, 32768)
	var g: int = SGFixed.pow(-131072, 131072)
	var h: int = SGFixed.pow(-131072, 196608)

	assert_eq(a, 65536) #1 to the power of fraction/integer = 1
	assert_eq(b, 262144) #Int exponent, int base - 2^2 = 4
	assert_eq(c, 92681) #Fraction exponent, int base - 2^0.5 = root 2
	assert_eq(d, 0) #Fraction exponent, negative base - unsupported, returns 0
	assert_eq(e, 16384) #int exponent, fractional base - 0.5^2 = 0.25
	assert_eq(f, 20724) #Fraction exponent, fraction base - 0.1^0.5 = 1/root 2
	assert_eq(g, 262144) #neg int exponent (even), negative int base - -2^2 = 4
	assert_eq(h, -524288) #neg int exponent (odd), negative int base - -2^3 = -8

func test_move_toward():
	var a: int = SGFixed.from_int(1)
	var b: int = SGFixed.from_int(2)
	var c: int = SGFixed.move_toward(a, b, 32768) #0.5
	var d: int = SGFixed.move_toward(a, b, 131072) #2

	assert_eq(c, 98304) # 1.5
	assert_eq(d, 131072) # 2

func test_trig():
	#
	# Test some important "well known" values
	#

	# sin()
	var sin_0: int = SGFixed.sin(0)
	assert_eq(sin_0, 0)

	var sin_90deg: int = SGFixed.sin(SGFixed.PI_DIV_2)
	assert_eq(sin_90deg, 65536)

	var sin_180deg: int = SGFixed.sin(SGFixed.PI)
	assert_eq(sin_180deg, 0)

	var sin_270deg: int = SGFixed.sin(SGFixed.PI + SGFixed.PI_DIV_2)
	assert_eq(sin_270deg, -65536)

	var sin_360deg: int = SGFixed.sin(SGFixed.TAU)
	assert_eq(sin_360deg, 0)

	# cos()
	var cos_0: int = SGFixed.cos(0)
	assert_eq(cos_0, 65536)

	var cos_90deg: int = SGFixed.cos(SGFixed.PI_DIV_2)
	assert_eq(cos_90deg, 0)

	var cos_180deg: int = SGFixed.cos(SGFixed.PI)
	assert_eq(cos_180deg, -65536)

	var cos_270deg: int = SGFixed.cos(SGFixed.PI + SGFixed.PI_DIV_2)
	assert_eq(cos_270deg, 0)

	var cos_360deg: int = SGFixed.cos(SGFixed.TAU)
	assert_eq(cos_360deg, 65536)

	# tan()
	var tan_0: int = SGFixed.tan(0)
	assert_eq(tan_0, 0)

	var tan_45deg: int = SGFixed.tan(SGFixed.PI_DIV_4)
	assert_eq(tan_45deg, 65536)

	var tan_90deg: int = SGFixed.tan(SGFixed.PI_DIV_2)
	assert_eq(tan_90deg, 0)

	var tan_135deg: int = SGFixed.tan(SGFixed.PI_DIV_2 + SGFixed.PI_DIV_4)
	assert_eq(tan_135deg, -65536)

	var tan_180deg: int = SGFixed.tan(SGFixed.PI)
	assert_eq(tan_180deg, 0)

	var tan_225deg: int = SGFixed.tan(SGFixed.PI + SGFixed.PI_DIV_4)
	assert_eq(tan_225deg, 65536)

	var tan_270deg: int = SGFixed.tan(SGFixed.PI + SGFixed.PI_DIV_2)
	assert_eq(tan_270deg, 0)

	var tan_315deg: int = SGFixed.tan(SGFixed.PI + SGFixed.PI_DIV_2 + SGFixed.PI_DIV_4)
	# Overshoots it a tiny bit, due to our SGFixed.PI_DIV_4 rounding up.
	assert_eq(tan_315deg, -65538)

	var tan_360deg: int = SGFixed.tan(SGFixed.PI * 2)
	assert_eq(tan_360deg, 0)

	# asin()
	var asin_65536: int = SGFixed.asin(65536)
	assert_eq(asin_65536, SGFixed.PI_DIV_2)

	var asin_32768: int = SGFixed.asin(32768)
	assert_eq(asin_32768, 34355)

	var asin_0: int = SGFixed.asin(0)
	assert_eq(asin_0, 0)

	var asin_n65536: int = SGFixed.asin(-65536)
	assert_eq(asin_n65536, -SGFixed.PI_DIV_2)

	var asin_n32768: int = SGFixed.asin(-32768)
	assert_eq(asin_n32768, -34355)

	# acos()
	var acos_65536: int = SGFixed.acos(65536)
	assert_eq(acos_65536, 0)

	var acos_32768: int = SGFixed.acos(32768)
	assert_eq(acos_32768, 68708)

	var acos_0: int = SGFixed.acos(0)
	# Sooo close! But we are 1 unchecked. That's probably fine?
	assert_eq(acos_0, SGFixed.PI_DIV_2 + 1)

	var acos_n32768: int = SGFixed.acos(-32768)
	assert_eq(acos_n32768, 137176)

	var acos_n65536: int = SGFixed.acos(-205887)
	assert_eq(acos_n65536, 0)

	# atan()
	var atan_0: int = SGFixed.atan(0)
	assert_eq(atan_0, 0)

	var atan_65536: int = SGFixed.atan(65536)
	assert_eq(atan_65536, SGFixed.PI_DIV_4)

	var atan_n65536: int = SGFixed.atan(-65536)
	assert_eq(atan_n65536, -SGFixed.PI_DIV_4)


	# atan2()
	var atan2_1_0: int = SGFixed.atan2(0, 65536)
	assert_eq(atan2_1_0, 0);

	var atan2_1_1: int = SGFixed.atan2(65536, 65536)
	assert_eq(atan2_1_1, SGFixed.PI_DIV_4)

	var atan2_0_1: int = SGFixed.atan2(65536, 0)
	assert_eq(atan2_0_1, SGFixed.PI_DIV_2)

	var atan2_n1_1: int = SGFixed.atan2(SGFixed.ONE, SGFixed.NEG_ONE)
	assert_eq(atan2_n1_1, SGFixed.PI_DIV_2 + SGFixed.PI_DIV_4)

	var atan2_n1_0: int = SGFixed.atan2(0, SGFixed.NEG_ONE)
	assert_eq(atan2_n1_0, SGFixed.PI)

	var atan2_n1_n1: int = SGFixed.atan2(SGFixed.NEG_ONE, SGFixed.NEG_ONE)
	assert_eq(atan2_n1_n1, -SGFixed.PI_DIV_2 - SGFixed.PI_DIV_4)

	var atan2_0_n1: int = SGFixed.atan2(SGFixed.NEG_ONE, 0)
	assert_eq(atan2_0_n1, -SGFixed.PI_DIV_2)

	var atan2_1_n1: int = SGFixed.atan2(SGFixed.NEG_ONE, SGFixed.ONE)
	assert_eq(atan2_1_n1, -SGFixed.PI_DIV_4)

	#
	# Test that big values loop around as expected
	#

	var sin_10: int = SGFixed.sin(10*65536)
	assert_eq(sin_10, -35665)

	var sin_50000: int = SGFixed.sin(50000*65536)
	assert_eq(sin_50000, -65313)

	var cos_10: int = SGFixed.cos(10*65536)
	assert_eq(cos_10, -55015)

#	var cos_50000: int = SGFixed.cos(50000*65536)
#	assert_eq(cos_50000, 1000)
#
#	var tan_10: int = SGFixed.tan(10*65536)
#	assert_eq(tan_10, 1000)
#
#	var tan_50000: int = SGFixed.tan(50000*65536)
#	assert_eq(tan_50000, 1000)
#
#	var asin_10: int = SGFixed.asin(10*65536)
#	assert_eq(asin_10, -35653)
#
#	var asin_50000: int = SGFixed.asin(50000*65536)
#	assert_eq(asin_50000, -65309)
#
#	var acos_10: int = SGFixed.acos(10*65536)
#	assert_eq(acos_10, 1000)
#
#	var acos_50000: int = SGFixed.acos(50000*65536)
#	assert_eq(acos_50000, 1000)
#
#	var atan_10: int = SGFixed.atan(10*65536)
#	assert_eq(atan_10, 1000)
#
#	var atan_50000: int = SGFixed.atan(50000*65536)
#	assert_eq(atan_50000, 1000)
#
#	var atan2_10: int = SGFixed.atan2(10*65536, 2*65536)
#	assert_eq(atan2_10, 1000)
#
#	var atan2_50000: int = SGFixed.atan2(50000*65536, 2*65536)
#	assert_eq(atan2_50000, 1000)

func test_from_string():
	var one = SGFixed.from_string("1")
	assert_eq(one, SGFixed.ONE)
	var half = SGFixed.from_string("0.5")
	assert_eq(half, SGFixed.HALF)
	var n_one = SGFixed.from_string("-1")
	assert_eq(n_one, SGFixed.NEG_ONE)
	var quarter = SGFixed.from_string("0.25")
	assert_eq(quarter, SGFixed.HALF/2)
	var one_percent = SGFixed.from_string("0.01")
	assert_eq(one_percent, SGFixed.ONE/100)

	var random = SGFixed.from_string("130.205")
	assert_eq(random, 8533114) #manually calculated
	var _invalid = SGFixed.from_string("buenas tardes")
	assert_eq(_invalid, 0)

func test_format_string():
	assert_eq( SGFixed.format_string(SGFixed.ONE), "1")
	assert_eq( SGFixed.format_string(SGFixed.ONE*200), "200")
	assert_eq( SGFixed.format_string(SGFixed.ONE*1024), "1024")
	assert_eq( SGFixed.format_string(SGFixed.ONE*65423), "65423")
	assert_eq( SGFixed.format_string(SGFixed.HALF), "0.5")
	assert_eq( SGFixed.format_string(SGFixed.ONE/4), "0.25")
	assert_eq( SGFixed.format_string(SGFixed.ONE/8), "0.125")
	assert_eq( SGFixed.format_string(SGFixed.ONE/16), "0.0625")
	assert_eq( SGFixed.format_string(SGFixed.ONE/32), "0.03125")
	assert_eq( SGFixed.format_string(SGFixed.ONE/64), "0.015625")
	assert_eq( SGFixed.format_string(SGFixed.ONE/128), "0.0078125")

func test_deg2rad():
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(360)), SGFixed.TAU) #TAU is 2 PI, a loop
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(180)), SGFixed.PI)
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(90)), SGFixed.PI_DIV_2)

	#SGFixed.PI_DIV_4 is rounded up therefore we need to add 1 to the deg_to_rad result
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(45))+1, SGFixed.PI_DIV_4)
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(30)), 34314)
	assert_eq(SGFixed.deg_to_rad(806092), 14068) # deg_to_rad(12.3)
	assert_eq(SGFixed.deg_to_rad(3279093), 57230) # deg_to_rad(50.035) Aprox
	assert_almost_eq(SGFixed.deg_to_rad(4696650), 81972, SGFixed.EPSILON) # deg_to_rad(71.6652) Aprox some are not exact

	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(720)), SGFixed.TAU*2) #TAU is 2 PI, a loop

### Negative checks
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-360)), -SGFixed.TAU) #TAU is 2 PI, a loop
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-180)), -SGFixed.PI)
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-90)), -SGFixed.PI_DIV_2)

	#Same reason as before but in negative
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-45))-1, -SGFixed.PI_DIV_4)
	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-30)), -34314)
	assert_eq(SGFixed.deg_to_rad(-806092), -14068) # deg_to_rad(12.3)
	assert_eq(SGFixed.deg_to_rad(-3279093), -57230) # deg_to_rad(50.035) Aprox
	assert_almost_eq(SGFixed.deg_to_rad(-4696650), -81972, SGFixed.EPSILON) # deg_to_rad(71.6652) Aprox, some are not exact

	assert_eq(SGFixed.deg_to_rad(SGFixed.from_int(-720)), -SGFixed.TAU*2) #TAU is 2 PI, a loop

#Converting from rad 2 deg has more inaccuracy because the decimal limit
func test_rad2deg():
	var precision_valve = 70
	assert_almost_eq(SGFixed.rad_to_deg(SGFixed.TAU), SGFixed.from_int(360), precision_valve) #TAU is 2 PI, a loop
	assert_almost_eq(SGFixed.rad_to_deg(SGFixed.PI), SGFixed.from_int(180), precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(SGFixed.PI_DIV_2), SGFixed.from_int(90), precision_valve)

	#SGFixed.PI_DIV_4 is rounded up therefore we need to add 1 to the deg_to_rad result
	assert_almost_eq(SGFixed.rad_to_deg(SGFixed.PI_DIV_4), SGFixed.from_int(45) + 1, precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(34314), SGFixed.from_int(30), precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(14068), 806092, precision_valve) # deg_to_rad(12.3)
	assert_almost_eq(SGFixed.rad_to_deg(57230), 3279093, precision_valve) # deg_to_rad(50.035) Aprox
	assert_almost_eq(SGFixed.rad_to_deg(81972), 4696650, precision_valve) # deg_to_rad(71.6652) Aprox some are not exact

	assert_almost_eq(SGFixed.rad_to_deg(SGFixed.TAU*2), SGFixed.from_int(720), precision_valve) #TAU is 2 PI, a loop

### Negative checks
	assert_almost_eq(SGFixed.rad_to_deg(-SGFixed.TAU), SGFixed.from_int(-360), precision_valve) #TAU is 2 PI, a loop
	assert_almost_eq(SGFixed.rad_to_deg(-SGFixed.PI), SGFixed.from_int(-180), precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(-SGFixed.PI_DIV_2), SGFixed.from_int(-90), precision_valve)

	#SGFixed.PI_DIV_4 is rounded up therefore we need to add 1 to the deg_to_rad result
	assert_almost_eq(SGFixed.rad_to_deg(-SGFixed.PI_DIV_4), -SGFixed.from_int(45) - 1, precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(-34314), -SGFixed.from_int(30), precision_valve)
	assert_almost_eq(SGFixed.rad_to_deg(-14068), -806092, precision_valve) # deg_to_rad(12.3)
	assert_almost_eq(SGFixed.rad_to_deg(-57230), -3279093, precision_valve) # deg_to_rad(50.035) Aprox
	assert_almost_eq(SGFixed.rad_to_deg(-81972), -4696650, precision_valve) # deg_to_rad(71.6652) Aprox some are not exact

	assert_almost_eq(SGFixed.rad_to_deg(-SGFixed.TAU*2), -SGFixed.from_int(720), precision_valve) #TAU is 2 PI, a loop

func test_floor():
	assert_eq(SGFixed.floor(SGFixed.from_float(12.3)), SGFixed.from_int(12))
	assert_eq(SGFixed.floor(SGFixed.from_float(0.999)), SGFixed.from_int(0))
	assert_eq(SGFixed.floor(SGFixed.from_int(12)), SGFixed.from_int(12))
	assert_eq(SGFixed.floor(SGFixed.from_float(6543.5)), SGFixed.from_int(6543))

func test_round():
	assert_eq(SGFixed.round(SGFixed.from_float(12.3)), SGFixed.from_int(12))
	assert_eq(SGFixed.round(SGFixed.from_float(12.499)), SGFixed.from_int(12))
	assert_eq(SGFixed.round(SGFixed.from_float(0.999)), SGFixed.from_int(1))
	assert_eq(SGFixed.round(SGFixed.from_int(12)), SGFixed.from_int(12))
	assert_eq(SGFixed.round(SGFixed.from_float(6543.5)), SGFixed.from_int(6544))

func test_ceil():
	assert_eq(SGFixed.ceil(SGFixed.from_float(12.3)), SGFixed.from_int(13))
	assert_eq(SGFixed.ceil(SGFixed.from_float(12.499)), SGFixed.from_int(13))
	assert_eq(SGFixed.ceil(SGFixed.from_float(0.999)), SGFixed.from_int(1))
	assert_eq(SGFixed.ceil(SGFixed.from_int(12)), SGFixed.from_int(12))
	assert_eq(SGFixed.ceil(SGFixed.from_float(6543.5)), SGFixed.from_int(6544))
