package nYAML;

class NYAML {

	public inline static function encode(o:Array<Dynamic>):String
		return new nYAML.NYAMLEncoder(o).getOutput()
		
	public inline static function decode(s:String):Array<Dynamic>
		return new nYAML.NYAMLDecoder(s).getData()

}
