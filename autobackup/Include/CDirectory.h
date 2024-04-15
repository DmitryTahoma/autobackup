#pragma once

#include "CLog.h"

#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>

class CDirectory
{
public:
    static std::vector<std::string> GetFiles(const char* _sPath);
    static void CreateDirectory(const char* _sPath);

};