
#include <string>

#include "tinyxml.h"

namespace JKT_PACKAGE_UTILS
{

class CJktXmlElement : public TiXmlElement
{
	void SetAttribute(const char* name, string &_value);
	void SetAttribute(const char* name, float _value);
	void SetAttribute(const char* name, bool _value);
};

}
