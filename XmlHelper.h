
#ifndef __JKT__UTILS_XMLHELPER_H
#define __JKT__UTILS_XMLHELPER_H

#include "tinyxml.h"

class CXmlHelper
{
	static const unsigned int NUM_INDENTS_PER_SPACE;
	static const char* getIndent( unsigned int numIndents );
	static const char* getIndentAlt( unsigned int numIndents );
	static int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent);
	static void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 );
public:
	static void dump_to_stdout(const char* pFilename);
};

#endif
