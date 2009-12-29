#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "centralisateur/Centralisateur.h"

class Fabrique
{
    static Centralisateur* m_centralisateur;
    static Viewer* m_agarView;
    static Controller* m_agarController;
public:
    static void construct(void);

    static Viewer* getAgarView();
    static Controller* getAgarController();
    static Centralisateur* getCentralisateur();
};

#endif
