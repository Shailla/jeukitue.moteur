#ifndef __JKT__OPENCENTRALISATEUR_H
#define __JKT__OPENCENTRALISATEUR_H

#include <agar/core.h>
#include <agar/gui.h>

class OpenCentralisateurView
{
    AG_Window *m_window;
    AG_Button* m_connection;
    AG_Textbox* m_userName;
    char m_userNameStr[40];
    AG_Textbox* m_adresse;
    char m_adresseStr[40];
    AG_Textbox* m_port;
public:
    OpenCentralisateurView(const AG_EventFn controllerCallback);
    ~OpenCentralisateurView(void);
    
    void show(void);
    void hide(void);

    char* getUserName(void);
    char* getAdresse(void);
    int getPort(void);
};

#endif
