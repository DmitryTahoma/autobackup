#include "autobackup.h"

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
		std::cout << "Rotation Day Of Week: " << cConfig.GetRotationDayOfWeek() << std::endl;
		std::cout << "Daily: " << cConfig.GetCountDaily() << std::endl;
		std::cout << "Weekly: " << cConfig.GetCountWeekly() << std::endl;
		std::cout << "Monthly: " << cConfig.GetCountMonthly() << std::endl;
		std::cout << std::endl;

		for (auto& rDbConfig : cConfig.GetDatabases())
		{
			std::cout << "Username: " << rDbConfig->m_sUsername << std::endl;
			std::cout << "Password: " << rDbConfig->m_sPassword << std::endl;
			std::cout << "Host: " << rDbConfig->m_sHost << std::endl;
			std::cout << "Port: " << rDbConfig->m_iPort << std::endl;
			std::cout << "Backup Directory: " << rDbConfig->m_sBackupDirectory << std::endl;
			std::cout << "Database Name: " << rDbConfig->m_sDatabaseName << std::endl;
			std::cout << "Database Provider: ";
			if (rDbConfig->m_eDatabaseProvider == EDatabaseProvider::Mysql)
				std::cout << "MySQL";
			else if (rDbConfig->m_eDatabaseProvider == EDatabaseProvider::MSSQL)
				std::cout << "MSSQL";
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}

	return 0;
}
