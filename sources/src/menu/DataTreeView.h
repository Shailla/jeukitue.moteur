#ifndef __JKT__DATATREEVIEW_H
#define __JKT__DATATREEVIEW_H

#include <map>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"

class Viewer;

namespace jkt {
	class CMaterial;
	class CLight;
}

class DataTreeView : public View {
	AG_Tlist* _dataList;
	AG_Table* _clientsTable;
	DataTree* _dataTree;

	static void drawBranche(DataTreeView* details, AbstractBranche* branche, int depth);
	void refreshTree();
	void drawWidgets(AG_Window* window, AG_Box* box);

public:
    DataTreeView(const AG_EventFn controllerCallback);
    ~DataTreeView(void);

    void show(void);
    static void refresh(AG_Event* event);

    static void selectionChanged(AG_Event* event);
};

#endif
