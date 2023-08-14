#include "XMLWrapper.h"

XMLDocWrapper::XMLDocWrapper()
{ 
	this->xmlDoc = new XMLDoc();
	this->xmlDoc->n_nodes = 2046;
}
XMLDocWrapper::~XMLDocWrapper()
{

}

void XMLDocWrapper::Wrapper_XMLDoc_free()
{
	//XMLDoc_free(this->xmlDoc);
};

void XMLDocWrapper::Wrapper_XMLDoc_print(FILE * io, const char* tag_sep, const char * child_sep, int keep_text_spaces, int sz_line, int nb_char_tab)
{
	XMLDoc_print(this->xmlDoc, io, tag_sep, child_sep, keep_text_spaces, sz_line, nb_char_tab);
}

int XMLDocWrapper::Wrapper_XMLDoc_parse_file_DOM(const char * name)
{
	int returnValue = XMLDoc_parse_file_DOM(C2SX(name),this->xmlDoc);
	fprintf(stdout, "XMLDoc_parse_file_DOM read [%d] n_nodes\n", this->xmlDoc->n_nodes);
	return returnValue;
}

void XMLDocWrapper::Wrapper_XMLDoc_init()
{
	XMLDoc_init(this->xmlDoc);
}

int XMLDocWrapper::Wrapper_getXMLDoc_n_nodes()
{
	return this->xmlDoc->n_nodes;
}

XMLNodeWrapper * XMLDocWrapper::Wrapper_getXMLDoc_Node_At(int i)
{
	if (i < 0 || i >= this->xmlDoc->n_nodes) return NULL;
	XMLNodeWrapper * nodeWrapper = new XMLNodeWrapper(this->xmlDoc->nodes[i]);
	return nodeWrapper;
}

XMLNodeWrapper::XMLNodeWrapper(XMLNode * xmlNode)
{
	this->xmlNode = xmlNode;
}

XMLNodeWrapper::~XMLNodeWrapper()
{

}

int XMLNodeWrapper::Wrapper_XMLNode_get_n_children()
{
	return this->xmlNode->n_children;
}

XMLNodeWrapper * XMLNodeWrapper::Wrapper_getXMLNode_Children_At(int i)
{
	if (i < 0 || i >= this->xmlNode->n_children) return NULL;
	XMLNodeWrapper * nodeWrapper = new XMLNodeWrapper(this->xmlNode->children[i]);
	return nodeWrapper;
}

const char * XMLNodeWrapper::Wrapper_getXMLNode_tag()
{
	return this->xmlNode->tag;
}

const char * XMLNodeWrapper::Wrapper_getXMLNode_text()
{
	return this->xmlNode->text;
}