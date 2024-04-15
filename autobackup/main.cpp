#include "Include/CConfiguration.h"
#include "Include/CMySQLDumper.h"
#include "Include/CLZMAUtil.h"
#include "Include/CDirectory.h"
#include "Include/CFile.h"
#include "Include/CLog.h"

std::vector<std::string> DumpDatabases(const std::shared_ptr<CConfiguration> &_rConfiguration);
void CompressDumps(const std::vector<std::string> &_rDumpFiles);
void DeleteBrokenArchives(const std::shared_ptr<CConfiguration>& _rConfiguration);
void DoRotation(const std::shared_ptr<CConfiguration>& _rConfiguration);

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
		CLog::Init(pConfiguration->GetLogFile(), pConfiguration->IsDebug());
		std::vector<std::string> cDumpFiles = DumpDatabases(pConfiguration);
		CompressDumps(cDumpFiles);
		DeleteBrokenArchives(pConfiguration);
		DoRotation(pConfiguration);
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
			CLog::WriteLine("Start dumping " + rDatabaseConfig->m_sDatabaseName);
			dumper->Dump();
			CLog::WriteLine("End dumping " + rDatabaseConfig->m_sDatabaseName);
			cDumpFiles.push_back(dumper->GetDumpFileName());
		}
	}
	return cDumpFiles;
}

void CompressDumps(const std::vector<std::string> &_rDumpFiles)
{
	for (auto& rFileName : _rDumpFiles)
	{
    	CLog::WriteLine("Start compressing " + rFileName);
		if (CLZMAUtil::Compress(rFileName, rFileName + ".lzma"))
		{
    		CLog::WriteLine("Compressed success!");
    		CLog::WriteLine("Removing " + rFileName);
			std::remove(rFileName.c_str());
    		CLog::WriteLine("File removed");
		}
		else
		{
    		CLog::WriteLine("Compression failed");
		}
	}
}

void DeleteBrokenArchives(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	CLog::WriteLine("Removing broken archives");
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
						CLog::WriteLine("Removing broken archive " + sStr2);
						std::remove((sPath + sStr2).c_str());
						CLog::WriteLine("Removed");
					}
				}
			}
		}
	}
}

void DailyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration);
void WeeklyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration);
void MonthlyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration);

void DoRotation(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	DailyRotation(_rConfiguration);
	if (_rConfiguration->GetRotationDayOfWeek() == CDateTime::GetNow().GetDayOfWeek() + 1)
	{
		WeeklyRotation(_rConfiguration);
		MonthlyRotation(_rConfiguration);
	}
}

void DailyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	CLog::WriteLine("Daily rotation...");
	for (auto& rDatabaseConfig : _rConfiguration->GetDatabases())
	{
		std::string sPath = rDatabaseConfig->m_sBackupDirectory + "/daily/";

		std::vector<std::string> cFiles = CDirectory::GetFiles(sPath.c_str());
		CDateTime cNow = CDateTime::GetNow();

		for (auto& rFile : cFiles)
		{
			std::string sFilePath = sPath + rFile;
			if ((cNow - CFile::GetModificationTime(sFilePath.c_str())).GetTotalDays() > (double)_rConfiguration->GetCountDaily())
			{
				CLog::WriteLine("Removing old daily: " + rFile);
				std::remove(sFilePath.c_str());
				CLog::WriteLine("Removed");
			}
		}
	}
}

std::string GetNewestFile(const std::string& _rFolderPath)
{
	std::vector<std::string> cFiles = CDirectory::GetFiles(_rFolderPath.c_str());
	int iNewestIndex = -1;
	for (int i = 0; i < (int)cFiles.size(); ++i)
	{
		if (iNewestIndex == -1)
		{
			CDateTime cTempDate = CFile::GetModificationTime((_rFolderPath + cFiles[i]).c_str());
			if (cTempDate.GetMonth() != 0)
			{
				iNewestIndex = i;
			}
		}
		else if ((CFile::GetModificationTime((_rFolderPath + cFiles[iNewestIndex]).c_str()) - CFile::GetModificationTime((_rFolderPath + cFiles[i]).c_str())).GetTotalSeconds() < 0)
		{
			iNewestIndex = i;
		}
	}

	return iNewestIndex == -1 ? std::string() : cFiles[iNewestIndex];
}

void WeeklyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	CLog::WriteLine("Weekly rotation...");
	for (auto& rDatabaseConfig : _rConfiguration->GetDatabases())
	{
		std::string sDailyDir = rDatabaseConfig->m_sBackupDirectory + "/daily/";
		std::string sNewestDaily = GetNewestFile(sDailyDir);
		if (!sNewestDaily.empty())
		{
			std::string sWeeklyDir = rDatabaseConfig->m_sBackupDirectory + "/weekly/";
			CDirectory::CreateDirectory(sWeeklyDir.c_str());
			std::string sSourceFile = sDailyDir + sNewestDaily;
			std::string sDestFile = sWeeklyDir + sNewestDaily;
			CFile::Copy(sSourceFile.c_str(), sDestFile.c_str());

			std::vector<std::string> cFiles = CDirectory::GetFiles(sWeeklyDir.c_str());
			CDateTime cNow = CDateTime::GetNow();

			for (auto& rFile : cFiles)
			{
				std::string sFilePath = sWeeklyDir + rFile;
				if ((cNow - CFile::GetModificationTime(sFilePath.c_str())).GetTotalDays() > (double)_rConfiguration->GetCountWeekly() * 7.0)
				{
					CLog::WriteLine("Remove old weekly: " + rFile);
					std::remove(sFilePath.c_str());
					CLog::WriteLine("Removed");
				}
			}
		}
	}
}

void MonthlyRotation(const std::shared_ptr<CConfiguration>& _rConfiguration)
{
	for (auto& rDatabaseConfig : _rConfiguration->GetDatabases())
	{
		std::string sMonthlyDir = rDatabaseConfig->m_sBackupDirectory + "/monthly/";
		CDirectory::CreateDirectory(sMonthlyDir.c_str());
		std::string sNewestMonthly = GetNewestFile(sMonthlyDir);
		CDateTime cNow = CDateTime::GetNow();
		if (sNewestMonthly.empty() || CFile::GetModificationTime((sMonthlyDir + sNewestMonthly).c_str()).GetMonth() != cNow.GetMonth())
		{
			CLog::WriteLine("Monthly rotation...");
			std::string sDailyDir = rDatabaseConfig->m_sBackupDirectory + "/daily/";
			std::string sNewestDaily = GetNewestFile(sDailyDir.c_str());
			if (!sNewestDaily.empty())
			{
				std::string sSourceFile = sDailyDir + sNewestDaily;
				std::string sDestFile = sMonthlyDir + sNewestDaily;
				CFile::Copy(sSourceFile.c_str(), sDestFile.c_str());
			}

			std::vector<std::string> cFiles = CDirectory::GetFiles(sMonthlyDir.c_str());
			for (auto& rFile : cFiles)
			{
				std::string sFilePath = sMonthlyDir + rFile;
				if ((cNow - CFile::GetModificationTime(sFilePath.c_str())).GetTotalDays() > (double)_rConfiguration->GetCountMonthly() * 28.0)
				{
					CLog::WriteLine("Remove old monthly: " + rFile);
					std::remove(sFilePath.c_str());
					CLog::WriteLine("Removed");
				}
			}
		}
	}
}