#include "../Include/CDirectory.h"

std::vector<std::string> CDirectory::GetFiles(const char *_sPath)
{
    std::vector<std::string> cFiles;

    DIR *pDirectory = opendir(_sPath);
    if (!pDirectory)
    {
        std::cerr << "Unable to open directory " << _sPath << std::endl;
        return cFiles;
    }

    dirent *pEntry;
    while ((pEntry = readdir(pDirectory)) != nullptr)
    {
        if (pEntry->d_type == DT_REG)
        {
            cFiles.push_back(pEntry->d_name);
        }
    }

    closedir(pDirectory);
    return cFiles;
}

void CDirectory::CreateDirectory(const char * _sPath)
{
    std::string sCommand = "mkdir -p ";
    sCommand += _sPath;
    system(sCommand.c_str());
}