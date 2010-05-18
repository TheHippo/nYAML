#include <iostream>
#include <yaml-cpp/yaml.h>
#include <neko.h>

using namespace std;
using namespace YAML;

void traverse(const YAML::Node & node, unsigned int depth = 0) {
    // recursive depth first
    YAML::CONTENT_TYPE type = node.GetType();
    string indent((size_t)depth, '\t');
    string out;
    switch (type)
    {
        case YAML::CT_SCALAR:
            node >> out;
            cout << indent << "SCALAR: " << out << endl;
            break;
        case YAML::CT_SEQUENCE:
            cout << indent << "SEQUENCE:" << endl;
            for (unsigned int i = 0; i < node.size(); i++) {
                const YAML::Node & subnode = node[i];
                cout << indent << "[" << i << "]:" << endl;
                traverse(subnode, depth + 1);
            }
            break;
        case YAML::CT_MAP:
            cout << indent << "MAP:" << endl;
            for (YAML::Iterator i = node.begin(); i != node.end(); ++i) {
                const YAML::Node & key   = i.first();
                const YAML::Node & value = i.second();
                key >> out;
                cout << indent << "KEY: " << out << endl;
                cout << indent << "VALUE:" << endl;
                traverse(value, depth + 1);
            }
            break;
        case YAML::CT_NONE:
            cout << indent << "(empty)" << endl;
            break;
        default:
            cerr << "Warning: traverse: unknown/unsupported node type" << endl;
    }
}

value decode(value str) {
	val_check(str,string);
	string cstr(val_string(str));
	istringstream stream;
	stream.str(cstr);
	Parser parser(stream);
	Node doc;
    while(parser.GetNextDocument(doc)) {
		traverse(doc);
		switch (doc.GetType()) {
			case CT_NONE:
				cout << "null" << endl;
				break;
			case CT_SCALAR:
				cout << "scalar" << endl;
				break;
			case CT_SEQUENCE:
				cout << "sequence" << endl;
				break;
			case CT_MAP:
				cout << "map" << endl;
				break;
			default:
				break;
		}
    }
	return val_null;
}


DEFINE_PRIM(decode,1);

