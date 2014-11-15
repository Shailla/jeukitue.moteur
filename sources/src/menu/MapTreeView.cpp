
#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "spatial/Map.h"
#include "main/Game.h"
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"
#include "spatial/widget/Texture.h"

#include "menu/MapTreeView.h"

using namespace JktMoteur;

extern CGame Game;

MapTreeView::MapTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Arbre de la MAP courante");

	_tree = AG_TreetblNew(m_window, AG_TREETBL_EXPAND, NULL, NULL);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	_buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 100, 100);
    hide();
}

MapTreeView::~MapTreeView(void)
{
}

void MapTreeView::show(void) {
	CMap* currentMap = Game.getMap();

	AG_TreetblClearRows(_tree);

	if(currentMap) {
		AG_TreetblAddCol(_tree, 0, "<XXXXXX>", "");
		AG_TreetblAddCol(_tree, 1, "<XXXXXX>", "");
		AG_TreetblAddCol(_tree, 2, "<XXXXXXXXXXXXXXXXXXXXX>", "");
		AG_TreetblAddCol(_tree, 3, "<XXXXXXXXXXXXXXXXXXXXX>", "");
		AG_TreetblAddCol(_tree, 4, "<XXXXXXXXXXXXXXXXXXXXX>", "");

		int rowId = 0;

		// Branche des matériaux
		{
			vector<CMaterial*>& materiaux = currentMap->m_TabMaterial;

			ostringstream materiauxStr;
			materiauxStr << materiaux.size() << " materiaux";
			AG_TreetblRow* materiauxRow = AG_TreetblAddRow(_tree, NULL, rowId++, "%d%s", 0, materiauxStr.str().c_str());

			vector<CMaterial*>::iterator iter;

			for(iter=materiaux.begin() ; iter!=materiaux.end() ; iter++) {
				CMaterial* mat = *iter;

				showOneMaterial(materiauxRow, mat, 1, rowId);
			}
		}

		// Branche des lumières
		{
			vector<CLight*>& lights = currentMap->getLights();

			ostringstream lightsStr;
			lightsStr << lights.size() << " lumieres";
			AG_TreetblRow* lightsRow = AG_TreetblAddRow(_tree, NULL, rowId++, "%d%s", 0, lightsStr.str().c_str());

			vector<CLight*>::iterator iter;

			for(iter=lights.begin() ; iter!=lights.end() ; iter++) {
				CLight* light = *iter;

				showOneLight(lightsRow, light, 1, rowId);
			}
		}
	}
	else {
		AG_TreetblAddCol(_tree, 0, "<-- Aucune MAP n'est ouverte -->", "-- Aucune MAP n'est ouverte --");
	}

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

void MapTreeView::showOneMaterial(AG_TreetblRow* parentRow, const CMaterial* mat, const int depth, int& rowId) {
	ostringstream materiauStr;

	switch(mat->Type()) {
	case CMaterial::MAT_TYPE_SIMPLE:
		materiauStr << "Materiau simple";
		break;
	case CMaterial::MAT_TYPE_TEXTURE:
		materiauStr << "Materiau texture";
		break;
	case CMaterial::MAT_TYPE_MULTI:
		materiauStr << "Materiau multiple";
		break;
	default:
		materiauStr << "Materiau inconnu";
		break;
	}

	AG_TreetblRow* materiauRow = AG_TreetblAddRow(_tree, parentRow, rowId++, "%d%s", depth, materiauStr.str().c_str());

	// Référence OpenGL
	ostringstream matRef;
	matRef << mat->getRef();
	AG_TreetblAddRow(_tree, materiauRow, rowId++, "%d%s%d%s", depth+1, "Ref. OpenGL : ", depth+2, matRef.str().c_str());

	// Lumière ambiante
	addFloat4TabToTree(_tree, materiauRow, rowId++, depth+1, "Ambiante : ", mat->m_Ambient);

	// Lumière diffuse
	addFloat4TabToTree(_tree, materiauRow, rowId++, depth+1, "Diffuse : ", mat->m_Diffuse);

	// Lumière spéculaire
	addFloat4TabToTree(_tree, materiauRow, rowId++, depth+1, "Speculaire : ", mat->m_Specular);

	switch(mat->Type()) {
	case CMaterial::MAT_TYPE_TEXTURE:
		{
			CMaterialTexture* texture = (CMaterialTexture*)mat;
			ostringstream textureId;
			textureId << texture->_texture->getGlTexName();
			AG_TreetblAddRow(_tree, materiauRow, rowId++, "%d%s%d%s", depth+1, "Fichier de texture : ", depth+2, texture->m_FichierTexture.c_str());
			AG_TreetblAddRow(_tree, materiauRow, rowId++, "%d%s%d%s", depth+1, "Identifiant de texture : ", depth+2, textureId.str().c_str());
		}
		break;
	case CMaterial::MAT_TYPE_MULTI:
		{
			CMaterialMulti* multi = (CMaterialMulti*)mat;

			for(int i=0 ; i<multi->NbrTex() ; i++) {
				CMaterial* sMat = multi->m_TabMat[i];
				showOneMaterial(materiauRow, sMat, depth+1, rowId);
			}
		}
		break;
	default:
		// Do nothing
		break;
	}
}

void MapTreeView::showOneLight(AG_TreetblRow* parentRow, const CLight* light, const int depth, int& rowId) {
	ostringstream lightStr;
	lightStr << "Lumiere";

	AG_TreetblRow* lightRow = AG_TreetblAddRow(_tree, parentRow, rowId++, "%d%s", depth, lightStr.str().c_str());

	AG_TreetblAddRow(_tree, lightRow, rowId++, "%d%s%d%s", depth+1, "Ref. OpenGL : ", light->getRefLight());

	// Position
	float position[4], ambiante[4], diffuse[4], speculaire[4];
	light->getPosition(position);
	light->getCouleurAmbiante(ambiante);
	light->getCouleurDiffuse(diffuse);
	light->getCouleurSpeculaire(speculaire);

	addFloat4TabToTree(_tree, lightRow, rowId++, depth+1, "Position : ", position);

	// Lumière ambiante
	addFloat4TabToTree(_tree, lightRow, rowId++, depth+1, "Lumiere ambiante : ", ambiante);

	// Lumière diffuse
	addFloat4TabToTree(_tree, lightRow, rowId++, depth+1, "Lumiere diffuse : ", diffuse);

	// Lumière spéculaire
	addFloat4TabToTree(_tree, lightRow, rowId++, depth+1, "Lumiere speculaire : ", speculaire);
}

AG_TreetblRow* MapTreeView::addFloat4TabToTree(AG_Treetbl* tree, AG_TreetblRow* parentRow, const int id, const int depth, const char* name, const float value[4]) {
	if(value) {
		ostringstream var;
		var << value[0] << "; " << value[1] << "; " << value[2] << "; " << value[3];

		return AG_TreetblAddRow(tree, parentRow, id, "%d%s%d%s", depth, name, depth+1, var.str().c_str());
	}
	else {
		return AG_TreetblAddRow(tree, parentRow, id, "%d%s%d%s", depth, name, depth+1, "--NULL--");
	}
}
