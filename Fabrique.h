#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

#include "AgarController.h"
#include "AgarView.h"
#include "Centralisateur.h"

class Fabrique
{
    static Centralisateur* m_centralisateur;
    static AgarView* m_agarView;
    static AgarController* m_agarController;
public:
    static void construct(void);

    static AgarView* getAgarView();
    static AgarController* getAgarController();
    static Centralisateur* getCentralisateur();
};

#endif
