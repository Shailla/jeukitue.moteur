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
	AG_Tlist* _dataTree;
	AG_Table* _clientsTable;

	void drawBranche(Branche* branche, int depth);

public:
    DataTreeView(const AG_EventFn controllerCallback);
    ~DataTreeView(void);

    void show(void);
    void refresh();

    static void refreshClientTable(DataTreeView* This);
    static void selectionChanged(AG_Event* event);
};

#endif
