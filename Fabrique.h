#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

#include "Controller.h"
#include "Viewer.h"
#include "Centralisateur.h"

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
