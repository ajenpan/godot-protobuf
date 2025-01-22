#pragma once

#include <google/protobuf/descriptor.h>

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/variant.hpp>

class GDProtobuf : public godot::Object {
public:
	GDCLASS(GDProtobuf, Object);

public:
	static void _bind_methods();

	bool add_desc_from_file(const godot::String& fname);
	bool add_desc_from_content(const godot::PackedByteArray& fname);

	godot::PackedByteArray marshal(const godot::String& msgname, const godot::Dictionary& dict);
	godot::Dictionary unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw);

public:
	godot::Dictionary msg_to_dict(const google::protobuf::Message& msg);
	google::protobuf::Message* new_msg(const std::string& name);

protected:
	google::protobuf::DescriptorPool pool;
};
