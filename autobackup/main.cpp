#include "Include/CConfiguration.h"
#include "Include/MySQLDumper.h"

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
				MySQLDumper dumper(rDatabaseConfig);
				dumper.Dump();
			}
		}
	}

	return 0;
}
