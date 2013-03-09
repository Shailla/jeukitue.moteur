#ifndef __JKT__DATATREEVIEW_H
#define __JKT__DATATREEVIEW_H

#include <map>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "data/DataTree.h"

extern DataTree dataTree;

class Viewer;
namespace JktMoteur {
	class CMaterial;
	class CLight;
}

class DataTreeView : public View
{
	AG_Treetbl* _dataTree;
	AG_Table* _clientsTable;
	std::map<AG_TreetblRow*, Branche*> _treeRows;

	void drawBranche(Branche* branche, AG_Treetbl* tree, AG_TreetblRow* parentRow, int& rowId);

public:
    DataTreeView(const AG_EventFn controllerCallback);
    ~DataTreeView(void);

    void show(void);
    void refresh();

    static void updateTable(AG_Event* event);
};

#endif
