	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
	#include <direct.h>
#endif

using namespace std;

#include "SDL.h"
#include "SDL_thread.h"

#include "util/FindFolder.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/Map.h"
#include "main/Cfg.h"
#include "spatial/FichierASE.h"
#include "ihm/MMenuPrinc.h"

#include "MOpenScene.h"

#include "ihm/MOpenASE.h"

using namespace JktMenu;
using namespace JktMoteur;

bool deprecatedOpenMAP( const void *arg );

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;
JktMoteur::CMap *pMapASE;

namespace JktMenu
{

	/************************/
	/*						*/
	/*   MENU OPEN ASE		*/
	/*						*/
	/************************/
	// Prototypes
int threadConvertASE_2( void *arg );

CMenu *MenuOpenASE;
CDlgBoite *BoiteConfirmOk;
CDlgBoite *BoiteErreur;
CDlgBoite *BoiteEcraseRep;
CDlgBoite *BoiteConvertASE;

void retourASE(void *var)	// Lib�rations m�moire et retour au menu sup�rieur
{
TRACE().p( TRACE_MENU, "retourASE(var=%x)", var );
	delete MenuOpenASE;
	CDlg::SetMenuActif( 0 );

	lanceMenuOpenScene( 0 );
}

bool copieTexture( CMaterialTexture *mat, CMap *pMapASE, string &nomRep )
{
	basic_string <char>::size_type index, npos = (basic_string <char>::size_type)-1;
	char ch;

	TRACE().p( TRACE_MOTEUR3D, "copieTexture(mat=%x,pMapASE=%x,nomRep=%s)",
	mat, pMapASE, nomRep.c_str() );
	TRACE().p( TRACE_MOTEUR3D, "copieTexture() Fichier de texture=%s",
	mat->m_FichierTexture.c_str() );

	fstream from;	// Fichier source pour la copie du fichier de texture
	fstream to;		// Fichier destination pour la copie du fichier de texture

	string& nom = mat->m_FichierTexture;
	cout << "\nCopie du fichier de texture : " << nom << " (materiau ref " << mat->getRef() << ")";

	from.open( nom.c_str(), ios_base::binary|ios_base::in );	// Ouvre le fichier texture d'origine
	if( from.fail() )
	{
		TRACE().p( TRACE_ERROR, "copieTexture() Echec d'ouverture du fichier de texture" );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec d'ouverture du fichier de texture (" << nom << ")";
		return false;	// Les choses se sont mal pass�es pour la conversion du fichier
	}
		// R�cup�ration du nom du fichier sans son chemin
	index = nom.find_last_of( "/" );		// Recherche du dernier slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

		// R�cup�ration du nom du fichier sans son chemin
	index = nom.find_last_of( "\\" );		// Recherche du dernier anti-slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

		// Nouveau nom avec chemin du fichier de texture
	nom = nomRep + "/" + nom;

	to.open( nom.c_str(), ios_base::binary|ios_base::out );	// Ouvre fichier destination texture
	if( to.fail() )
	{
		TRACE().p( TRACE_ERROR, "copieTexture() Echec de creation du fichier de texture" );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec de creation du fichier de texture ( " << nom << " )";
		return false;	// Les choses se sont mal pass�es pour la conversion du fichier
	}

		// Copie du fichier proprement dite
	while( from.get( ch ) )
		to.put( ch );

		// V�rifie si la copie s'est bien pass�
	if( !from.eof() )
	{
		TRACE().p( TRACE_ERROR, "copieTexture() Echec de copie du fichier de texture" );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec de copie du fichier de texture (" << nom << ")";
		return false;	// La conversion n'a pas abouti
	}

TRACE().p( TRACE_MOTEUR3D, "copieTexture() Ok" );

	return true;
}

void retourConfirmNon( void *arg )
{
TRACE().p( TRACE_MENU, "retourConfirmNon(var=%x)", arg );
	delete BoiteConfirmOk;
	lanceMenuOpenScene( 0 );
}

void retourConfirmOui( void *arg )
{
TRACE().p( TRACE_MENU, "retourConfirmOui(var=%x)", arg );
	delete BoiteConfirmOk;

	if(deprecatedOpenMAP(arg)) {
		Aide = false;
		pFocus->SetPlayFocus();
		Game.setModeLocal();
	}
	else {
		CDlgBoite* BoiteEchec = new CDlgBoite(" Echec lecture fichier Map", "La lecture du fichier Map a echoue", lanceMenuPrinc, CDlgBoite::JKT_DLG_ERREUR);
		BoiteEchec->addBouton( 2, "Ok", lanceMenuPrinc );

		CDlg::SetMenuActif( BoiteEchec );
	}
}

void confirmOk( void *arg )
{
	BoiteConfirmOk = new CDlgBoite( "Confirmation", "La conversion du fichier ASE a reussi. Voulez-vous lancer cette MAP ?", retourConfirmNon, CDlgBoite::JKT_DLG_CONFIRM );
	BoiteConfirmOk->addBouton( 1, "Oui", retourConfirmOui, arg );
	BoiteConfirmOk->addBouton( 3, "Non", retourConfirmNon );
	CDlg::SetMenuActif( BoiteConfirmOk );
}

void retourErreur( void *arg )
{
TRACE().p( TRACE_MENU, "retourErreur(var=%x)", arg );
	delete BoiteErreur;
	lanceMenuOpenScene( 0 );
}

void erreur(const char *msg )
{
	BoiteErreur = new CDlgBoite( "Erreur", msg, retourErreur, CDlgBoite::JKT_DLG_ERREUR );
	BoiteErreur->addBouton( 2, "Ok", retourErreur );

	CDlg::SetMenuActif( BoiteErreur );
}

void retourEcraserRep( void *arg )
{
TRACE().p( TRACE_MENU, "retourEcraserRep(var=%x)", arg );
	delete BoiteEcraseRep;
	lanceMenuOpenScene( 0 );
}

bool deleteOnlyFiles(string &fichier, const string &path) {
	string var;

	if((fichier!="..") && (fichier!=".")) {		// Si ni "..", ni "."
		var = path + fichier;
		if( !CFindFolder::isFolder(var) ) {		// Si ce n'est pas un r�pertoire (=>fichier)
			cout << "\nDeleting file : " << var;

			if(remove(var.c_str()))
				return false;
		}
	}

	return true;
}

bool delDirectory(const string &path) {
	cout << "\n\nPATH\t : " << path;
	string var;
//	struct _finddata_t fileinfo;
//	intptr_t hFile;
	string fichier;

	CFindFolder folder(path.c_str(), 0, 0);

	while(folder.findNext(fichier)) {
		if(!deleteOnlyFiles(fichier, path))
			return false;
	}

		// Effacement de tous les fichiers du r�pertoire
/*	var = path + "*.*";
	hFile = _findfirst( var.c_str(), &fileinfo );
	if( hFile!=-1L )
	{
		if( !deleteOnlyFiles( fileinfo, path ) )
			return false;

		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			if( !deleteOnlyFiles( fileinfo, path ) )
				return false;
		}
	}*/

	folder.reset();

	while( folder.findNext(fichier) )
	{
		if( (fichier!="..") && (fichier!=".") )	// Si ni "..", ni "."
		{
			var = path + fichier;
			var += '/';

			if( !delDirectory( var ) )
				return false;

			if( !CFindFolder::chmod( var.c_str(), true, true ) )
				cerr << endl << __FILE__ << ":" << __LINE__ << " MOpenASE::delDirectory(" << path << ")" << endl;

			if( CFindFolder::rmdir( var.c_str() ) ) {
				cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur _rmdir : " << var;
				return false;
			}
		}
	}

	return true;
}

void yesEcraseRep(void *arg) {
	string var;
	string path = string("./Map/").append((char*)arg).append("/");

	delete BoiteEcraseRep;
	CDlg::SetMenuActif( BoiteConvertASE );

	if(!delDirectory(path)) {	// Efface r�cursivement tout ce qui se trouve dans le r�pertoire path
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Au moins une entree ne peut etre supprimee";
		erreur( "Au moins une entree ne peut etre supprime" );
	}
	else {
		SDL_CreateThread( threadConvertASE_2, arg );
	}
}

void noEcraseRep( void *arg )
{
	delete pMapASE;
	lanceMenuConvertASE( 0 );
	delete BoiteEcraseRep;
}

void ecraserRepOuiNon( void *arg )
{
	BoiteEcraseRep = new CDlgBoite( "Question", "Voulez-vous ecraser le repertoire existant ?", retourErreur, CDlgBoite::JKT_DLG_NULL );
	BoiteEcraseRep->addBouton( 1, "Oui", yesEcraseRep, arg );
	BoiteEcraseRep->addBouton( 3, "Non", noEcraseRep );
	CDlg::SetMenuActif( BoiteEcraseRep );
}

int threadConvertASE_1(void *arg)
{
	string nomFichierASE = (char*)arg;

	BoiteConvertASE = new CDlgBoite( "Tache en cours", "Ouverture du fichier ASE en cours...", lanceMenuConvertASE, CDlgBoite::JKT_DLG_ENCOURS );
	CDlg::SetMenuActif( BoiteConvertASE );

	pMapASE = new CMap(0);		// Cr�e une classe pour recevoir les donn�es de la map

	// Conversion fichier ASE -> fichier Map
	if(	!CFichierASE::LitFichierASE(nomFichierASE.c_str(), pMapASE, Config.Debug.bAfficheFichier ) )	// Lit le fichier ASE de la map
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Lecture du fichier ASE impossible ou fichier corrompu 2";
		delete pMapASE;

		erreur( "Lecture du fichier ASE impossible ou fichier corrompu 2" );

		return 0;	// Les choses se sont mal pass�es pour la conversion du fichier
	}

	pMapASE->EchangeYZ();						// Inversion des coordonn�es Y et Z
	pMapASE->Scale( -1.0f, 1.0f, 1.0f );

		// Transfert des fichiers de texture dans le r�pertoire du fichier Map
	string nomRep = "./Map/" + nomFichierASE;

	pMapASE->Save(nomFichierASE);

	if( CFindFolder::mkdir( nomRep.c_str() )!=0 )	// Cr�ation du r�pertoire pour les textures
	{		// Si un r�pertoire existe d�j�, demande s'il faut l'�craser
		ecraserRepOuiNon( arg );
	}
	else
	{		// Sinon passe imm�diatement � l'�tape 2 de la conversion
		SDL_CreateThread( threadConvertASE_2, arg );
	}

	return 0;	// Fin de la premi�re �tape de conversion
}

int threadConvertASE_2( void *arg )
{
	string nomFichierASE = (char*)arg;
	string nomRep = "./Map/" + nomFichierASE;

	CMaterial *mat;
	vector<CMaterial*>::iterator iter;
	for( iter=pMapASE->m_TabMaterial.begin() ; iter!=pMapASE->m_TabMaterial.end() ; iter++ )
	{
		mat = *iter;
		if( mat->Type()==CMaterial::MAT_TYPE_TEXTURE )
		{
			CMaterialTexture *matRef = (CMaterialTexture*)mat;
			if( !copieTexture( matRef, pMapASE, nomRep ) )
			{
				erreur( "Erreur a la copie d'un fichier de texture" );
				delete pMapASE;
				return 0;
			}
		}
		else if( mat->Type()==CMaterial::MAT_TYPE_MULTI )
		{
			CMaterialMulti *matMulti = (CMaterialMulti *)mat;
			for( int i=0 ; i<matMulti->NbrTex() ; i++ )
			{
				if( matMulti->m_TabMat[i]->Type()==CMaterial::MAT_TYPE_TEXTURE )
				{
					if( !copieTexture( (CMaterialTexture*)matMulti->m_TabMat[i], pMapASE, nomRep ) )
					{
						erreur("Erreur a la copie d'un fichier de texture");
						delete pMapASE;
						return 0;
					}
				}
			}
		}
	}

	delete pMapASE;

	confirmOk( arg );

	return 0;
}

void suivantASE(void *arg)
{
	SDL_CreateThread( threadConvertASE_1, arg );
}

void lanceMenuConvertASE(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConvertASE(var=%x)", var );
	cout << "\nEst-ce que t'es l�";
//	struct _finddata_t fileinfo;
//	intptr_t hFile;
	int nbrFichier = 0;		// Nombre de fichiers ASE � prendre en compte
	string name;

	PF *liste_suivant_open_ASE;
	char **item_menu_open_ASE;
	void **liste_argument_open_ASE;

	CFindFolder folder( "./ASE/", 0, ".ASE" );
	nbrFichier = folder.nbr();

//	hFile = _findfirst( "./ASE/*.ASE", &fileinfo );
//	if( hFile!=-1L )
//	{
//		nbrFichier = 1;
//		while( _findnext( hFile, &fileinfo )!=-1L )
//			nbrFichier++;
//	}
//
//	_findclose( hFile );

	liste_suivant_open_ASE = new PF[ nbrFichier ];
	liste_argument_open_ASE = new void*[ nbrFichier ];
	item_menu_open_ASE = new char*[ nbrFichier ];

	nbrFichier = 0;
	folder.reset();

	while( folder.findNext(name) )
	{
		name.erase( name.find_last_of( "." ) );
		liste_suivant_open_ASE[nbrFichier] = suivantASE;

		item_menu_open_ASE[nbrFichier] = new char[name.size()+1];
		liste_argument_open_ASE[nbrFichier] = new char[name.size()+1];
		strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_open_ASE[nbrFichier]), name.c_str() );
		nbrFichier++;
	}
//	hFile = _findfirst( "./ASE/*.ASE", &fileinfo );
//	if( hFile!=-1L )
//	{
//		name = fileinfo.name;
//		name.erase( name.find_last_of( "." ) );
//		liste_suivant_open_ASE[nbrFichier] = suivantASE;
//
//		item_menu_open_ASE[nbrFichier] = new char[name.size()+1];
//		liste_argument_open_ASE[nbrFichier] = new char[name.size()+1];
//		strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
//		strcpy( (char*)(liste_argument_open_ASE[nbrFichier]), name.c_str() );
//		nbrFichier++;
//		while( _findnext( hFile, &fileinfo )!=-1L )
//		{
//			name = fileinfo.name;
//			name.erase( name.find_last_of( "." ) );
//
//			liste_suivant_open_ASE[nbrFichier] = suivantASE;
//
//			item_menu_open_ASE[nbrFichier] = new char[name.size()+1];
//			liste_argument_open_ASE[nbrFichier] = new char[name.size()+1];
//			strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
//			strcpy( (char*)liste_argument_open_ASE[nbrFichier], name.c_str() );
//			nbrFichier++;
//		}
//	}

	MenuOpenASE = new CMenu( "Convertir une Scene ASE", (const char**)item_menu_open_ASE, nbrFichier,
						liste_suivant_open_ASE, retourASE, liste_argument_open_ASE );

	delete[] liste_suivant_open_ASE;
	delete[] liste_argument_open_ASE;
	delete[] item_menu_open_ASE;

	CDlg::SetMenuActif( MenuOpenASE );
}

}	// JktMenu
