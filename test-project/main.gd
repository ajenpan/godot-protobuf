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
	# message TestMessage {
	# 	int32 i32 = 1;
	# 	int64 i64 = 2;
	# 	float d32 = 3;
	# 	double d64 = 4;
	# 	string str = 5;
	# 	bytes bz = 6;
	# 	Person persion = 7;
	# 	TestEnum e = 8;
	# 	// repeated;
	# 	repeated int32 r_i32 = 11;
	# 	repeated int64 r_i64 = 12;
	# 	repeated float r_d32 = 13;
	# 	repeated double r_d64 = 14;
	# 	repeated string r_str = 15;
	# 	repeated bytes r_bz = 16;
	# 	repeated Person r_persion = 17;
	# 	repeated TestEnum r_e = 18;
	# }
	var dict = {
		i32 = 1,
		i64 = 2,
		d32 = 3.0,
		d64 = 4.0,
		str = "5",
		bz = PackedByteArray(),
		persion = {},
	}
	var msgname = "testproto.TestMessage"
	print(msgname)
	# godot::PackedByteArray marshal(const godot::String& msgname, const godot::Dictionary& dict);
	# godot::Dictionary unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw);
	print(dict)

	var raw = gdproto.marshal(msgname, dict)
	print(raw)
	var newdict = gdproto.unmarshal(msgname, raw)
	print(newdict)
	for k in dict:
		print(k)
		print(newdict[k])
		if dict[k] != newdict[k]:
			print(k, " not eq")
