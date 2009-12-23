

#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>

#include "SDL.h"
#include "SDL_thread.h"

#include "FindFolder.h"

#include "Trace.h"

using namespace std;

Donnees::Donnees( int line, const char *nomFichier )
{
	m_Line = line;
	m_NomFichier = nomFichier;
}

void Donnees::p( int indic, const char *txt, ... )
{
	va_list vl;
	va_start( vl, txt );
	Trace::instance().print( TRACE_NORMAL, m_Line, m_NomFichier, indic, txt, vl );
}

Trace* Trace::m_Instance = 0;

Trace::Trace()
{
	m_Mutex = SDL_CreateMutex();
	SDL_mutexP( m_Mutex );

	string name;
	stringstream nomFichier, nomFichierXML;
	int numeroFichier = -1, num;

	char chemin[] = "./Traces/";
	
	CFindFolder folder( chemin, NOM_FICHIER_TRACE, EXT_FICHIER_TRACE );
	
	while( folder.findNext( name ) )
	{
		if( name.size() >= strlen(EXT_FICHIER_TRACE) )
		{
			name = string( name.begin()+strlen(NOM_FICHIER_TRACE), name.end()-strlen(EXT_FICHIER_TRACE) );	// Extraction du numéro du fichier trace

			if( name == "0" )
			{
				if( numeroFichier < 0 )
					numeroFichier = 0;
			}
			else 
			{
				num = atoi( name.c_str() );
				if( num > numeroFichier )
					numeroFichier = num;
			}
		}
	}

	CFindFolder folderXml( chemin, NOM_FICHIER_TRACE, EXT_XML_FICHIER_TRACE );
	
	while( folderXml.findNext(name) )
	{
		cout << endl << name << " ";
		name = string( name.begin()+strlen(NOM_FICHIER_TRACE), name.end()-strlen(EXT_XML_FICHIER_TRACE) );	// Extraction du numéro du fichier trace
		cout << name;

		if( name == "0" )
		{
			if( numeroFichier < 0 )
				numeroFichier = 0;
		}
		else 
		{
			num = atoi( name.c_str() );
			if( num > numeroFichier )
				numeroFichier = num;
		}
	}

	numeroFichier++;	// Numéro nouveau fichier = plus grand numéro d'ancien fichier + 1

		// Ouverture du fichier de trace
	nomFichier << "./Traces/" << NOM_FICHIER_TRACE << numeroFichier << EXT_FICHIER_TRACE;
	m_Fichier.open( nomFichier.str().c_str() );

		// Ouverture du fichier de trace en version XML
	nomFichierXML << "./Traces/" << NOM_FICHIER_TRACE << numeroFichier << EXT_XML_FICHIER_TRACE;
	m_FichierXML.open( nomFichierXML.str().c_str() );
	m_FichierXML << "<?xml version='1.0' encoding='utf-8'?>" << endl << endl << "<TracesJKT>" << endl;
	
	m_Instance = this;	
	
	SDL_mutexV( m_Mutex );
}

Trace::~Trace()
{
	SDL_mutexP( m_Mutex );

	cout << endl << "FIN DES TRACES" << endl;

	m_FichierXML << endl << endl << "</TracesJKT>" << flush;
	m_Fichier.flush();
	
	m_FichierXML.close();
	m_Fichier.close();	

	m_Instance = 0;

	SDL_DestroyMutex( m_Mutex );
}

Trace &Trace::instance()
{
	if( !m_Instance )
		m_Instance = new Trace();
	
	return *m_Instance;
}

void Trace::print_old( int type, int line, const char *nomFichier, int indic, const char *txt, ... )
{
	va_list vl;
	va_start( vl, txt );

	print( type, line, nomFichier, indic, txt, vl );
}

void Trace::print( int type, int line, const char *nomFichier, int indic, const char *txt, va_list &vl )
{
	SDL_mutexP( m_Mutex );

	if( (indic & LEVEL_TRACE) || (LEVEL_TRACE ==-1) )	// Si on ne veut pas cette trace-là alors sors
	{
		bool prec = false;	// Indique que le caractère précédent était un %
		stringstream ligne, ligne1, ligne2, ligne3, ligne4, ligne5;
		stringstream ligneXML;
		void *THIS = 0;
		bool bTHIS = false;

			// Début de la trace XML
		switch( type )
		{
		case TRACE_NORMAL:
			ligne1 << "Normal\t";
			ligneXML << "<normal" << flush;
			break;
		case TRACE_BEGIN:
			ligne1 << "Begin\t";
			ligneXML << "<begin" << flush;
			break;
		case TRACE_END:
			ligne1 << "End\t";
			ligneXML << "<end" << flush;
			break;
		}		

			// Insère le nom du fichier tracé
		ligne1 << nomFichier << "(" << line << ")" << flush;
		ligneXML << " fichier=" << '"' << nomFichier << '"' << flush;
		ligneXML << " ligne=" << '"' << line << '"' << flush;

		ligne.width( 28 );
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << ligne1.str() << flush;

			// Insère le temps relatif
		ligne2 << SDL_GetTicks();
		ligneXML << " temps=" << '"' << SDL_GetTicks() << '"' << flush;
		
		ligne.width( 10 );
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << ligne2.str() << flush;
		
			// Insère le numéro de thread et le pointeur this
		ligne3 << "(thread=" << SDL_ThreadID() << ",this=";
		ligneXML << " thread=" << '"' << SDL_ThreadID() << '"' << flush;
		ligneXML << " this=" << '"' << flush;

		if( bTHIS )
		{
			ligne3 << THIS << ")";
			ligneXML << THIS << '"';
		}
		else
		{
			ligne3 << ")";
			ligneXML << '"';
		}

		ligne.width( 20 );
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << ligne3.str() << flush;

		ligneXML << " package=" << '"' << flush;
		switch( indic )
		{
		case TRACE_ERROR:
			ligne4 << "TRACE_ERROR";
			ligneXML << "TRACE_ERROR" << '"';
			break;
		case TRACE_INFO:
			ligne4 << "TRACE_INFO";
			ligneXML << "TRACE_INFO" << '"';
			break;
		case TRACE_SON:
			ligne4 << "TRACE_SON";
			ligneXML << "TRACE_SON" << '"';
			break;
		case TRACE_MOTEUR3D:
			ligne4 << "TRACE_MOTEUR3D";
			ligneXML << "TRACE_MOTEUR3D" << '"';
			break;
		case TRACE_MENU:
			ligne4 << "TRACE_MENU";
			ligneXML << "TRACE_MENU" << '"';
			break;
		case TRACE_UTILS:
			ligne4 << "TRACE_UTILS";
			ligneXML << "TRACE_UTILS" << '"';
			break;
		case TRACE_RESEAU:
			ligne4 << "TRACE_RESEAU";
			ligneXML << "TRACE_RESEAU" << '"';
			break;
		case TRACE_OTHER:
			ligne4 << "TRACE_OTHER";
			ligneXML << "TRACE_OTHER" << '"';
			break;
		}

		ligne.width( 15 );
		ligne.setf( ios_base::left, ios_base::adjustfield );
		ligne << ligne4.str() << flush;

		for( int i=0 ; txt[i]!='\0' ; i++ )
		{
			if( !prec )
			{
				if( txt[i]=='%' )
					prec = true;
				else
					ligne5 << txt[i];
			}
			else
			{
				switch( txt[i] )
				{
					case 'd':
					{
						int a = va_arg( vl, int );
						ligne5 << a;
						break;
					}
					case 'b':
					{
#ifdef WIN32
						bool a = va_arg( vl, bool );
						if( a )
							ligne5 << "true";
						else
							ligne5 << "false";
						break;
#elif defined(__linux__)
						int a = va_arg( vl, int );
						if( a )
							ligne5 << "true";
						else
							ligne5 << "false";
						break;
#endif
					}
					case 'f':
					{
#ifdef WIN32
						float a = va_arg( vl, float );
						ligne5 <<  a;
						break;
#elif defined(__linux__)
						double a = va_arg( vl, double );
						ligne5 <<  a;
						break;
#endif
					}
					case 'x':
					{
						void *a = va_arg( vl, void* );
						ligne5 <<  a;
						break;
					}
					case 's':
					{
						char *a = va_arg( vl, char* );
						ligne5 <<  a;
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
						ligne5 << '%';
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
						ligne5 << "???";
						break;
					}
				}
				prec = false;
			}
		}
		
		ligne << ligne5.str() << endl << flush;
		ligneXML << " texte=" << '"' << ligne5.str() << '"' << flush;

		m_Fichier << ligne.str();
		m_FichierXML << ligneXML.str() << ">" << flush;

		switch( type )
		{
		case TRACE_NORMAL:
			m_FichierXML << "</normal>" << endl;
			break;
		case TRACE_BEGIN:
			m_FichierXML << "</begin>" << endl;
			break;
		case TRACE_END:
			m_FichierXML << "</end>" << endl;
			break;
		}

		va_end( vl );
	}

	SDL_mutexV( m_Mutex );
}
