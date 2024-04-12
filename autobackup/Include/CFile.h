#pragma once

#include <iostream>
#include <sys/stat.h>

#include "CDateTime.h"

class CFile
{
public:
    static CDateTime GetModificationTime(const char* _sFilePath);

private:

};