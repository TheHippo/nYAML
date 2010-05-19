package nYAML;

class NYAMLDecoder {

	static var decode = neko.Lib.load("nyaml","decode",1);

	var data:Dynamic;
	
	public function new(s:String)
		data = neko.Lib.nekoToHaxe(NYAMLDecoder.decode(untyped s.__s))
	
	public inline function getData():Array<Dynamic>
		return data
}
