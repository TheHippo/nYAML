class Test {
	static function main() {
		var file = neko.io.File.read("test.yaml",true);
		trace(nYAML.NYAML.decode(file.readAll().toString()));
	}
}
