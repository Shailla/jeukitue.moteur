#ifndef __JKT__MAPTREEVIEW_H
#define __JKT__MAPTREEVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;
namespace jkt {
	class CMaterial;
	class CLight;
}

class MapTreeView : public View
{
//	AG_Tlist* _tree;
	AG_Treetbl* _tree;

    AG_Button* _buttonRetour;

    void showOneMaterial(AG_TreetblRow* parentRow, const jkt::CMaterial* mat, const int depth, int& rowId);
    void showOneLight(AG_TreetblRow* parentRow, const jkt::CLight* mat, const int depth, int& rowId);
    AG_TreetblRow* addFloat4TabToTree(AG_Treetbl* tree, AG_TreetblRow* parentRow, const int id, const int depth, const char* name, const float value[4]);

public:
    MapTreeView(const AG_EventFn controllerCallback);
    ~MapTreeView(void);

    void show(void);
};

#endif
