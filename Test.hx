class Test {
	static function main() {
		var file = neko.io.File.read("test.yaml",true);
		var docs = nYAML.NYAML.decode(file.readAll().toString());
		for (doc in docs)
			trace(Std.string(doc));
		trace(nYAML.NYAML.encode(docs));
	}
}
