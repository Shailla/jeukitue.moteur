#ifndef __JKT__DATATREEVIEW_H
#define __JKT__DATATREEVIEW_H

#include <map>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "data/DataTree.h"

class DataTreeDetails {
public:
	AG_Tlist* _dataList;
	AG_Table* _clientsTable;
};

extern DataTree serveurDataTree;
extern std::vector<DataTree*> clientDataTrees;

class Viewer;
namespace JktMoteur {
	class CMaterial;
	class CLight;
}

class DataTreeView : public View {
	DataTreeDetails _serveurDetails;
	std::map<DataTree*, DataTreeDetails*> _clientDataTrees;

	static void drawBranche(DataTreeDetails* details, Branche* branche, int depth);
	static void refreshServeur(AG_Window* window, DataTree* tree, DataTreeDetails* details);
	static void drawWidgets(DataTreeDetails* details, AG_Box* box);

public:
    DataTreeView(const AG_EventFn controllerCallback);
    ~DataTreeView(void);

    void show(void);
    static void refresh(AG_Event* event);

    static void refreshClientTable(DataTreeDetails* details);
    static void selectionChanged(AG_Event* event);
    static void openClientsWindows(AG_Event* event);
};

#endif
