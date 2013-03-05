#ifndef __JKT__DATATREEVIEW_H
#define __JKT__DATATREEVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "data/DataTree.h"

extern DataTree dataTreeServeur;
extern DataTree dataTreeClient1;

class Viewer;
namespace JktMoteur {
	class CMaterial;
	class CLight;
}

class DataTreeView : public View
{
	AG_Treetbl* _treeServeur;
	AG_Treetbl* _treeClient1;

	void drawBranche(Branche* branche, AG_Treetbl* tree, int colId);

public:
    DataTreeView(const AG_EventFn controllerCallback);
    ~DataTreeView(void);

    void show(void);
};

#endif
