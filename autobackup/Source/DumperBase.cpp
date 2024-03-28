#include <ctime>
#include "../Include/DumperBase.h"

DumperBase::DumperBase(const std::shared_ptr<SDatabaseConfig>& _rConfig)
{
    pConfig = _rConfig;
}

std::string DumperBase::GetCurrentDateString()
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