#ifndef __JKT__OPENSCENEASEECRASEREPVIEW_H
#define __JKT__OPENSCENEASEECRASEREPVIEW_H

#include <vector>
#include <string>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

#include "menu/View.h"

class OpenSceneASEEcraseRepView : public View
{
	std::string _aseName;
public:
	OpenSceneASEEcraseRepView(const AG_EventFn controllerCallback);
	~OpenSceneASEEcraseRepView(void);

	void setAseName(const std::string& aseName);
	std::string getAseName() const;
};

#endif

