#include "Include/CConfiguration.h"
#include "Include/MySQLDumper.h"
#include "Include/MyLZMA.h"

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
		std::vector<std::shared_ptr<DumperBase>> cDumps;
		for (auto& rDatabaseConfig : cConfig.GetDatabases())
		{
			if (rDatabaseConfig->m_eDatabaseProvider == EDatabaseProvider::Mysql)
			{
				std::shared_ptr<MySQLDumper> dumper = std::make_shared<MySQLDumper>(rDatabaseConfig);
				dumper->Dump();
				cDumps.push_back(dumper);
			}
		}

		for (auto& rDump : cDumps)
		{
			std::string sFileName = rDump->GetDumpFileName();
			std::cout << "Start compressing " << sFileName << std::endl;
			LZMA::Compress(sFileName, sFileName + ".lzma");
			std::cout << "Compressed" << std::endl;
			std::cout << "Removing " << sFileName << std::endl;
			std::remove(sFileName.c_str());
			std::cout << "File removed" << std::endl;
		}
	}

	return 0;
}
