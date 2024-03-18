#include "../Include/CConfiguration.h"

CConfiguration::CConfiguration()
	: m_iRotationDayOfWeek(0),
	m_iCountDaily(1),
	m_iCountWeekly(1),
	m_iCountMonthly(1)
{
}

int CConfiguration::GetRotationDayOfWeek() const
{
	return m_iRotationDayOfWeek;
}

int CConfiguration::GetCountDaily() const
{
	return m_iCountDaily;
}

int CConfiguration::GetCountWeekly() const
{
	return m_iCountWeekly;
}

int CConfiguration::GetCountMonthly() const
{
	return m_iCountMonthly;
}

bool CConfiguration::Load(const char* _sPath)
{
	std::ifstream cFile(_sPath);
	if (!cFile.is_open())
	{
		return false;
	}

	std::string sLine;
	std::unordered_map<std::string, std::shared_ptr<SDatabaseConfig>> cDbMap;
	while (std::getline(cFile, sLine))
	{
		if (sLine.empty() || sLine[0] == '#') continue;

		std::istringstream cIss(sLine);
		std::string sKey, sValue;
		std::getline(cIss, sKey, '=');
		std::getline(cIss, sValue);

		if (sKey.empty() || sValue.empty()) continue;

		sValue.erase(std::remove(sValue.begin(), sValue.end(), '\"'), sValue.end());

		if (!ContainDatabaseIndex(sKey))
		{
			std::istringstream cIss2(sValue);
			int iValue;
			if (cIss2 >> iValue)
			{
				std::transform(sKey.begin(), sKey.end(), sKey.begin(), ::toupper);
				if (sKey == "ROTATION_DAY_OF_WEEK")
				{
					m_iRotationDayOfWeek = iValue;
				}
				else if (sKey == "COUNT_DAILY")
				{
					m_iCountDaily = iValue;
				}
				else if (sKey == "COUNT_WEEKLY")
				{
					m_iCountWeekly = iValue;
				}
				else if (sKey == "COUNT_MONTHLY")
				{
					m_iCountMonthly = iValue;
				}
			}
		}
		else
		{
			std::size_t iDbIndex = sKey.find('_');
			if (iDbIndex == std::string::npos) continue;

			std::string sDbPrefix = sKey.substr(0, iDbIndex);
			std::string sDbParam = sKey.substr(iDbIndex + 1);

			std::transform(sDbPrefix.begin(), sDbPrefix.end(), sDbPrefix.begin(), ::toupper);
			std::transform(sDbParam.begin(), sDbParam.end(), sDbParam.begin(), ::toupper);

			if (cDbMap.find(sDbPrefix) == cDbMap.end())
			{
				cDbMap[sDbPrefix] = std::make_shared<SDatabaseConfig>();
			}
			std::shared_ptr<SDatabaseConfig>& rDbInfo = cDbMap[sDbPrefix];

			if (sDbParam == "USERNAME")
			{
				rDbInfo->m_sUsername = sValue;
			}
			else if (sDbParam == "PASS")
			{
				rDbInfo->m_sPassword = sValue;
			}
			else if (sDbParam == "HOST")
			{
				rDbInfo->m_sHost = sValue;
			}
			else if (sDbParam == "PORT")
			{
				std::istringstream cIss2(sValue);
				int iPort;
				if (!(cIss2 >> iPort))
				{
					std::cerr << "Error: Invalid port value for database" << std::endl;
				}
				else
				{
					rDbInfo->m_iPort = iPort;
				}
			}
			else if (sDbParam == "NAME")
			{
				rDbInfo->m_sDatabaseName = sValue;
			}
			else if (sDbParam == "CONTAINER_ID")
			{
				rDbInfo->m_sContainerName = sValue;
			}
			else if (sDbParam == "BACKUP_DIR")
			{
				rDbInfo->m_sBackupDirectory = sValue;
			}
			else if (sDbParam == "TYPE")
			{
				if (sValue == "mysql")
					rDbInfo->m_eDatabaseProvider = EDatabaseProvider::Mysql;
				else if (sValue == "MSSQL")
					rDbInfo->m_eDatabaseProvider = EDatabaseProvider::MSSQL;
			}
		}
	}
	cFile.close();

	for (const auto& rPair : cDbMap)
	{
		m_cDatabases.push_back(rPair.second);
	}

	return true;
}

const std::vector<std::shared_ptr<SDatabaseConfig>>& CConfiguration::GetDatabases() const
{
	return m_cDatabases;
}

bool CConfiguration::ContainDatabaseIndex(const std::string& _rStr)
{
	std::regex cPattern("^DB\\d+");
	return std::regex_search(_rStr, cPattern);
}
