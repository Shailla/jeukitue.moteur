#ifndef __JKT__OPENCENTRALISATEUR_H
#define __JKT__OPENCENTRALISATEUR_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConfigCentralisateurView : public View
{
    char _adresse[40];
    int _port;
public:
    ConfigCentralisateurView(const AG_EventFn controllerCallback);
    ~ConfigCentralisateurView(void);

    void show(void);
    char* getUserName(void);
    char* getAdresse(void);
    int getPort(void);
};

#endif
