#ifndef __JKT__OPENSCENEASEECRASEREPVIEW_H
#define __JKT__OPENSCENEASEECRASEREPVIEW_H

#include <vector>
#include <string>

#include "menu/View.h"

class OpenSceneASEEcraseRepView : public View
{
	std::string _aseName;
	std::string _repName;
public:
	OpenSceneASEEcraseRepView(const AG_EventFn controllerCallback);
	~OpenSceneASEEcraseRepView(void);

	void setAseName(const std::string& aseName);
	std::string getAseName() const;

	void setRepName(const std::string& repName);
	std::string getRepName() const;
};

#endif

