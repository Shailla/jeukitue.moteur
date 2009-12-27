
#include <iostream>

#ifdef WIN32
	#include <io.h>
#endif
#include <stdarg.h>
#include <sstream>

using namespace std;

#include "TraceMethod.h"

#include "Trace.h"

CTraceMethod::CTraceMethod( int ligne, const char *nomFichier, int category, void *ptr, char *txt, ... )
{
	m_Category = category;
	m_Texte = txt;
	m_ClassePtr = ptr;
	m_NomFichier = nomFichier;
	m_Ligne = ligne;

	stringstream texte;

	va_list vl;
	bool prec = false;	// Indique que le caractère précédent était un %

	va_start( vl, txt );

	for( int i=0 ; txt[i]!='\0' ; i++ )
	{
		if( !prec )
 		{
			if( txt[i]=='%' )
				prec = true;
			else
				texte << txt[i];
		}
		else
		{
			switch( txt[i] )
			{
				case 'd':
				{
					int a = va_arg( vl, int );
					texte << a;
					break;
				}
				case 'b':
				{
#ifdef VS
					bool a = va_arg( vl, bool );
					if( a )
						texte << "true";
					else
						texte << "false";
					break;
#else
					int a = va_arg( vl, int );
					if( a )
						texte << "true";
					else
						texte << "false";
					break;
#endif
				}
				case 'f':
				{
#ifdef VS
					float a = va_arg( vl, float );
					texte <<  a;
					break;
#else
					double a = va_arg( vl, double );
					texte <<  a;
					break;
#endif
				}
				case 'x':
				{
					void *a = va_arg( vl, void* );
					texte <<  a;
					break;
				}
				case 's':
				{
					char *a = va_arg( vl, char* );
					texte <<  a;
					break;
				}
				case 'S':
				{
					cerr << endl << "Erreur TraMethod.cpp : NE PAS PASSER DE STRING PAR '...'" << endl;
					//string a = va_arg( vl, string );
					//texte <<  a;
					break;
				}
				case '%':
				{
					texte << '%';
					break;
				}
				default:
				{
					texte << "???";
					break;
				}
			}
			prec = false;
		}
	}

	va_end( vl );

	texte << "%T";

	Trace::instance().print_old( TRACE_BEGIN, ligne, nomFichier, category, texte.str().c_str(), ptr );
}

CTraceMethod::~CTraceMethod()
{
	Trace::instance().print_old( TRACE_END, m_Ligne, m_NomFichier.c_str(),
	m_Category, "%s%T", m_Texte.c_str(), m_ClassePtr );
}
