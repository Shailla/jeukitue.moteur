
#ifndef __JKT__DLGBOITE_H
#define __JKT__DLGBOITE_H

#include <vector>
#include <string>
using namespace std;

#include "spatial/widget/Icone.h"

#include "Dlg.h"

using namespace JktMoteur;

namespace JktMenu
{

typedef void (*PF)(void *);

class CBouton
{
public:
	bool m_bActif;
	string _txt;
	void *m_Arg;			// Argument � transmettre � la fonction appel�e lors de l'appui du bouton
	bool m_bFocus;			// Indique si le bouton est focuss�
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
	enum JKT_DLG {			// Les diff�rents types de bo�te de dialogue
		JKT_DLG_NULL,		// Bo�te sans type particulier
		JKT_DLG_INFO,		// Bo�te d'info
		JKT_DLG_ENCOURS,	// Bo�te qui indique une t�che ne cours
		JKT_DLG_CONFIRM,	// Bo�te de confirmation
		JKT_DLG_ERREUR,		// Bo�te d'erreur
	};
private:
	static bool m_B_INIT_CLASSE;	// Indique si la classe a d�j� �t� initialis�e

	static Icone* _iconeErreur;		// Texture de l'icone associ�e � un message d'erreur
	static Icone* _iconeInfo;			// Texture de l'icone associ�e � un message d'information
	static Icone* _iconeEnCours;		// Texture de l'icone associ�e � un message d'information
	static Icone* _iconeConfirm;		// Texture de l'icone associ�e � un message de confitmation

	char *m_Titre;			// Titre de la bo�te de dialogue
	vector<string> m_Texte;	// Texte apparaissant dans la boite de dialogue
	PF m_FonctionRetour;	// Fonction retour de ce sous menu
	int m_Focus;			// Indique quel bouton est focuss�
	JKT_DLG m_Type;			// Type de la bo�te de dialogue

	void afficheTexte();	// Affiche le texte de la bo�te de dialogue
	void afficheIcone();	// Affiche l'icone associ�e au type de message
	void afficheCadre();	// Affiche le cadre de la bo�te
	void afficheTitre();	// Affiche le titre dans le cadre de la bo�te de dialogue
	void afficheBouton();	// Affiche le(s) bouton(s) de la bo�te
	void right();
	void left();

	void MouseBoutonDown( SDL_Event *event );
	void KeyDown( SDL_Event *event );

public:
	CBouton m_Bouton1;
	CBouton m_Bouton2;
	CBouton m_Bouton3;

	static bool INIT_CLASSE();			// Initialise la classe si elle ne l'a pas encore �t�
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
