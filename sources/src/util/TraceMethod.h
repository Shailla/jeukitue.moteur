#ifndef __JKT__UTILS_TRACEMETHOD_H
#define __JKT__UTILS_TRACEMETHOD_H

#include <string>

#define TRACEMETHOD() CTraceMethod t(__LINE__,__FILE__)

class CTraceMethod {
	int m_Ligne;
	std::string m_NomFichier;

public:
	CTraceMethod(int ligne, const char *nomFichier);
	~CTraceMethod();
};

#endif
