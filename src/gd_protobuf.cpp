#include "gd_protobuf.h"

#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/reflection.h>

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace google::protobuf;
using namespace godot;

void GDProtobuf::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_desc_from_file"), &GDProtobuf::add_desc_from_file);
	ClassDB::bind_method(D_METHOD("add_desc_from_content"), &GDProtobuf::add_desc_from_content);

	ClassDB::bind_method(D_METHOD("marshal"), &GDProtobuf::marshal);
	ClassDB::bind_method(D_METHOD("unmarshal"), &GDProtobuf::unmarshal);
}

bool GDProtobuf::add_desc_from_file(const godot::String& fname) {
	auto raw = godot::FileAccess::get_file_as_bytes(fname);
	if (raw.is_empty()) {
		return false;
	}
	return add_desc_from_content(raw);
}

bool GDProtobuf::add_desc_from_content(const godot::PackedByteArray& raw) {
	google::protobuf::FileDescriptorSet fileset;
	auto ok = fileset.ParseFromArray(raw.ptr(), raw.size());
	if (!ok) {
		return ok;
	}

	for (auto& f : fileset.file()) {
		pool.BuildFile(f);
	}
	return true;
}

godot::PackedByteArray GDProtobuf::marshal(const godot::String& msgname, const godot::Dictionary& dict) {
	godot::PackedByteArray ret;
	return ret;
}

godot::Dictionary GDProtobuf::unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw) {
	godot::Dictionary out;
	return out;
}

godot::Dictionary GDProtobuf::msg_to_dict(const google::protobuf::Message& msg) {
	const google::protobuf::Descriptor* descriptor = msg.GetDescriptor();
	const google::protobuf::Reflection* reflection = msg.GetReflection();

	godot::Dictionary out;

	for (int i = 0; i < descriptor->field_count(); i++) {
		const auto* field = descriptor->field(i);
		if (field->is_repeated()) {
			godot::Array arr;
			auto rep_size = reflection->FieldSize(msg, field);
			if (rep_size > 0) {
				switch (field->cpp_type()) {
					case google::protobuf::FieldDescriptor::CPPTYPE_INT32: {
						auto ref = reflection->GetRepeatedFieldRef<int32_t>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_INT64: {
						auto ref = reflection->GetRepeatedFieldRef<int64_t>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_UINT32: {
						auto ref = reflection->GetRepeatedFieldRef<uint32_t>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: {
						auto ref = reflection->GetRepeatedFieldRef<uint64_t>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE: {
						auto ref = reflection->GetRepeatedFieldRef<double>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT: {
						auto ref = reflection->GetRepeatedFieldRef<float>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_BOOL: {
						auto ref = reflection->GetRepeatedFieldRef<bool>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
						auto ref = reflection->GetRepeatedFieldRef<int32_t>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(*iter);
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
						auto ref = reflection->GetRepeatedFieldRef<std::string>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(godot::String((*iter).c_str()));
					} break;
					case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
						auto ref = reflection->GetRepeatedFieldRef<google::protobuf::Message>(msg, field);
						for (auto iter = ref.begin(); iter != ref.end(); iter++)
							arr.push_back(msg_to_dict(*iter));
					} break;
					default: {
					} break;
				}
			}
			out[godot::String(field->name().c_str())] = arr;
		} else {
			godot::Variant var;
			switch (field->cpp_type()) {
				case google::protobuf::FieldDescriptor::CPPTYPE_INT32: {
					var = reflection->GetInt32(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_INT64: {
					var = reflection->GetInt64(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_UINT32: {
					var = reflection->GetUInt32(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: {
					var = reflection->GetUInt64(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE: {
					var = reflection->GetDouble(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT: {
					var = reflection->GetFloat(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_BOOL: {
					var = reflection->GetBool(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
					var = reflection->GetEnumValue(msg, field);
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
					std::string value = reflection->GetString(msg, field);
					var = godot::String(value.c_str());
				} break;
				case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
					var = msg_to_dict(reflection->GetMessage(msg, field));
				} break;
				default: {
				} break;
			}
			out[godot::String(field->name().c_str())] = var;
		}
	}
	return out;
}

google::protobuf::Message* GDProtobuf::new_msg(const std::string& name) {
	auto descriptor = pool.FindMessageTypeByName(name);
	if (descriptor == nullptr) {
		return nullptr;
	}
	google::protobuf::DynamicMessageFactory factory;
	const google::protobuf::Message* prototype_msg = factory.GetPrototype(descriptor);
	if (prototype_msg == nullptr) {
		return nullptr;
	}
	return prototype_msg->New();
}
