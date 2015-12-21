

#ifndef __JKT__UTILS_TRACE_H
#define __JKT__UTILS_TRACE_H

#include <string>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_thread.h"

void saveTime(Uint32& time);

void logTime(const char* msg, Uint32& time);


/********************************************************************************
*  If you don't want to compile with the trace system activated,
*  so undefine all TRACE_{level}_ACTIVE
********************************************************************************/

#define TRACE_DEBUG_ACTIVE
//#undef TRACE_DEBUG_ACTIVE

#define TRACE_INFO_ACTIVE
//#undef TRACE_INFO_ACTIVE

#define TRACE_WARN_ACTIVE
//#undef TRACE_WARN_ACTIVE

#define TRACE_ERROR_ACTIVE
//#undef TRACE_ERROR_ACTIVE

enum TraceLevel {
	TRACE_LEVEL_DEBUG = 1,
	TRACE_LEVEL_INFO = 2,
	TRACE_LEVEL_WARN = 3,
	TRACE_LEVEL_ERROR = 4
};

enum TraceType {
	TRACE_NORMAL,
	TRACE_BEGIN,
	TRACE_END,
};

#ifdef TRACE_DEBUG_ACTIVE
	#define LOGDEBUG(x)	Donnees(__LINE__,__FILE__).debug x
#else
	#define LOGDEBUG(x)
#endif

#ifdef TRACE_INFO_ACTIVE
	#define LOGINFO(x)	Donnees(__LINE__,__FILE__).info x
#else
	#define LOGINFO(x)
#endif

#ifdef TRACE_WARN_ACTIVE
	#define LOGWARN(x)	Donnees(__LINE__,__FILE__).warn x
#else
	#define LOGWARN(x)
#endif

#ifdef TRACE_ERROR_ACTIVE
	#define LOGERROR(x)	Donnees(__LINE__,__FILE__).error x
#else
	#define LOGERROR(x)
#endif

#define TRACE_FOLDER			"./log/"
#define CURRENT_FICHIER_TRACE	"jkt.current.log"
#define PREFIX_FICHIER_TRACE	"jkt."
#define EXT_FICHIER_TRACE		".log"
#define LEVEL_TRACE				2
#define FICHIER_SOURCE_BASE		"\\src\\"

class Donnees
{
	int _line;
	const char *_sourceFile;
	void p(TraceLevel level, const char *txt, ... );
public:
	Donnees(int line, const char *nomFichier);

	void debug(const char *txt, ... );
	void info(const char *txt, ... );
	void warn(const char *txt, ... );
	void error(const char *txt, ... );
};

std::ofstream mFichier();

class Trace 
{
	static Trace *m_Instance;
	std::ofstream m_Fichier;
	SDL_mutex *m_Mutex;

		// Constructor / destructor
	Trace();
	~Trace();

public:
	static Trace& instance();
	void print(TraceLevel level, TraceType type, int line, const char *nomFichier, const char *txt , va_list &vl );
};

#endif
