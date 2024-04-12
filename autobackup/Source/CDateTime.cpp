#include "../Include/CDateTime.h"

CDateTime::CDateTime()
{
    m_iYear = m_iMonth = m_iDay = m_iHour = m_iMinute = m_iSecond = 0;
}

CDateTime::CDateTime(std::tm * pTm)
{
    m_iYear = pTm->tm_year + 1900;
	m_iMonth = pTm->tm_mon + 1;
	m_iDay = pTm->tm_mday;
	m_iHour = pTm->tm_hour;
	m_iMinute = pTm->tm_min;
	m_iSecond = pTm->tm_sec;
}

CDateTime CDateTime::GetNow()
{
    std::time_t iCurrentTime = std::time(nullptr);
    return CDateTime(std::localtime(&iCurrentTime));
}

CTimeSpan CDateTime::operator-(const CDateTime &_rOther) const
{
    CTimeSpan cTimeSpan;

    std::time_t sStart = ToTime_T();
    std::time_t sEnd = _rOther.ToTime_T();
    
    if (sStart == -1 || sEnd == -1) return cTimeSpan;
    
    double dDif = std::difftime(sStart, sEnd);
    cTimeSpan.SetSeconds(static_cast<int>(dDif));
    return cTimeSpan;
}

std::string CDateTime::ToString() const
{
    char sBuf[30];
    sprintf(sBuf, "%i-%02i-%02i_%02i-%02i-%02i", m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute, m_iSecond);
    return std::string(sBuf);
}

std::time_t CDateTime::ToTime_T() const
{
    std::tm sTm = {0};
    sTm.tm_year = m_iYear - 1900;
    sTm.tm_mon = m_iMonth - 1;
    sTm.tm_mday = m_iDay;
    sTm.tm_hour = m_iHour;
    sTm.tm_min = m_iMinute;
    sTm.tm_sec = m_iSecond;
    return std::mktime(&sTm);
}