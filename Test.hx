class Test {
	static function main() {
		var file = neko.io.File.read("test.yaml",true);
		for (doc in nYAML.NYAML.decode(file.readAll().toString()))
			trace(Std.string(doc)+": "+Type.typeof(doc));
	}
}
