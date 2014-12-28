#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>

#include "SDL.h"
#include "SDL_thread.h"

#include "FindFolder.h"

#include "Trace.h"

using namespace std;

Trace* Trace::m_Instance = NULL;

Donnees::Donnees(int line, const char *sourceFile) {
	_line = line;
	_sourceFile = sourceFile;
}

void Donnees::p(TraceLevel level, const char *txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(level, TRACE_NORMAL, _line, _sourceFile, txt, vl );
}

void Donnees::debug(const char *txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_NORMAL, _line, _sourceFile, txt, vl );
}

void Donnees::info(const char *txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_INFO, TRACE_NORMAL, _line, _sourceFile, txt, vl );
}

void Donnees::warn(const char *txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_WARN, TRACE_NORMAL, _line, _sourceFile, txt, vl );
}

void Donnees::error(const char *txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_ERROR, TRACE_NORMAL, _line, _sourceFile, txt, vl );
}

Trace::Trace() {
	m_Mutex = SDL_CreateMutex();
	SDL_LockMutex( m_Mutex );

	string name;
	stringstream nomFichier;
	int numeroFichier = -1, num;

	char chemin[] = "./log/";

	CFindFolder folder( chemin, NOM_FICHIER_TRACE, EXT_FICHIER_TRACE );

	while(folder.findNext(name)) {
		if(name.size() >= strlen(EXT_FICHIER_TRACE)) {
			name = string( name.begin()+strlen(NOM_FICHIER_TRACE), name.end()-strlen(EXT_FICHIER_TRACE) );	// Extraction du numéro du fichier trace

			if( name == "0" ) {
				if( numeroFichier < 0 )
					numeroFichier = 0;
			}
			else {
				num = atoi( name.c_str() );
				if( num > numeroFichier )
					numeroFichier = num;
			}
		}
	}

	numeroFichier++;	// Numéro nouveau fichier = plus grand numéro d'ancien fichier + 1

	// Ouverture du fichier de trace
	nomFichier << "./log/" << NOM_FICHIER_TRACE << numeroFichier << EXT_FICHIER_TRACE;
	m_Fichier.open( nomFichier.str().c_str() );

	m_Instance = this;

	SDL_UnlockMutex(m_Mutex);
}

Trace::~Trace() {
	SDL_LockMutex( m_Mutex );

	cout << endl << "FIN DES TRACES" << endl;

	m_Fichier.flush();

	m_Fichier.close();

	m_Instance = 0;

	SDL_DestroyMutex( m_Mutex );
}

Trace &Trace::instance() {
	if( !m_Instance )
		m_Instance = new Trace();

	return *m_Instance;
}

void Trace::print(TraceLevel level, TraceType type, int line, const char *nomFichier, const char *txt, va_list &vl) {
	SDL_LockMutex( m_Mutex );

	if( (level >= LEVEL_TRACE) || (LEVEL_TRACE ==-1) )	// Si on ne veut pas cette trace-là alors sors
	{
		bool prec = false;	// Indique que le caractère précédent était un %
		void *THIS = 0;
		bool bTHIS = false;

		stringstream ligne;

		// Niveau de log
		ligne.width(6);
		ligne.setf( ios_base::left, ios_base::adjustfield );

		switch(level) {
		case TRACE_LEVEL_DEBUG:
			ligne << "DEBUG";
			break;
		case TRACE_LEVEL_INFO:
			ligne << "INFO";
			break;
		case TRACE_LEVEL_WARN:
			ligne << "WARN";
			break;
		case TRACE_LEVEL_ERROR:
			ligne << "ERROR";
			break;
		}

		// Temps
		ligne.width( 10 );
		ligne.setf( ios_base::left, ios_base::adjustfield );

		ligne << SDL_GetTicks();

		// This et thread
		stringstream var1;

		if( bTHIS ) {
			var1 << "[" << SDL_ThreadID() << "-" << THIS << "]";
		}
		else {
			var1 << "[" << SDL_ThreadID() << "-0]";
		}

		ligne.width(15);
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << var1.str();

		// Fichier source et ligne
		string varFichier = nomFichier;
		size_t pos = varFichier.find(FICHIER_SOURCE_BASE);
		varFichier.replace(0, pos + sizeof(FICHIER_SOURCE_BASE)-1, "");

		stringstream var2;
		var2 << varFichier << "(" << line << ")";

		ligne.width(40);
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << var2.str();

		// Message précédé éventuellement par Begin ou End si c'est une TraceMethod
		switch( type ) {
		case TRACE_BEGIN:
			ligne << "Begin - ";
			break;
		case TRACE_END:
			ligne << "End - ";
			break;
		default:
		case TRACE_NORMAL:
			// nothing
			break;
		}

		for( int i=0 ; txt[i]!='\0' ; i++ ) {
			if( !prec ) {
				if( txt[i]=='%' )
					prec = true;
				else
					ligne << txt[i];
			}
			else {
				switch( txt[i] ) {
					case 'd':
					{
						int a = va_arg( vl, int );
						ligne << a;
						break;
					}
					case 'l':
					{
						int a = va_arg( vl, int );
						ligne << a;
						break;
					}
					case 'b':
					{
#ifdef VS
						bool a = va_arg( vl, bool );
#else
						int a = va_arg( vl, int );
#endif
						if( a )
							ligne << "true";
						else
							ligne << "false";
						break;
					}
					case 'f':
					{
#ifdef VS
						float a = va_arg( vl, float );
#else
						double a = va_arg( vl, double );
#endif
						ligne <<  a;
						break;
					}
					case 'x':
					{
						void *a = va_arg( vl, void* );
						ligne <<  a;
						break;
					}
					case 's':
					{
						char *a = va_arg( vl, char* );
						ligne <<  a;
						break;
					}
					case 'S':
					{
						cerr << endl << "Erreur TRACE.CPP : NE PAS PASSER DE STRING PAR '...'" << endl;
						//string a = va_arg( vl, string );
						//ligne5 <<  a;
						break;
					}
					case '%':
					{
						ligne << '%';
						break;
					}
					case 'T':	// Trace-t-on le pointeur de l'instance dans laquelle on est ?
					{
						void *a = va_arg( vl, void* );
						THIS = a;
						bTHIS = true;
						break;
					}
					default:
					{
						ligne << "???";
						break;
					}
				}
				prec = false;
			}
		}

		ligne << endl << flush;

		m_Fichier << ligne.str();

		va_end( vl );
	}

	SDL_UnlockMutex(m_Mutex);
}
