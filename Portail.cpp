

#include <fstream>
#include "SDL.h"

using namespace std;

class CLight;
class CMaterial;
class CGeo;
class CMouve;
class CMap;
class CGame;
class CGeoObject;
class CPorte;

#include "Trace.h"
#include "IfstreamMap.h"
#include "Focus.h"
#include "Cfg.h"
#include "Dlg.h"
#include "DlgBoite.h"
#include "Menu.h"

extern bool Aide;
extern CCfg Config;

void quit_JKT();
void lanceMenuConfig(void *var);		// Menu de configuration vid�o, son, r�seau, clavier,...
void lanceMenuOpenScene(void *var);		// Menu d'ouverture fichiers ASE et MAP
void lanceMenuMultijoueur(void *var);	// Menu pour le jeu multi-joueurs
void lanceMenuTest(void *var);
void quit_tutorial( int code );

namespace JKT_PACKAGE_MENU
{
extern CFocus *pFocus;

void quit_JKTici(void *var)
{
TRACE().p( TRACE_MENU, "quit_JKTici(%x)", var );
	quit_tutorial( 0 );
}

#include "Portail.h"

}	// JKT_PACKAGE_MENU