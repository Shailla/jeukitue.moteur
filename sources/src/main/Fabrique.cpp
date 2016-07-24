
#include <iostream>

using namespace std;

#include "util/Trace.h"

#include "main/Fabrique.h"

using namespace jkt;

Centralisateur* Fabrique::_centralisateur = 0;
Viewer* Fabrique::_agarView = 0;
Controller* Fabrique::_agarController = 0;
PluginEngine* Fabrique::_pluginEngine = 0;
CommandeInterpreter* Fabrique::_commandeInterpreter = 0;
GLUtils* Fabrique::_glUtils = 0;

void Fabrique::construct(void) {
    UdpController* udpController = new UdpController();
    _centralisateur = new Centralisateur(udpController);

    // Agar initialization
    _pluginEngine = new PluginEngine();
    _agarView = new Viewer(&Controller::executeAction);

    _agarController = new Controller(_agarView, _pluginEngine);
    _commandeInterpreter = new CommandeInterpreter(_agarView);

    _glUtils = new GLUtils();
}

Centralisateur* Fabrique::getCentralisateur() {
    return _centralisateur;
}

Viewer* Fabrique::getAgarView() {

	if(!_agarView) {
		LOGERROR(("ACCES PREMATURE A UN ELEMENT DE LA FABRIQUE"));
		exit(1);
	}

    return _agarView;
}

Controller* Fabrique::getAgarController() {
	if(!_agarController) {
		LOGERROR(("ACCES PREMATURE A UN ELEMENT DE LA FABRIQUE"));
		exit(1);
	}

	return _agarController;
}

PluginEngine* Fabrique::getPluginEngine() {
	if(!_pluginEngine) {
		LOGERROR(("ACCES PREMATURE A UN ELEMENT DE LA FABRIQUE"));
		exit(1);
	}

    return _pluginEngine;
}

CommandeInterpreter* Fabrique::getCommandeInterpreter() {
	if(!_commandeInterpreter) {
		LOGERROR(("ACCES PREMATURE A UN ELEMENT DE LA FABRIQUE"));
		exit(1);
	}

    return _commandeInterpreter;
}

GLUtils* Fabrique::getGlUtils() {
	if(!_glUtils) {
		LOGERROR(("ACCES PREMATURE A UN ELEMENT DE LA FABRIQUE"));
		exit(1);
	}

    return _glUtils;
}
