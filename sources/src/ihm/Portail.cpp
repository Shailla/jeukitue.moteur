

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

#include "util/Trace.h"
#include "spatial/IfstreamMap.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "ihm/Dlg.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

void quit_game( int code );

namespace JktMenu
{

void quit_JKTici(void *var)
{
TRACE().p( TRACE_MENU, "quit_JKTici(%x)", var );
	quit_game( 0 );
}

#include "ihm/Portail.h"

}	// JktMenu
