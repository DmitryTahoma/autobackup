#pragma once

#include "CTimeSpan.h"

#include <ctime>
#include <string>

class CDateTime
{
public:
    CDateTime();
    CDateTime(std::tm* pTm);
    
    static CDateTime GetNow();

    int GetDayOfWeek();
    int GetMonth();

    CTimeSpan operator-(const CDateTime& _rOther) const;

    std::string ToString() const;

private:
    std::time_t ToTime_T() const;

private:
    int m_iYear;
    int m_iMonth;
    int m_iDay;
    int m_iHour;
    int m_iMinute;
    int m_iSecond;

};