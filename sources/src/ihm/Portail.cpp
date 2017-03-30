

#include <fstream>
#include "SDL.h"

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

#include "ihm/Portail.h"

using namespace std;

namespace jkt
{

void quit_JKTici(void *var)
{
LOGDEBUG(("quit_JKTici(%x)", var));
	quit_game("Qui game from old gui action", 0);
}

}	// JktMenu
