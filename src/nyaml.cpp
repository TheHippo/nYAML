#include <algorithm>
#include <string>
#include <yaml-cpp/yaml.h>
#include <neko.h>

using namespace std;
using namespace YAML;


int lower_case (int c) {
	return tolower (c);
}

int lowerCompare(string in, string cmp) {
	transform(in.begin(), in.end(), in.begin(), ::tolower);
	if (in == cmp)
		return 1;
	return 0;	
}

value parseSkalar(string in) {
	
	string cmp("true");
	if (lowerCompare(in,cmp))
		return val_true;
	else {
		cmp.assign("false");
		if (lowerCompare(in,cmp))
			return val_false;
		else {
			int i = atoi(in.c_str());
			ostringstream s; 
			if (s << i)
				if (in==s.str())
					return alloc_int(i);
					
			double f = atof(in.c_str());
			ostringstream sf;
			if (sf << f)
				if (in == sf.str())
					return alloc_float(f);
		}
		
	}
	return alloc_string(in.c_str());
}

value traverse(const YAML::Node & node) {
    // recursive depth first
    CONTENT_TYPE type = node.GetType();
    string out;
    switch (type) {
        case CT_SCALAR:
            node >> out;
            return parseSkalar(out);
            break;
        case CT_SEQUENCE:
			{
				value ret = alloc_array(node.size());
				value *ptr = val_array_ptr(ret);
				for (unsigned int i = 0; i < node.size(); i++) {
					const Node & subnode = node[i];
					ptr[i]=traverse(subnode);
				}
				return ret;
			}
            break;
        case CT_MAP:
			{
				value ret = alloc_object(NULL);
				for (YAML::Iterator i = node.begin(); i != node.end(); ++i) {
					const Node & key   = i.first();
					const Node & value = i.second();
					key >> out;
					alloc_field(ret,val_id(out.c_str()),traverse(value));
				}
				return ret;
			}
            break;
        case CT_NONE:
            return val_null;
            break;
        default:
            failure("Warning: traverse: unknown/unsupported node type");
            break;
    }
    return val_null;
}

value decode(value str) {
	val_check(str,string);
	string cstr(val_string(str));
	istringstream stream;
	stream.str(cstr);
	Parser parser(stream);

	list<value> documents;

	Node doc;
	while(parser.GetNextDocument(doc)) {
		value ndoc = traverse(doc);
		documents.push_back(ndoc);		
	}

	unsigned int size = documents.size();
	value ret = alloc_array(size);
	value  *ptr = val_array_ptr(ret);
	for (unsigned int i=0; i<size; i++) {
		ptr[i] = documents.front();
		documents.pop_front();
	};
	return ret;
}

value encode(value ar) {
	return alloc_string("foo");
}


DEFINE_PRIM(decode,1);
DEFINE_PRIM(encode,1);
