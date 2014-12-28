
#ifndef __JKT__DIVERS_H
#define __JKT__DIVERS_H

#include <string>

void quit_game(const char* txt, int code);
void quit_game(const std::string& txt, int code);
void quit_JKT();
void load_Intro( int width, int height );
void load_IntroSub( int width, int height );

#endif

