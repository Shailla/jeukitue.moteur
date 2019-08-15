
#ifndef __JKT__JKT_H
#define __JKT__JKT_H

#include <map>

class CPlayer;
namespace jkt {
	class GraphicObject;
}

int main(int argc, char** argv);
void quit_game( int code );												// Quitte avec code d'erreur
void gravitePlayer(Uint32 now, CPlayer *player, float deltaTime);		// Fonction implémentant la gravité
void initMenu(void);
void keyboard(unsigned char key, int x, int y);							// Agit en fonction du clavier
void display(void);														// Affiche l'écran
unsigned int timer(unsigned int nbr, void *fct);						// Focntion qui s'exécute p�riodiquement et qui provoque l'affichage
void play_handle_key_down( SDL_Event* event );
Uint32 getPixel(SDL_Surface *surface, int x, int y);

void addGraphicObjectToInitialize(jkt::GraphicObject* graphicObject);
void addGraphicObjectToDestruct(jkt::GraphicObject* graphicObject);

#endif /* __JKT__JKT_H */

