
#include <string>
#include <iostream>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include "SDL.h"
#include "main/glfont2.h"

using namespace std;

#include "util/Trace.h"
#include "util/Erreur.h"
#include "spatial/materiau/Textures.h"
#include "main/Cfg.h"
#include "main/Focus.h"

#include "ihm/DlgBoite.h"

using namespace glfont;
using namespace JktMenu;
using namespace JktUtils;

extern CCfg Config;
extern bool Aide;
extern GLFont myfont;
extern CFocus *pFocus;

namespace JktMenu
{

#define TAILLEFONT 0.5f
#define CORX		200.0f
#define CORY		200.0f
#define TAILLEBX	350.0f
#define TAILLEBY	150.0f

#define TAILLE_BOUTON_X		90.0f
#define TAILLE_BOUTON_Y		20.0f

bool CDlgBoite::m_B_INIT_CLASSE = false;
Icone* CDlgBoite::_iconeErreur = NULL;
Icone* CDlgBoite::_iconeConfirm = NULL;
Icone* CDlgBoite::_iconeInfo = NULL;
Icone* CDlgBoite::_iconeEnCours = NULL;

CBouton::CBouton()
{
	m_bFocus = false;
	m_bActif = false;
	txt = 0;
	m_FctGo = NULL;
	m_Arg = NULL;
}

CBouton::CBouton(const char *txt, PF fct_go, void *arg)
{
	m_Arg = arg;
	m_FctGo = fct_go;
	put( txt );
}

void CBouton::operator =(CBouton &but)
{
	if( txt )
		delete txt;

	m_Arg = but.m_Arg;
	m_FctGo = but.m_FctGo;

	txt = new char[ strlen( but.txt ) + 1 ];
	strcpy( txt, but.txt );
	m_bFocus = but.m_bFocus;
	m_bActif = but.m_bActif;
}

void CBouton::ExecFctGo()
{
	if( m_FctGo )
		m_FctGo( m_Arg );
	else
	{
		pFocus->SetPlayFocus();
		Aide = false;
	}
}

void CBouton::focus( bool indic )
{	m_bFocus = indic;	}

void CBouton::put( const char *t )
{
	m_bActif = true;

	if( txt )
		delete[] txt;

	txt = new char[ strlen( t ) + 1 ];
	strcpy( txt, t );
}

void CBouton::affiche()
{
	if(m_bActif)
	{
		afficheCadre();		// Affiche le cadre du bouton
		afficheTexte();		// Affiche le texte dans le bouton
	}
}

void CBouton::afficheCadre()
{
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );

	float color_fond[] = { 0.8f, 0.8f, 0.9f };
	float color_bord[] = { 0.6f, 0.6f, 1.0f };
	int rend;

	for( int i=0 ; i<2 ; i++ )
	{
		if( i==0 )
		{
			glColor3fv( color_fond );
			rend = GL_QUADS;
		}
		else
		{
			glColor3fv( color_bord );
			rend = GL_LINE_LOOP;
			glLineWidth( 2 );
		}

		glBegin( rend );
				// Affichage de la barre
			glVertex2f( 0.0f,				0.0f			);
			glVertex2f( 0.0f,				TAILLE_BOUTON_Y );
			glVertex2f( TAILLE_BOUTON_X,	TAILLE_BOUTON_Y );
			glVertex2f( TAILLE_BOUTON_X,	0.0f			);
		glEnd();
	}
}

void CBouton::afficheTexte()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );

	myfont.Begin();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if( m_bFocus )
		glColor3f( 1.0f, 0.0f, 0.0f );
	else
		glColor3f( 0.0f, 0.0f, 0.0f );

	myfont.DrawStringCentre(txt, TAILLEFONT, TAILLE_BOUTON_X/2, TAILLE_BOUTON_Y/2);
}

bool CDlgBoite::INIT_CLASSE()
{
LOGDEBUG(("CDlgBoite::INIT_CLASSE()" ));

	if( !m_B_INIT_CLASSE ) {
		// Lecture des sons et images des styles de boites de dialogue
		try {
			Uint8* pixelsErreur = JktMoteur::litFichierImage("@Icone/Erreur.bmp", -1.0f);
			Uint8* pixelsConfirm = JktMoteur::litFichierImage("@Icone/Confirm.bmp", -1.0f);
			Uint8* pixelsInfo = JktMoteur::litFichierImage("@Icone/Info.bmp", -1.0f);
			Uint8* pixelsEnCours = JktMoteur::litFichierImage("@Icone/EnCours.bmp", -1.0f);

			_iconeErreur = JktMoteur::litFichierIcone("@Icone/Erreur.bmp", -1.0f, pixelsErreur);
			_iconeConfirm = JktMoteur::litFichierIcone("@Icone/Confirm.bmp", -1.0f, pixelsConfirm);
			_iconeInfo = JktMoteur::litFichierIcone("@Icone/Info.bmp", -1.0f, pixelsInfo);
			_iconeEnCours = JktMoteur::litFichierIcone("@Icone/EnCours.bmp", -1.0f, pixelsEnCours);
		}
		catch(CErreur& erreur) {
			return false;
		}

		m_B_INIT_CLASSE = true;	// Indique que l'initialisation de classe a eu lieu
	}

	return true;
}

CDlgBoite::CDlgBoite( const char *titre, const char *txt, PF fct_retour, JKT_DLG type )
	: CDlg()
{
LOGDEBUG(("CDlgBoite::CDlgBoite(titre=%x,txt=%s,fct_retour=%x,type=%d)%T", titre, txt, fct_retour, type, this ));
	m_Type = type;

	m_Titre = new char[ strlen( titre ) + 1 ];
	strcpy( m_Titre, titre );

	myfont.GetStringOfSize( string(txt), TAILLEFONT, TAILLEBX-60.f, m_Texte,15 );

	m_FonctionRetour = fct_retour;

	m_Focus = 0;
}

CDlgBoite::CDlgBoite( const char *titre, const char *txt, PF fct_retour, CBouton &but1, CBouton &but2, CBouton &but3, JKT_DLG type )
	: CDlg()
{
	m_Type = type;

	m_Titre = new char[ strlen( titre ) + 1 ];
	strcpy( m_Titre, titre );

	myfont.GetStringOfSize( string(txt), TAILLEFONT, TAILLEBX-60.f, m_Texte, 15 );

	m_FonctionRetour = fct_retour;

		// Implémentation des boutons
	m_Bouton1 = but1;
	m_Bouton2 = but2;
	m_Bouton3 = but3;

	m_Focus = 1;
}

void CDlgBoite::addBouton( int but, const char *txt, PF fct_go, void *arg )
{
	switch( but )
	{
	case 1:
		m_Bouton1.put( txt );
		m_Bouton1.m_FctGo = fct_go;
		m_Bouton1.m_Arg = arg;
		break;
	case 2:
		m_Bouton2.put( txt );
		m_Bouton2.m_FctGo = fct_go;
		m_Bouton2.m_Arg = arg;
		break;
	case 3:
		m_Bouton3.put( txt );
		m_Bouton3.m_FctGo = fct_go;
		m_Bouton3.m_Arg = arg;
		break;
	default:
		break;
	}

	if(m_Bouton1.m_bActif)
		m_Focus = 1;
	else if(m_Bouton2.m_bActif)
		m_Focus = 2;
	else if(m_Bouton3.m_bActif)
		m_Focus = 3;
}

CDlgBoite::~CDlgBoite(void)
{
	delete m_Titre;
}

void CDlgBoite::afficheCadre()
{
	float color_fond[] = { 0.9f, 0.9f, 0.95f };
	float color_bord[] = { 0.6f, 0.6f, 1.0f };
	int rend;

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );

		// Affiche le fond de la boîte en mode GL_QUADS, puis les contours en mode GL_LINE_LOOP
	for( int i=0 ; i<2 ; i++ )
	{
		if( i==0 )
		{
			glColor3fv( color_fond );
			rend = GL_QUADS;
		}
		else
		{
			glColor3fv( color_bord );
			rend = GL_LINE_LOOP;
			glLineWidth( 4 );
		}

		glBegin( rend );
				// Affichage du cadre à proprement dit
			glVertex2f( CORX + TAILLEBX,	CORY + TAILLEBY	);
			glVertex2f( CORX + TAILLEBX,	CORY			);
			glVertex2f( CORX,				CORY			);
			glVertex2f( CORX,				CORY + TAILLEBY	);
		glEnd();
	}

		// Affiche la barre de titre de la boîte de dialogue
	glBegin( GL_QUADS );
			glVertex2f( CORX + TAILLEBX,	CORY + TAILLEBY			);
			glVertex2f( CORX + TAILLEBX,	CORY + TAILLEBY - 30	);
			glVertex2f( CORX,				CORY + TAILLEBY - 30		);
			glVertex2f( CORX,				CORY + TAILLEBY				);
	glEnd();

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
}

void CDlgBoite::afficheTitre()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );

	myfont.Begin();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f( 0.0f, 0.0f, 0.0f );
	myfont.DrawString( m_Titre, TAILLEFONT, CORX + 20, CORY + TAILLEBY - 8 );
}

void CDlgBoite::afficheTexte()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );

	myfont.Begin();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f( 0.0f, 0.0f, 0.0f );
	for( unsigned int i=0 ; i<m_Texte.size() ; i++ )
		myfont.DrawString( m_Texte[i], TAILLEFONT, CORX + 50, CORY + TAILLEBY - 50 - i*20.0f );
}

void CDlgBoite::afficheBouton()
{
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );

	for( int j=1 ; j<4 ; j++ )
	{
		switch( j )
		{
		case 1:
			glPushMatrix();
				glTranslatef( CORX + 15.0f, CORY + 10.0f, 0.0f );

				if( m_Focus==1 )
					m_Bouton1.focus( true );
				else
					m_Bouton1.focus( false );

				m_Bouton1.affiche();
			glPopMatrix();
			break;
		case 2:
			glPushMatrix();
				glTranslatef( CORX + 130.0f, CORY + 10.0f, 0.0f );

				if( m_Focus==2 )
					m_Bouton2.focus( true );
				else
					m_Bouton2.focus( false );

				m_Bouton2.affiche();
			glPopMatrix();
			break;
		case 3:
			glPushMatrix();
				glTranslatef( CORX + 245.0f, CORY + 10.0f, 0.0f );

				if( m_Focus==3 )
					m_Bouton3.focus( true );
				else
					m_Bouton3.focus( false );

				m_Bouton3.affiche();
			glPopMatrix();
			break;
		default:
			break;
		}
	}
}

void CDlgBoite::afficheIcone() {
	if( m_Type!=JKT_DLG_NULL ) {
		switch(m_Type) {
		case JKT_DLG_CONFIRM:
			_iconeConfirm->affiche(CORX + 10, CORX + 40, CORY + TAILLEBY - 80, CORY + TAILLEBY - 50);
			break;
		case JKT_DLG_INFO:
			_iconeInfo->affiche(CORX + 10, CORX + 40, CORY + TAILLEBY - 80, CORY + TAILLEBY - 50);
			break;
		case JKT_DLG_ENCOURS:
			_iconeEnCours->affiche(CORX + 10, CORX + 40, CORY + TAILLEBY - 80, CORY + TAILLEBY - 50);
			break;
		case JKT_DLG_ERREUR:
			_iconeErreur->affiche(CORX + 10, CORX + 40, CORY + TAILLEBY - 80, CORY + TAILLEBY - 50);
			break;
		default:
			break;
		}
	}
}

void CDlgBoite::go() {
	afficheCadre();		// Affiche le cadre de la boîte de dialogue
	afficheTitre();		// Affiche le titre dans le cadre de la boîte de dialogue
	afficheIcone();		// Affiche l'icone associé à la boite de dialogue
	afficheTexte();		// Affiche le texte de la boîte de dialogue
	afficheBouton();	// Affiche les bouton dans la boîte de dialogue
}

void CDlgBoite::right() {
	bool cbon = false;	// Indique que le focus a été déplacé avec succès

	for(int i=1 ; ( (i<4) && (!cbon) ) ; i++) {
		switch(m_Focus) {
		case 1:
			m_Focus = 2;
			if( m_Bouton2.m_bActif )
				cbon = true;
			break;
		case 2:
			m_Focus = 3;
			if( m_Bouton3.m_bActif )
				cbon = true;
			break;
		case 3:
			m_Focus = 1;
			if( m_Bouton1.m_bActif )
				cbon = true;
			break;
		default:
			break;
		}
	}
}

void CDlgBoite::left() {
	bool cbon = false;	// Indique que le focus a été déplacé avec succès

	for(int i=1 ; ( (i<4) && (!cbon) ) ; i++) {
		switch( m_Focus ) {
		case 1:
			m_Focus = 3;
			if( m_Bouton3.m_bActif )
				cbon = true;
			break;
		case 2:
			m_Focus = 1;
			if( m_Bouton1.m_bActif )
				cbon = true;
			break;
		case 3:
			m_Focus = 2;
			if( m_Bouton2.m_bActif )
				cbon = true;
			break;
		default:
			break;
		}
	}
}

void CDlgBoite::KeyDown(SDL_Event *event) {
	switch( event->key.keysym.sym )
	{
	case SDLK_RETURN:
		switch( m_Focus )
		{
		case 1:
			m_Bouton1.ExecFctGo();
			break;
		case 2:
			m_Bouton2.ExecFctGo();
			break;
		case 3:
			m_Bouton3.ExecFctGo();
			break;
		default:
			break;
		}

		break;
	case SDLK_ESCAPE:
		if( m_FonctionRetour )
		{
			m_FonctionRetour( 0 );
		}
		else
		{
			pFocus->SetPlayFocus();
			Aide = false;
		}
		break;

	case SDLK_UP :
		break;

	case SDLK_DOWN :
		break;

	case SDLK_RIGHT :
		right();
		break;

	case SDLK_LEFT  :
		left();
		break;

	default:
		break;
	}
}

void CDlgBoite::MouseBoutonDown( SDL_Event *event )
{
	if( event->button.button==SDL_BUTTON_RIGHT )
	{
		if( m_FonctionRetour )
		{
			m_FonctionRetour( 0 );
		}
		else
		{
			pFocus->SetPlayFocus();
			Aide = false;
		}
	}
	else if( event->button.button==SDL_BUTTON_LEFT )
	{
		int x = event->button.x;
		int y = Config.Display.Y - event->button.y;

		if( x >= CORX + 245 )
		{
			if( x <= CORX + 245 + 90 )
			{
				if( y >= CORY + 10 )
				{
					if( y <= CORY + 10 +20 )
					{
						if( m_Bouton3.m_bActif )
						{
							m_Focus = 3;
							m_Bouton3.ExecFctGo();
						}
					}
				}
			}
		}
		else if( x >= CORX + 130 )
		{
			if( x <= CORX + 130 + 90 )
			{
				if( y >= CORY + 10 )
				{
					if( y <= CORY + 10 +20 )
					{
						if( m_Bouton2.m_bActif )
						{
							m_Focus = 2;
							m_Bouton2.ExecFctGo();
						}
					}
				}
			}
		}
		else if( x >= CORX + 15 )
		{
			if( x <= CORX + 15 + 90 )
			{
				if( y >= CORY + 10 )
				{
					if( y <= CORY + 10 +20 )
					{
						if( m_Bouton1.m_bActif )
						{
							m_Focus = 1;
							m_Bouton1.ExecFctGo();
						}
					}
				}
			}
		}
	}
	else
		cout << "\nSouris : " << (int)event->button.button;
}

void CDlgBoite::handle_key_down( SDL_Event *event )
{
	switch( event->type )
	{
	case SDL_MOUSEBUTTONDOWN:
		MouseBoutonDown( event );
		break;

	case SDL_KEYDOWN:
		KeyDown( event );
		break;

	default:
		break;
	}
}

}	// JktMenu
