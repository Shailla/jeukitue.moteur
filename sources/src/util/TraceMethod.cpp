
#include <iostream>

#ifdef WIN32
	#include <io.h>
#endif
#include <stdarg.h>
#include <sstream>

#include "TraceMethod.h"

#include "Trace.h"

using namespace std;

CTraceMethod::CTraceMethod( int ligne, const char *nomFichier) {
	m_NomFichier = nomFichier;
	m_Ligne = ligne;

	va_list vl;

	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_BEGIN, ligne, nomFichier, "", vl);
}

CTraceMethod::~CTraceMethod() {
	va_list vl;

	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_END, m_Ligne, m_NomFichier.c_str(), "", vl);
}
