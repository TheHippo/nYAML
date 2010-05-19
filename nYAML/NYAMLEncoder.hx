package nYAML;

class NYAMLEncoder {

	static var encode = neko.Lib.load("nyaml","encode",1);
	
	var out:String;
	
	public function new(data:Array<Dynamic>) {
		var ret = new List<String>();
		for (x in data)
			ret.push(neko.Lib.nekoToHaxe(NYAMLEncoder.encode(neko.Lib.haxeToNeko(x))));
		out = ret.join("\n");
	}
	
	public inline function getOutput()
		return out

}
