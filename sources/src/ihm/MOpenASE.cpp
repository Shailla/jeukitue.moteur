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
#include <boost/filesystem/operations.hpp> // includes boost/filesystem/path.hpp
#include <boost/filesystem/fstream.hpp>                       // for std::cout
using namespace boost::filesystem;

#include <SDL.h>
#include <SDL_thread.h>

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
#include "service/MapService.h"
#include <service/dto/AseInformationDto.h>
#include "MOpenScene.h"

#include "ihm/MOpenASE.h"

using namespace std;
using namespace jkt;

bool deprecatedOpenMAP( const void *arg );

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;
jkt::CMap *pMapASE;

namespace jkt
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

void retourASE(void *var) {	// Libérations mémoire et retour au menu supérieur
LOGDEBUG(("retourASE(var=%x)", var));
	delete MenuOpenASE;
	CDlg::SetMenuActif( 0 );

	lanceMenuOpenScene( 0 );
}

bool copieTexture( CMaterialTexture *mat, CMap *pMapASE, string &nomRep ) {
	basic_string <char>::size_type index, npos = (basic_string <char>::size_type)-1;
	char ch;

	LOGDEBUG(("copieTexture(mat=%x,pMapASE=%x,nomRep=%s)", mat, pMapASE, nomRep.c_str() ));
	LOGDEBUG(("copieTexture() Fichier de texture=%s", mat->m_FichierTexture.c_str() ));

	std::fstream from;		// Fichier source pour la copie du fichier de texture
	std::fstream to;		// Fichier destination pour la copie du fichier de texture

	string& nom = mat->m_FichierTexture;
	cout << "\nCopie du fichier de texture : " << nom << " (materiau ref " << mat->getRef() << ")";

	from.open( nom.c_str(), ios_base::binary|ios_base::in );	// Ouvre le fichier texture d'origine

	if( from.fail() ) {
		LOGDEBUG(("copieTexture() Echec d'ouverture du fichier de texture"));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec d'ouverture du fichier de texture (" << nom << ")";
		return false;	// Les choses se sont mal pass�es pour la conversion du fichier
	}
	// Récupération du nom du fichier sans son chemin
	index = nom.find_last_of( "/" );		// Recherche du dernier slach

	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

	// Récupération du nom du fichier sans son chemin
	index = nom.find_last_of( "\\" );		// Recherche du dernier anti-slach

	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

	// Nouveau nom avec chemin du fichier de texture
	nom = nomRep + "/" + nom;

	to.open( nom.c_str(), ios_base::binary|ios_base::out );	// Ouvre fichier destination texture
	if( to.fail() ) {
		LOGDEBUG(("copieTexture() Echec de creation du fichier de texture"));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec de creation du fichier de texture ( " << nom << " )";
		return false;	// Les choses se sont mal pass�es pour la conversion du fichier
	}

	// Copie du fichier proprement dite
	while( from.get( ch ) )
		to.put( ch );

	// Vérifie si la copie s'est bien pass�
	if( !from.eof() ) {
		LOGDEBUG(("copieTexture() Echec de copie du fichier de texture"));
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Echec de copie du fichier de texture (" << nom << ")";
		return false;	// La conversion n'a pas abouti
	}

LOGDEBUG(("copieTexture() Ok"));

	return true;
}

void retourConfirmNon( void *arg ) {
LOGDEBUG(("retourConfirmNon(var=%x)", arg));
	delete BoiteConfirmOk;
	lanceMenuOpenScene( 0 );
}

void retourConfirmOui( void *arg ) {
LOGDEBUG(("retourConfirmOui(var=%x)", arg));
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

void confirmOk( void *arg ) {
	BoiteConfirmOk = new CDlgBoite( "Confirmation", "La conversion du fichier ASE a reussi. Voulez-vous lancer cette MAP ?", retourConfirmNon, CDlgBoite::JKT_DLG_CONFIRM );
	BoiteConfirmOk->addBouton( 1, "Oui", retourConfirmOui, arg );
	BoiteConfirmOk->addBouton( 3, "Non", retourConfirmNon );
	CDlg::SetMenuActif( BoiteConfirmOk );
}

void retourErreur( void *arg ) {
LOGDEBUG(("retourErreur(var=%x)", arg));
	delete BoiteErreur;
	lanceMenuOpenScene( 0 );
}

void erreur(const char *msg ) {
	BoiteErreur = new CDlgBoite( "Erreur", msg, retourErreur, CDlgBoite::JKT_DLG_ERREUR );
	BoiteErreur->addBouton( 2, "Ok", retourErreur );

	CDlg::SetMenuActif( BoiteErreur );
}

void retourEcraserRep( void *arg ) {
	LOGDEBUG(("retourEcraserRep(var=%x)", arg));
	delete BoiteEcraseRep;
	lanceMenuOpenScene( 0 );
}

void yesEcraseRep(void *arg) {
	string var;
	string path = string("./Map/").append((char*)arg).append("/");

	delete BoiteEcraseRep;
	CDlg::SetMenuActif( BoiteConvertASE );

	if(!remove_all(path)) {	// Efface r�cursivement tout ce qui se trouve dans le r�pertoire path
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

	pMapASE = new CMap(0);		// Crée une classe pour recevoir les données de la map

	// Conversion fichier ASE -> fichier Map
	if(	!CFichierASE::LitFichierASE(nomFichierASE.c_str(), pMapASE, Config.Debug.bAfficheFichier ) )	// Lit le fichier ASE de la map
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Lecture du fichier ASE impossible ou fichier corrompu 2";
		delete pMapASE;

		erreur( "Lecture du fichier ASE impossible ou fichier corrompu 2" );

		return 0;	// Les choses se sont mal pass�es pour la conversion du fichier
	}

	pMapASE->echangeYZ();						// Inversion des coordonnées Y et Z
	pMapASE->scale( -1.0f, 1.0f, 1.0f );

		// Transfert des fichiers de texture dans le répertoire du fichier Map
	string nomRep = "./Map/" + nomFichierASE;
	pMapASE->init();
	pMapASE->Save(nomFichierASE);

	if( CFindFolder::mkdir( nomRep.c_str() )!=0 ) {		// Création du r�pertoire pour les textures
		// Si un répertoire existe déjà, demande s'il faut l'écraser
		ecraserRepOuiNon( arg );
	}
	else {
		// Sinon passe immédiatement à l'étape 2 de la conversion
		SDL_CreateThread( threadConvertASE_2, arg );
	}

	return 0;	// Fin de la première étape de conversion
}

int threadConvertASE_2( void *arg ) {
	string nomFichierASE = (char*)arg;
	string nomRep = "./Map/" + nomFichierASE;

	CMaterial *mat;
	vector<CMaterial*>::iterator iter;
	for( iter=pMapASE->_materials.begin() ; iter!=pMapASE->_materials.end() ; iter++ ) {
		mat = *iter;

		if( mat->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
			CMaterialTexture *matRef = (CMaterialTexture*)mat;

			if( !copieTexture( matRef, pMapASE, nomRep ) ) {
				erreur( "Erreur a la copie d'un fichier de texture" );
				delete pMapASE;
				return 0;
			}
		}
		else if( mat->Type()==CMaterial::MAT_TYPE_MULTI ) {
			CMaterialMulti *matMulti = (CMaterialMulti *)mat;

			for( int i=0 ; i<matMulti->NbrTex() ; i++ ) {
				if( matMulti->m_TabMat[i]->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
					if( !copieTexture( (CMaterialTexture*)matMulti->m_TabMat[i], pMapASE, nomRep ) ) {
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

void suivantASE(void *arg) {
	SDL_CreateThread( threadConvertASE_1, arg );
}

void lanceMenuConvertASE(void *var) {
LOGDEBUG(("lanceMenuConvertASE(var=%x)", var));
	string name;

	PF *liste_suivant_open_ASE;
	char **item_menu_open_ASE;
	void **liste_argument_open_ASE;

	vector<AseInformationDto> content;
	MapService::loadAseDirectoryContent(content);

	int nbrFichier = content.size();	// Nombre de fichiers ASE à prendre en compte

	liste_suivant_open_ASE = new PF[ nbrFichier ];
	liste_argument_open_ASE = new void*[ nbrFichier ];
	item_menu_open_ASE = new char*[ nbrFichier ];
	nbrFichier = 0;

	for(AseInformationDto dto : content) {
		name = dto.getAseFileMinimalName();
		liste_suivant_open_ASE[nbrFichier] = suivantASE;

		item_menu_open_ASE[nbrFichier] = new char[name.size()+1];
		liste_argument_open_ASE[nbrFichier] = new char[name.size()+1];
		strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_open_ASE[nbrFichier]), name.c_str() );
		nbrFichier++;
	}

	MenuOpenASE = new CMenu( "Convertir une sc\u00e8ne ASE", (const char**)item_menu_open_ASE, nbrFichier,
			liste_suivant_open_ASE, retourASE, liste_argument_open_ASE );

	delete[] liste_suivant_open_ASE;
	delete[] liste_argument_open_ASE;
	delete[] item_menu_open_ASE;

	CDlg::SetMenuActif( MenuOpenASE );
}

}	// JktMenu
