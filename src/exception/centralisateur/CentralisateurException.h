#pragma once
#include "exception/JktException.h"

class CentralisateurException : public JktException
{
public:
    CentralisateurException(void);
    ~CentralisateurException(void);
};
