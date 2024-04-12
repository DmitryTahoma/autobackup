#include "../Include/CFile.h"

CDateTime CFile::GetModificationTime(const char * _sFilePath)
{
    struct stat fileStat;

    if (stat(_sFilePath, &fileStat) == -1)
    {
        std::cerr << "Unable to open file " << _sFilePath << std::endl;
        return CDateTime();
    }
    
    std::time_t iModificationTime = fileStat.st_mtime;
    return CDateTime(std::localtime(&iModificationTime));
}