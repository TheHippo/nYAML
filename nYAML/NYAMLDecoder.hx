package nYAML;

class NYAMLDecoder {

	static var decode:String->Void = neko.Lib.load("nyaml","decode",1);

	var data:Dynamic;
	
	public function new(s:String)
		data = NYAMLDecoder.decode(untyped s.__s)
	
	public inline function getData():Dynamic
		return data
}
