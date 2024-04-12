#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>

class CDirectory
{
public:
    static std::vector<std::string> GetFiles(const char* _sPath);

};