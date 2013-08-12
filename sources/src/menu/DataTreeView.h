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

namespace JktMoteur {
	class CMaterial;
	class CLight;
}


//class DataTreeDetails {
//	AG_Window* _window;
//	AG_Tlist* _dataList;
//	AG_Table* _clientsTable;
//	DataTree* _dataTree;
//
//public:
//	DataTreeDetails(AG_Window* window, AG_Tlist* dataList, AG_Table* clientsTable, DataTree* dataTree) {
//		_window = window;
//		_dataList = dataList;
//		_clientsTable = clientsTable;
//		_dataTree = dataTree;
//	}
//
//	AG_Window* getWindow() {
//		return _window;
//	}
//
//	AG_Table* getClientsTable() {
//		return _clientsTable;
//	}
//
//	AG_Tlist* getDataList() {
//		return _dataList;
//	}
//
//	DataTree* getDataTree() {
//		return _dataTree;
//	}
//};


class DataTreeView : public View {
	AG_Window* _window;
	AG_Tlist* _dataList;
	AG_Table* _clientsTable;
	DataTree* _dataTree;

	static void drawBranche(DataTreeView* details, Branche* branche, int depth);
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
