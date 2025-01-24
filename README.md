# godot protobuf

the google protobuf extension for godot

## useage

1. gen proto desc file
   `protoc -descriptor_set_out=test.desc test.proto`
2. load desc file on your gdscript

```godot
	var gdproto = GDProtobuf.new()
	gdproto.add_desc_from_file("res://bin/test.desc")
```

3. and then you can marshal dict and send

## api

```cpp
	bool add_desc_from_file(const godot::String& fname);
	bool add_desc_from_content(const godot::PackedByteArray& fname);

	godot::PackedByteArray marshal(const godot::String& msgname, const godot::Dictionary& dict);
	godot::Dictionary unmarshal(const godot::String& msgname, const godot::PackedByteArray& raw);
```
