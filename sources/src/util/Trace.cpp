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

void saveTime(Uint32& time) {
	time = SDL_GetTicks();
}

void logTime(const char* msg, Uint32& time) {
	Uint32 newTime = SDL_GetTicks();
	Uint32 duree = newTime - time;

	if(duree > 0) {
		std::cout << std::endl << "TIME-" << msg << ": " << duree;
	}

	time = newTime;
}

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

void Donnees::debug(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt.c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::info(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt.c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_INFO, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::warn(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt.c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_WARN, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::error(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt.c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_ERROR, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::debug(const stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt.str().c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::info(const stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt.str().c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_INFO, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::warn(const stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt.str().c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_WARN, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::error(const stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt.str().c_str());
	Trace::instance().print(TraceLevel::TRACE_LEVEL_ERROR, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}





Trace::Trace() {
	m_Mutex = SDL_CreateMutex();
	SDL_LockMutex( m_Mutex );

	// Nom du fichier de log courrant
	stringstream logFile;
	logFile << TRACE_FOLDER << CURRENT_FICHIER_TRACE;

	// Si un fichier de traces existe déjà, alors renomme-le puis tente à nouveau de l'ouvrir
	if( ifstream( logFile.str().c_str()) ) {
		// Recherche un index d'ancien fichier de traces disponible
		CFindFolder folder( TRACE_FOLDER, PREFIX_FICHIER_TRACE, EXT_FICHIER_TRACE );

		int numeroFichier = -1, num;
		string filename;

		while(folder.findNext(filename)) {
			if(filename.size() >= strlen(EXT_FICHIER_TRACE)) {
				filename = string( filename.begin()+strlen(PREFIX_FICHIER_TRACE), filename.end()-strlen(EXT_FICHIER_TRACE) );	// Extraction du numéro du fichier trace

				if( filename == "0" ) {
					if( numeroFichier < 0 )
						numeroFichier = 0;
				}
				else {
					num = atoi( filename.c_str() );
					if( num > numeroFichier )
						numeroFichier = num;
				}
			}
		}

		numeroFichier++;	// Numéro nouveau fichier = plus grand numéro d'ancien fichier + 1

		stringstream newName;
		newName << TRACE_FOLDER << PREFIX_FICHIER_TRACE << numeroFichier << EXT_FICHIER_TRACE;

		// Renomme ancien fichier courant
		rename(logFile.str().c_str(), newName.str().c_str());
	}

	// Ouvre le fichier de log courant
	m_Fichier.open( logFile.str().c_str() );

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

		m_Fichier << ligne.str() << flush;

		va_end( vl );
	}

	SDL_UnlockMutex(m_Mutex);
}
