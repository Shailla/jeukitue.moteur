#pragma once
#include "jktexception.h"

class CentralisateurException : public JktException
{
public:
    CentralisateurException(void);
    ~CentralisateurException(void);
};
