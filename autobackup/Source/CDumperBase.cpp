#include "../Include/CDumperBase.h"

CDumperBase::CDumperBase(const std::shared_ptr<SDatabaseConfig>& _rConfig)
{
    m_pConfig = _rConfig;
}

std::string CDumperBase::GetCurrentDateString()
{
    std::time_t iCurrentTime = std::time(nullptr);
	std::tm* pLocalTime = std::localtime(&iCurrentTime);

	int iYear = pLocalTime->tm_year + 1900;
	int iMonth = pLocalTime->tm_mon + 1;
	int iDay = pLocalTime->tm_mday;
	int iHours = pLocalTime->tm_hour;
	int iMinutes = pLocalTime->tm_min;
	int iSeconds = pLocalTime->tm_sec;

    char sBuf[30];
    sprintf(sBuf, "%i-%02i-%02i_%02i-%02i-%02i", iYear, iMonth, iDay, iHours, iMinutes, iSeconds);
    return std::string(sBuf);
}

std::string CDumperBase::GetDumpFileName()
{
	return m_sDumpFileName;
}