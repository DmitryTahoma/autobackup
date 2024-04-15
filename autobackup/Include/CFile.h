#pragma once

#include <iostream>
#include <sys/stat.h>
#include <fstream>

#include "CDateTime.h"
#include "CLog.h"

class CFile
{
public:
    static CDateTime GetModificationTime(const char* _sFilePath);
    static void Copy(const char* _sSourcePath, const char* _sDestPath);

private:

};