
#ifndef __JKT__DLGBOITE_H
#define __JKT__DLGBOITE_H

#include <vector>
#include <string>
using namespace std;

#include "Dlg.h"

namespace JktMenu
{

typedef void (*PF)(void *);

class CBouton
{
public:
	bool m_bActif;
	char *txt;
	void *m_Arg;			// Argument à transmettre à la fonction appelée lors de l'appui du bouton
	bool m_bFocus;			// Indique si le bouton est focussé
	PF m_FctGo;

	CBouton();
	CBouton( const char *txt, PF fct_go, void *arg=0 );
	void operator =( CBouton &but );

	void put( const char *t );
	void focus( bool indic );
	void ExecFctGo();
	void affiche();
	void afficheCadre();
	void afficheTexte();
};

class CDlgBoite:public CDlg
{
public:
	enum JKT_DLG {			// Les différents types de boîte de dialogue
		JKT_DLG_NULL,		// Boîte sans type particulier
		JKT_DLG_INFO,		// Boîte d'info
		JKT_DLG_ENCOURS,	// Boîte qui indique une tâche ne cours
		JKT_DLG_CONFIRM,	// Boîte de confirmation
		JKT_DLG_ERREUR,		// Boîte d'erreur
	};
private:
	static bool m_B_INIT_CLASSE;	// Indique si la classe a déjà été initialisée

	static int texErreur;		// Texture de l'icone associée à un message d'erreur
	static int texInfo;			// Texture de l'icone associée à un message d'information
	static int texEnCours;		// Texture de l'icone associée à un message d'information
	static int texConfirm;		// Texture de l'icone associée à un message de confitmation

	char *m_Titre;			// Titre de la boîte de dialogue
	vector<string> m_Texte;	// Texte apparaissant dans la boite de dialogue
	PF m_FonctionRetour;	// Fonction retour de ce sous menu
	int m_Focus;			// Indique quel bouton est focussé
	JKT_DLG m_Type;			// Type de la boîte de dialogue

	void afficheTexte();	// Affiche le texte de la boîte de dialogue
	void afficheIcone();	// Affiche l'icone associée au type de message
	void afficheCadre();	// Affiche le cadre de la boîte
	void afficheTitre();	// Affiche le titre dans le cadre de la boîte de dialogue
	void afficheBouton();	// Affiche le(s) bouton(s) de la boîte
	void right();
	void left();

	void MouseBoutonDown( SDL_Event *event );
	void KeyDown( SDL_Event *event );

public:
	CBouton m_Bouton1;
	CBouton m_Bouton2;
	CBouton m_Bouton3;

	static bool INIT_CLASSE();			// Initialise la classe si elle ne l'a pas encore été
		// Constructeurs / destructeurs
	CDlgBoite( const char *titre, const char *txt, PF fct_retour=0, JKT_DLG type=JKT_DLG_NULL );
	CDlgBoite( const char *titre, const char *txt, PF fct_retour, CBouton &but1, CBouton &but2, CBouton &but3, JKT_DLG type=JKT_DLG_NULL );
	virtual ~CDlgBoite(void);

	void addBouton( int but, const char *txt, PF fct_go, void *arg=0 );

		// Fonctions virtuelles de la classe de base CDlg
	void go();
	void handle_key_down( SDL_Event *event );
};

}	// JktMenu

#endif
