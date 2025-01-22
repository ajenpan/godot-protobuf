# godot protobuf

the google protobuf extension for godot

## api

todo:

```cpp
	bool add_desc_from_file(const godot::String& fname);
	bool add_desc_from_content(const godot::PackedByteArray& fname);

	godot::PackedByteArray marshal(const godot::String& msgname, const godot::Dictionary& dict);
	godot::Dictionary unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw);
```
