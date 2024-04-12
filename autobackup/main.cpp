#include "Include/CConfiguration.h"
#include "Include/CMySQLDumper.h"
#include "Include/CLZMAUtil.h"
#include "Include/CDirectory.h"
#include "Include/CFile.h"

std::vector<std::string> DumpDatabases(const std::shared_ptr<CConfiguration> &_rConfiguration);
void CompressDumps(const std::vector<std::string> &_rDumpFiles);
void DeleteBrokenArchives(const std::shared_ptr<CConfiguration>& _rConfiguration);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
		return 1;
	}

	std::shared_ptr<CConfiguration> pConfiguration = std::make_shared<CConfiguration>();
	if (!pConfiguration->Load(argv[1]))
	{
		std::cerr << "Unable to open file " << argv[1] << std::endl;
		return 1;
	}
	else
	{
		std::vector<std::string> cDumpFiles = DumpDatabases(pConfiguration);
		CompressDumps(cDumpFiles);
		DeleteBrokenArchives(pConfiguration);
	}

	return 0;
}

std::vector<std::string> DumpDatabases(const std::shared_ptr<CConfiguration> &_rConfiguration)
{
	std::vector<std::string> cDumpFiles;
	for (auto &rDatabaseConfig : _rConfiguration->GetDatabases())
	{
		if (rDatabaseConfig->m_eDatabaseProvider == EDatabaseProvider::Mysql)
		{
			std::shared_ptr<CMySQLDumper> dumper = std::make_shared<CMySQLDumper>(rDatabaseConfig);
			dumper->Dump();
			cDumpFiles.push_back(dumper->GetDumpFileName());
		}
	}
	return cDumpFiles;
}

void CompressDumps(const std::vector<std::string> &_rDumpFiles)
{
	for (auto& rFileName : _rDumpFiles)
	{
		std::cout << "Start compressing " << rFileName << std::endl;
		if (CLZMAUtil::Compress(rFileName, rFileName + ".lzma"))
		{
			std::cout << "Compressed" << std::endl;
			std::cout << "Removing " << rFileName << std::endl;
			std::remove(rFileName.c_str());
			std::cout << "File removed" << std::endl;
		}
		else
		{
			std::cerr << "Compression failed" << std::endl;
		}
	}
}

void DeleteBrokenArchives(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	for (auto& rDatabaseConfig : _rConfiguration->GetDatabases())
	{
		std::string sPath = rDatabaseConfig->m_sBackupDirectory + "/daily/";

		std::vector<std::string> cFiles = CDirectory::GetFiles(sPath.c_str());
		CDateTime cNow = CDateTime::GetNow();

		for (int i = 0; i < (int)cFiles.size(); ++i)
		{
			for (int j = 0; j < (int)cFiles.size(); ++j)
			{
				if (i != j)
				{
					std::string& rStr1 = cFiles[i];
					std::string sStr2 = cFiles[j] + ".lzma";

					if (rStr1.length() == sStr2.length() && std::equal(rStr1.begin(), rStr1.end(), sStr2.begin()))
					{
						std::cout << "Removing broken archive " << sStr2 << std::endl;
						std::remove((sPath + sStr2).c_str());
						std::cout << "Removed" << std::endl;
					}
				}
			}
		}
	}
}