#pragma once
#include "sxmlc.h"

class XMLNodeWrapper
{
public:
	XMLNodeWrapper(XMLNode * xmlNode);
	~XMLNodeWrapper();

	

	int Wrapper_XMLNode_get_n_children();
	XMLNodeWrapper * Wrapper_getXMLNode_Children_At(int i);
	const char * Wrapper_getXMLNode_tag();
	const char * Wrapper_getXMLNode_text();

private:
	XMLNode * xmlNode;
};

class XMLDocWrapper
{
public:
	XMLDocWrapper();
	~XMLDocWrapper();

	void Wrapper_XMLDoc_free();

	void Wrapper_XMLDoc_print(FILE * io, const char* tag_sep, const char * child_sep, int keep_text_spaces, int sz_line, int nb_char_tab);

	int Wrapper_XMLDoc_parse_file_DOM(const char * name);

	void Wrapper_XMLDoc_init();

	int Wrapper_getXMLDoc_n_nodes();

	XMLNodeWrapper * Wrapper_getXMLDoc_Node_At(int i);

private:
	XMLDoc * xmlDoc;


};




