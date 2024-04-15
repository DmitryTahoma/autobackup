#include "../Include/CMySQLDumper.h"

CMySQLDumper::CMySQLDumper(const std::shared_ptr<SDatabaseConfig>& _rConfig)
    : CDumperBase(_rConfig)
{
}

void CMySQLDumper::Dump()
{
    std::string sDailyDir = m_pConfig->m_sBackupDirectory + "/daily/";
    MakeDirectory(sDailyDir);

    std::stringstream ss;
    ss << sDailyDir << m_pConfig->m_sDatabaseName << "_" << GetCurrentDateString() << ".sql";
    m_sDumpFileName = ss.str();

    ss.str("");
    ss << "docker exec "
        << m_pConfig->m_sContainerName
        << " mysqldump"
        << " -h " << m_pConfig->m_sHost
        << " -P " << std::to_string(m_pConfig->m_iPort)
        << " -u " << m_pConfig->m_sUsername
        << " -p" << m_pConfig->m_sPassword
        << " -d " << m_pConfig->m_sDatabaseName
        << " --no-data=false"
        << " --routines"
        << " > " << m_sDumpFileName;
    
    system(ss.str().c_str());
}

void CMySQLDumper::MakeDirectory(const std::string& _sPath)
{
    std::string sCommand = "mkdir -p " + _sPath;
    system(sCommand.c_str());
}