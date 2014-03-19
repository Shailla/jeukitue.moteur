#include "main/Fabrique.h"

using namespace JktPlugin;

Centralisateur* Fabrique::_centralisateur = 0;
Viewer* Fabrique::_agarView = 0;
Controller* Fabrique::_agarController = 0;
PluginEngine* Fabrique::_pluginEngine = 0;
CommandeInterpreter* Fabrique::_commandeInterpreter = 0;

void Fabrique::construct(void) {
    UdpController* udpController = new UdpController();
    _centralisateur = new Centralisateur(udpController);

    // Agar initialization
    _pluginEngine = new PluginEngine();
    _agarView = new Viewer(Controller::executeAction);

    _agarController = new Controller(_agarView, _pluginEngine);
    _commandeInterpreter = new CommandeInterpreter(_agarView);
}

Centralisateur* Fabrique::getCentralisateur() {
    return _centralisateur;
}

Viewer* Fabrique::getAgarView() {
    return _agarView;
}

Controller* Fabrique::getAgarController() {
    return _agarController;
}

PluginEngine* Fabrique::getPluginEngine() {
    return _pluginEngine;
}

CommandeInterpreter* Fabrique::getCommandeInterpreter() {
    return _commandeInterpreter;
}
