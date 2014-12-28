

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

#include "main/divers.h"
#include "util/Trace.h"
#include "spatial/IfstreamMap.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "ihm/Dlg.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

namespace JktMenu
{

void quit_JKTici(void *var)
{
TRACE().debug("quit_JKTici(%x)", var);
	quit_game("Qui game from old gui action", 0);
}

#include "ihm/Portail.h"

}	// JktMenu
