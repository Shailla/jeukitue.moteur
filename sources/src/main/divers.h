
#ifndef __JKT__DIVERS_H
#define __JKT__DIVERS_H

#include <string>

using namespace std;

void quit_game();
void quit_game(int code);
void quit_game(const char* txt,int code);
void quit_game(const string& txt,int code);
void quit_JKT();
void load_Intro( int width, int height );
void load_IntroSub( int width, int height );

#endif

