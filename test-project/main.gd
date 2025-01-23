extends "res://test_base.gd"

var custom_signal_emitted = null

class TestClass:
	func test(p_msg: String) -> String:
		return p_msg + " world"
	
func _ready(): 	
	print("start to test")
	var gdproto = GDProtobuf.new()
	var ok = gdproto.add_desc_from_file("res://bin/test.desc")
	print("add_desc_from_file result:", ok)

