extends Node2D

@onready var detector_area1 = $DetectorArea1
@onready var detector_area2 = $DetectorArea2

@onready var colliding_area = $CollidingArea
@onready var colliding_area_shape1 = $CollidingArea/Shape1
@onready var colliding_area_shape2 = $CollidingArea/Shape2

@onready var colliding_body = $CollidingBody
@onready var colliding_body_shape1 = $CollidingBody/Shape1
@onready var colliding_body_shape2 = $CollidingBody/Shape2
