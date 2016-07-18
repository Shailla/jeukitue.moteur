#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include <map>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

#include "SDL.h"
#include "SDL_thread.h"

#include "FindFolder.h"
#include "StringUtils.h"

#include "Trace.h"

using namespace std;
using namespace JktUtils;

Trace* Trace::_Instance = NULL;

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
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::info(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_INFO, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::warn(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_WARN, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::error(const string& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_ERROR, TRACE_NORMAL, _line, _sourceFile, txt.c_str(), vl );
}

void Donnees::debug(const std::stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_DEBUG, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::info(const std::stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_INFO, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::warn(const std::stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_WARN, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Donnees::error(const std::stringstream& txt, ... ) {
	va_list vl;
	va_start(vl, txt);
	Trace::instance().print(TraceLevel::TRACE_LEVEL_ERROR, TRACE_NORMAL, _line, _sourceFile, txt.str().c_str(), vl );
}

void Trace::loadConfigKey(map<string, string>& config, const char* key, string& value) {
	// Fichier à exclure des log
	string var = config[key];

	if(var.length() > 0) {
		value = var;
	}
}

void Trace::loadConfigKeyCommaSeparated(map<string, string>& config, const char* key, vector<string>& value) {
	// Fichier à exclure des log
	string var = config[key];

	if(var.length() > 0) {
		value = StringUtils::split(var, StringUtils::isComma);

		for(string& file : value) {
			StringUtils::trim(file);
		}
	}
}

Trace::Trace() {
	_Mutex = SDL_CreateMutex();
	SDL_LockMutex( _Mutex );

	/* ***********************************************
	 * Lecture de la politique de log
	 * **********************************************/

	std::ifstream logConfig("log.ini");

	if(!logConfig) {
		cerr << endl << "Echec d'ouverture du fichier log.ini";
	}

	map<string, string> config;

	if(logConfig) {
		string line, key;

		// Lit les clé et valeurs dans le fichier de configuration des log
		while(getline(logConfig, line)) {
			string key, value;
			StringUtils::splitOnce(line, StringUtils::isEqual, key, value);

			StringUtils::trim(key);
			StringUtils::trim(value);

			config[key] = value;
		}

		logConfig.close();

		// Niveau minimum de log
		string var;
		loadConfigKey(config, "logger.level", var);
		_loggerLevel = (TraceLevel)atoi(var.c_str());

		// Fichier à exclure des log
		loadConfigKeyCommaSeparated(config, "logger.file.exclude.debug", _excludeDebugFiles);
		loadConfigKeyCommaSeparated(config, "logger.file.exclude.info", _excludeInfoFiles);
		loadConfigKeyCommaSeparated(config, "logger.file.exclude.warn", _excludeWarnFiles);
		loadConfigKeyCommaSeparated(config, "logger.file.exclude.error", _excludeErrorFiles);
	}

	/* ***********************************************
	 * Création du fichier de log
	 * **********************************************/

	// Nom du fichier de log courrant
	std::stringstream logFile;
	logFile << TRACE_FOLDER << CURRENT_FICHIER_TRACE;

	// Si un fichier de traces existe déjà, alors renomme-le puis tente à nouveau de l'ouvrir
	if( std::ifstream( logFile.str().c_str()) ) {
		path logPath(TRACE_FOLDER);

		if(!exists(logPath)) {
			cerr << endl << "Le répertoire '" << TRACE_FOLDER << "' n'existe pas";
			return;
		}

		// Recherche un index d'ancien fichier de traces disponible
		int numeroFichier = -1, num;
		directory_iterator end_itr; // default construction yields past-the-end

		for ( directory_iterator itr( logPath ); itr != end_itr; ++itr ) {
			if ( is_directory( *itr ) ) {
				// Ignore directories
			}
			else {
				string filename = itr->path().filename().string();

				if( JktUtils::StringUtils::isBeginingWith(filename, PREFIX_FICHIER_TRACE) && JktUtils::StringUtils::isFinishingWith(filename, EXT_FICHIER_TRACE) ) {
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
		}

		numeroFichier++;	// Numéro nouveau fichier = plus grand numéro d'ancien fichier + 1

		stringstream newName;
		newName << TRACE_FOLDER << PREFIX_FICHIER_TRACE << numeroFichier << EXT_FICHIER_TRACE;

		// Renomme ancien fichier courant
		boost::filesystem::rename(logFile.str(), newName.str());
	}

	// Ouvre le fichier de log courant
	_Fichier.open( logFile.str().c_str() );

	_Instance = this;

	SDL_UnlockMutex(_Mutex);
}

Trace::~Trace() {
	SDL_LockMutex( _Mutex );

	cout << endl << "FIN DES TRACES" << flush << endl;

	_Fichier.flush();
	_Fichier.close();

	_Instance = 0;

	SDL_DestroyMutex( _Mutex );
}

Trace &Trace::instance() {
	if( !_Instance )
		_Instance = new Trace();

	return *_Instance;
}

void Trace::print(TraceLevel level, TraceType type, int line, const char *nomFichier, const char *txt, va_list &vl) {
	SDL_LockMutex( _Mutex );

	bool youHaveToLog = (level >= _loggerLevel);	// Si on ne veut pas cette trace-là alors sors
	string nomCourtFichier;

	// Vérifie si le fichier n'est pas exclu des log
	if(youHaveToLog) {
		nomCourtFichier = nomFichier;
		size_t pos = nomCourtFichier.find(FICHIER_SOURCE_BASE);
		nomCourtFichier.replace(0, pos + sizeof(FICHIER_SOURCE_BASE)-1, "");

		vector<string>* excludeFiles;

		switch(level) {
		case TraceLevel::TRACE_LEVEL_DEBUG:
			excludeFiles = &_excludeDebugFiles;
			break;
		case TraceLevel::TRACE_LEVEL_INFO:
			excludeFiles = &_excludeInfoFiles;
			break;
		case TraceLevel::TRACE_LEVEL_WARN:
			excludeFiles = &_excludeWarnFiles;
			break;
		case TraceLevel::TRACE_LEVEL_ERROR:
			excludeFiles = &_excludeErrorFiles;
			break;
		default:
			excludeFiles = 0;
			break;
		}

		if(excludeFiles) {
			for(const string& var : *excludeFiles) {
				if(var == nomCourtFichier) {
					youHaveToLog = false;
					break;
				}
			}
		}
	}

	if(youHaveToLog) {
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
		stringstream var2;
		var2 << nomCourtFichier << "(" << line << ")";

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
				case 'c':
				{
					char a = va_arg( vl, int );
					ligne << a;
					break;
				}
				case 'd':
				{
					int a = va_arg( vl, int );
					ligne << a;
					break;
				}
				case 'l':
				{
					long a = va_arg( vl, long );
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

		_Fichier << ligne.str() << flush;

		va_end( vl );
	}

	SDL_UnlockMutex(_Mutex);
}
