#include <iostream>
#include <yaml-cpp/yaml.h>
#include <neko.h>

using namespace std;
using namespace YAML;

value parseSkalar(string in) {
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


DEFINE_PRIM(decode,1);

