

#ifndef __JKT__UTILS_TRACE_H
#define __JKT__UTILS_TRACE_H

#include <string>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_thread.h"


/********************************************************************************
*  If you don't want to compile with the trace system activated,
*  so undefine TRACE_ACTIVE
********************************************************************************/

//#undef TRACE_ACTIVE
#define TRACE_ACTIVE

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

#ifdef TRACE_ACTIVE
	#define TRACE()	Donnees(__LINE__,__FILE__)
#else
	#define TRACE() "//"
#endif

#define NOM_FICHIER_TRACE		"jkt."
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
