#pragma once

#include "CDateTime.h"

#include <string>
#include <iostream>
#include <fstream>

class CLog
{
public:
    static void Init(const std::string& _rLogFile, bool _bWriteToConsole);
    static void WriteLine(const char* _sStr);
    static void WriteLine(const std::string& _rStr);

private:
    static std::string m_sLogFile;
    static bool m_bWriteToConsole;

};