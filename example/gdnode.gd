extends Node
@onready var node: Node = $Node # contains the python script
const IMAGE = preload("res://icon.svg")
func _process(delta: float) -> void:
	var img = IMAGE.get_image()
	var data = img.get_data()
	node.call_deferred("process_frame",data,img.get_width(),img.get_height(),self)
	
func update_image(image) -> void:
	# Do something
	print("update_image")
