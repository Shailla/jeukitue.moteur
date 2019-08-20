#ifndef __JKT__FABRIQUE_H
#define __JKT__FABRIQUE_H

namespace jkt {
class PluginEngine;
}

class Centralisateur;
class Viewer;
class Controller;
class CommandeInterpreter;
class GLUtils;

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
