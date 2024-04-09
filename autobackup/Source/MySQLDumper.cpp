#include "../Include/MySQLDumper.h"
#include <iostream>
#include <sstream>

MySQLDumper::MySQLDumper(const std::shared_ptr<SDatabaseConfig>& _rConfig)
    : DumperBase(_rConfig)
{
}

void MySQLDumper::Dump()
{
    std::cout << "Start dumping " << pConfig->m_sDatabaseName << std::endl;

    std::string sDailyDir = pConfig->m_sBackupDirectory + "/daily/";
    MakeDirectory(sDailyDir);

    std::stringstream ss;
    ss << sDailyDir << pConfig->m_sDatabaseName << "_" << GetCurrentDateString() << ".sql";
    sDumpFileName = ss.str();

    ss.str("");
    ss << "docker exec "
        << pConfig->m_sContainerName
        << " mysqldump"
        << " -h " << pConfig->m_sHost
        << " -P " << std::to_string(pConfig->m_iPort)
        << " -u " << pConfig->m_sUsername
        << " -p" << pConfig->m_sPassword
        << " -d " << pConfig->m_sDatabaseName
        << " --no-data=false"
        << " --routines"
        << " > " << sDumpFileName;
    
    system(ss.str().c_str());

    std::cout << "Success dumping " << pConfig->m_sDatabaseName << std::endl;
}

void MySQLDumper::MakeDirectory(const std::string& _sPath)
{
    std::string sCommand = "mkdir -p " + _sPath;
    system(sCommand.c_str());
}