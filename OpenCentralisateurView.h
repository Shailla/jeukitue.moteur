#ifndef __JKT__OPENCENTRALISATEUR_H
#define __JKT__OPENCENTRALISATEUR_H

#include <agar/core.h>
#include <agar/gui.h>

#include "View.h"

class OpenCentralisateurView : public View
{
    AG_Textbox* m_userName;
    char m_userNameStr[40];
    AG_Textbox* m_adresse;
    char m_adresseStr[40];
    AG_Textbox* m_port;
    AG_Button* m_connection;
	AG_Button* m_retour;
public:
    OpenCentralisateurView(const AG_EventFn controllerCallback);
    ~OpenCentralisateurView(void);

    char* getUserName(void);
    char* getAdresse(void);
    int getPort(void);
};

#endif
