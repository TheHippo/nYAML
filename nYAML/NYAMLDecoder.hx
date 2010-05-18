package nYAML;

class NYAMLDecoder {

	var input:String;
	var data:Dynamic;
	
	public function new(s:String)
		this.input = s
	
	public inline function getData():Dynamic
		return data
}
