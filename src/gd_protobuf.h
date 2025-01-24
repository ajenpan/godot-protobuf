#pragma once

#include <google/protobuf/descriptor.h>

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/variant.hpp>

class GDProtobuf : public godot::Object {
	GDCLASS(GDProtobuf, Object);

public:
	static void _bind_methods();

	bool add_desc_from_file(const godot::String& fname);
	bool add_desc_from_content(const godot::PackedByteArray& fname);

	godot::PackedByteArray marshal(const godot::String& msgname, const godot::Dictionary& dict);
	godot::Dictionary unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw);

public:
	bool msg_to_dict(const google::protobuf::Message& msg, godot::Dictionary& out);
	bool dict_to_msg(const godot::Dictionary& dict, google::protobuf::Message* out);

protected:
	google::protobuf::DescriptorPool pool;
};
