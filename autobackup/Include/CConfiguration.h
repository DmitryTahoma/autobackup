#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <regex>

#include "SDatabaseConfig.h"

class CConfiguration
{
public:
	CConfiguration();

	int GetRotationDayOfWeek() const;
	int GetCountDaily() const;
	int GetCountWeekly() const;
	int GetCountMonthly() const;
	std::string GetLogFile() const;
	bool IsDebug() const;

	bool Load(const char *_sPath);
	const std::vector<std::shared_ptr<SDatabaseConfig>> &GetDatabases() const;

private:
	bool ContainDatabaseIndex(const std::string &_rStr);

private:
	std::vector<std::shared_ptr<SDatabaseConfig>> m_cDatabases;

	int m_iRotationDayOfWeek;

	int m_iCountDaily;
	int m_iCountWeekly;
	int m_iCountMonthly;

	std::string m_sLogFile;
	bool m_bDebug;
};