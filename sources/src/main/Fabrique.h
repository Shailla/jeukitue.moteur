#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "centralisateur/Centralisateur.h"
#include "plugin/PluginEngine.h"
#include "script/CommandeInterpreter.h"
#include "util/fonte/FonteEngine.h"

class Fabrique
{
    static Centralisateur* _centralisateur;
    static Viewer* _agarView;
    static Controller* _agarController;
    static JktPlugin::PluginEngine* _pluginEngine;
    static CommandeInterpreter* _commandeInterpreter;
    static FonteEngine* _fonteEngine;
public:
    static void construct(void);

    static Viewer* getAgarView();
    static Controller* getAgarController();
    static Centralisateur* getCentralisateur();
    static JktPlugin::PluginEngine* getPluginEngine();
    static CommandeInterpreter* getCommandeInterpreter();
    static FonteEngine* getFonteEngine();
};

#endif
