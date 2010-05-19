#include <algorithm>
#include <string>
#include "../yaml-cpp/include/yaml.h"
#include <neko.h>


using namespace std;
using namespace YAML;

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
			cmp.assign("null");
			if (lowerCompare(in,cmp))
				return val_null;
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
		
	}
	return alloc_string(in.c_str());
}

value traverse_decode(const YAML::Node & node) {
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
					ptr[i]=traverse_decode(subnode);
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
					alloc_field(ret,val_id(out.c_str()),traverse_decode(value));
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
	try {
		Parser parser(stream);

		list<value> documents;

		Node doc;
		//process every document
		while(parser.GetNextDocument(doc)) {
			value ndoc = traverse_decode(doc);
			if (!val_is_null(ndoc))
				documents.push_back(ndoc);		
		}

		//pack documents into a neko array
		unsigned int size = documents.size();
		value ret = alloc_array(size);
		value  *ptr = val_array_ptr(ret);
		for (unsigned int i=0; i<size; i++) {
			ptr[i] = documents.front();
			documents.pop_front();
		};
		return ret;
	}
	catch (ParserException& e) {
		failure(e.what());
		return val_null;
	}
}

void traverse_encode(value obj,Emitter &out);

void iter_object(value obj, field f, void *p) {
	list<field> *fields = (list<field>*)p;
	fields->push_back(f);
}

void traverse_encode(value obj,Emitter &out) {
	switch (val_type(obj)) {
		case VAL_NULL:
			out << "null";
			break;
		case VAL_INT:
			out << val_int(obj);
			break;
		case VAL_FLOAT:
			out << val_float(obj);
			break;
		case VAL_BOOL:
			if (val_bool(obj))
				out << "true";
			else
				out << "false";
			break;
		case VAL_ARRAY:
			{
				unsigned int size = val_array_size(obj);
				value *ptr = val_array_ptr(obj);
				//if (size < 4) TODO: find a good criteria when to use flow
				//	out << Flow;
				out << BeginSeq;				
				for (unsigned int i = 0; i < size; i++) {
					traverse_encode(ptr[i],out);
				}
				out << EndSeq;
			}
			break;
		case VAL_FUNCTION:
			failure("Coudl not encode function");
			break;
		case VAL_STRING:
			out << val_string(obj);
			break;
		case VAL_OBJECT:
			{
				list<field> fields;
				//get keys
				val_iter_fields(obj,iter_object,&fields);
				out << BeginMap;
				field f;
				while (!fields.empty()) { //iterate
					f = fields.front();
					out <<  Key << val_string(val_field_name(f));
					fields.pop_front();
					out << Value;
					traverse_encode(val_field(obj,f),out);
				}
				out << EndMap;
			}
			break;
		case VAL_ABSTRACT:
			failure("Could not encode abstract");
			break;
		default:
			failure("Unknown value type");
			break;
	}
}

value encode(value ar) {
	if (val_is_function(ar) || val_is_abstract(ar)) {
		failure("Could not process input");
		return val_null;
	}
	else {
		Emitter out;
		traverse_encode(ar,out);
		if (!out.good())
			failure(out.GetLastError().c_str());
		return alloc_string(out.c_str());
	}
}

DEFINE_PRIM(decode,1);
DEFINE_PRIM(encode,1);
