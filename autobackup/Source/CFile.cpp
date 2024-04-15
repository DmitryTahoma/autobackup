#include "../Include/CFile.h"

CDateTime CFile::GetModificationTime(const char * _sFilePath)
{
    struct stat fileStat;

    if (stat(_sFilePath, &fileStat) == -1)
    {
        std::string sStr = "Unable to open file " + std::string(_sFilePath);
        CLog::WriteLine(sStr);
        std::cerr << sStr << std::endl;
        return CDateTime();
    }
    
    std::time_t iModificationTime = fileStat.st_mtime;
    return CDateTime(std::localtime(&iModificationTime));
}

void CFile::Copy(const char * _sSourcePath, const char * _sDestPath)
{
    std::ifstream cSource(_sSourcePath, std::ios::binary);
    std::ofstream cDest(_sDestPath, std::ios::binary);
    cDest << cSource.rdbuf();
    cSource.close();
    cDest.close();
}