

#ifndef __JKT__UTILS_TRACE_H
#define __JKT__UTILS_TRACE_H

#include <string>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_thread.h"

enum TraceType {
	TRACE_NORMAL,
	TRACE_BEGIN,
	TRACE_END,
};

// If you don't want to activate the trace system, undefine TRACE_ACTIVE
//#undef TRACE_ACTIVE
#define TRACE_ACTIVE

#ifdef TRACE_ACTIVE
	#define TRACE()	Donnees(__LINE__,__FILE__)
#else
	#define TRACE() "//"
#endif

#define NOM_FICHIER_TRACE		"traces"
#define EXT_FICHIER_TRACE		".log"
#define EXT_XML_FICHIER_TRACE	".xml"
#define LEVEL_TRACE				-1

#define TRACE_ERROR			1
#define TRACE_INFO			2
#define TRACE_SON			4
#define TRACE_MOTEUR3D		8
#define TRACE_MENU			16
#define TRACE_UTILS			32
#define TRACE_RESEAU		64
#define TRACE_OTHER			128

class Donnees
{
	int m_Line;
	const char *m_NomFichier;
public:
	Donnees( int line, const char *nomFichier );
	void p(int indic, const char *txt , ... );
};

std::ofstream mFichier();

class Trace 
{
	static Trace *m_Instance;
	std::ofstream m_Fichier;
	std::ofstream m_FichierXML;
	SDL_mutex *m_Mutex;

		// Constructor / destructor
	Trace();
	~Trace();

public:
	static Trace &instance();
	void print( int type, int line, const char *nomFichier, int indic, const char *txt , va_list &vl );
	void print_old( int type, int line, const char *nomFichier, int indic, const char *txt, ... );
};

#endif
