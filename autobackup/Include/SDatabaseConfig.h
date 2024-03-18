#pragma once

#include <string>

#include "EDatabaseProvider.h"

struct SDatabaseConfig
{
public:
	std::string m_sUsername;
	std::string m_sPassword;
	std::string m_sHost;
	unsigned short m_iPort;
	std::string m_sDatabaseName;

	std::string m_sContainerName;
	std::string m_sBackupDirectory;
	EDatabaseProvider m_eDatabaseProvider;

	SDatabaseConfig()
		: m_eDatabaseProvider(EDatabaseProvider::Mysql),
		m_iPort(3306)
	{
	}

};