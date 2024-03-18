#include "autobackup.h"

std::string IntToString(int iValue)
{
	std::string sResult;
	if (iValue < 10)
	{
		sResult += "0";
	}
	return sResult + std::to_string(iValue);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
		return 1;
	}

	CConfiguration cConfig;
	if (!cConfig.Load(argv[1]))
	{
		std::cerr << "Unable to open file " << argv[1] << std::endl;
	}
	else
	{
		for (auto& rDatabaseConfig : cConfig.GetDatabases())
		{
			if (rDatabaseConfig->m_eDatabaseProvider == EDatabaseProvider::Mysql)
			{
				std::cout << "Backuping " << rDatabaseConfig->m_sDatabaseName << "..." << std::endl;

				std::time_t iCurrentTime = std::time(nullptr);
				std::tm* pLocalTime = std::localtime(&iCurrentTime);

				int iYear = pLocalTime->tm_year + 1900;
				int iMonth = pLocalTime->tm_mon + 1;
				int iDay = pLocalTime->tm_mday;
				int iHours = pLocalTime->tm_hour;
				int iMinutes = pLocalTime->tm_min;
				int iSeconds = pLocalTime->tm_sec;

				std::string m_sDirectory = rDatabaseConfig->m_sBackupDirectory + "/daily/";
				std::string m_sCommand = "mkdir -p " + m_sDirectory;
				
				system(m_sCommand.c_str());

				m_sCommand = "docker exec " + rDatabaseConfig->m_sContainerName + " mysqldump -u " + rDatabaseConfig->m_sUsername + " -p" + rDatabaseConfig->m_sPassword + " --all-databases > "
					+ m_sDirectory + rDatabaseConfig->m_sDatabaseName + "_" + IntToString(iYear) + "-" + IntToString(iMonth) + "-" + IntToString(iDay) + "_" + IntToString(iHours) + "-" + IntToString(iMinutes) + "-" + IntToString(iSeconds) + ".sql";
				
				system(m_sCommand.c_str());
			}	
		}
	}

	return 0;
}
