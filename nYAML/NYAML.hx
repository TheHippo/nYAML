package nYAML;

class NYAML {

	public static function encode(o:Dynamic):String {
		throw "not implemented yet!";
		return "";
	}
		
	public static function decode(s:String):Dynamic
		return new nYAML.NYAMLDecoder(s).getData()

}
