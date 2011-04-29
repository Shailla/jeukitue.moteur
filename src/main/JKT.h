
#ifndef __JKT__JKT_H
#define __JKT__JKT_H

bool indicSpace = true;		//gère si la théière animée part ou revient
float positionSpace = 0.0;	 //position de la théière animée

CCfg Config;		// Contient la configuration du jeu

CGame Game;		// Contient toutes les données vivantes du jeu

const char nomFichierJoueur[] = "@Joueur\\joueurTex";

int JKT_RenderMode = GL_TRIANGLES;	// Rendu normal ou en mode filaire
bool JKT_AfficheToutesTextures = false;

string nomFichierConfig = "config";

JktMenu::CFocus *pFocus;

int nbrMainPlayer = 0;	//nbre de joueurs sur la map

SDL_Event event;	//évênement SDL pour la gestion des input (clavier/souris/joystick...)

SDL_TimerID timer_ID = 0;
void *param = 0;
SDL_Event event_SDL;

bool Aide = false;

extern JktSon::CDemonSons *DemonSons;	// Requêtes des son à jouer

Uint32 tempsTimer = 0;		// Temps pris par la fonction 'timer'
Uint32 tempsDisplay = 0;	// Temps pris par la fonction 'display'


	//FONCTIONS FONCTIONS FONCTIONS
int main(int argc, char** argv);
void quit_tutorial();				// Quitte sans code d'erreur
void quit_tutorial( int code );		// Quitte avec code d'erreur
void gravitePlayer(CPlayer *player);	//fonction implémentant la gravité
void init(void);
void keyboard(unsigned char key, int x, int y);			// agit en fonction du clavier 
void display(void);										// Affiche l'écran
unsigned int timer(unsigned int nbr, void *fct);	//focntion qui s'exécute périodiquement et qui provoque l'affichage
void play_handle_key_down( SDL_Event* event );
void menu_agar_handle_key_down( SDL_Event* event );
static void process_events( void );
Uint32 getPixel(SDL_Surface *surface, int x, int y);
int main(int argc, char** argv);

#endif

