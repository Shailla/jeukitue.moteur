
#ifndef __JKT__JKT_H
#define __JKT__JKT_H

class CPlayer;

int main(int argc, char** argv);
void quit_tutorial();								// Quitte sans code d'erreur
void quit_game( int code );							// Quitte avec code d'erreur
void gravitePlayer(CPlayer *player);				//fonction implémentant la gravité
void init(void);
void keyboard(unsigned char key, int x, int y);		// agit en fonction du clavier
void display(void);									// Affiche l'écran
unsigned int timer(unsigned int nbr, void *fct);	//focntion qui s'exécute périodiquement et qui provoque l'affichage
void play_handle_key_down( SDL_Event* event );
void menu_agar_handle_key_down( SDL_Event* event );
static void process_events( void );
Uint32 getPixel(SDL_Surface *surface, int x, int y);

#endif

