#include "../Include/CLog.h"

std::string CLog::m_sLogFile;
bool CLog::m_bWriteToConsole = false;

void CLog::Init(const std::string& _rLogFile, bool _bWriteToConsole)
{
    m_sLogFile = _rLogFile;
    m_bWriteToConsole = _bWriteToConsole;
}

void CLog::WriteLine(const char* _sStr)
{
    std::string sStr = "[" + CDateTime::GetNow().ToString() + "] " + _sStr;

    if (m_bWriteToConsole)
    {
        std::cout << sStr << std::endl;
    }
    
    std::ofstream cFile(m_sLogFile, std::ios_base::app);
    if (!cFile.is_open())
    {
        std::cerr << "Unable to open log file!" << std::endl;
    }
    else
    {
        cFile << sStr << std::endl;
        cFile.close();

        if (cFile.fail())
        {
            std::cerr << "Error closing log file!" << std::endl;
        }
    }
}

void CLog::WriteLine(const std::string& _rStr)
{
    WriteLine(_rStr.c_str());
}