#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <lzma.h>

class CLZMAUtil
{
public:
    static bool Compress(const std::string& _rInputFileName, const std::string& _rOutputFileName);

};