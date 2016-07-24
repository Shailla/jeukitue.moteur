#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "centralisateur/Centralisateur.h"
#include "plugin/PluginEngine.h"
#include "script/CommandeInterpreter.h"
#include "util/GLUtils.h"

class Fabrique
{
    static Centralisateur* _centralisateur;
    static Viewer* _agarView;
    static Controller* _agarController;
    static jkt::PluginEngine* _pluginEngine;
    static CommandeInterpreter* _commandeInterpreter;
    static GLUtils* _glUtils;
public:
    static void construct(void);

    static Viewer* getAgarView();
    static Controller* getAgarController();
    static Centralisateur* getCentralisateur();
    static jkt::PluginEngine* getPluginEngine();
    static CommandeInterpreter* getCommandeInterpreter();
    static GLUtils* getGlUtils();
};

#endif
