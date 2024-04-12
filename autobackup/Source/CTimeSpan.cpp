#include "../Include/CTimeSpan.h"

CTimeSpan::CTimeSpan()
{
    m_iSeconds = 0;
}

CTimeSpan::CTimeSpan(const unsigned long long & _iSeconds)
{
    m_iSeconds = _iSeconds;
}

int CTimeSpan::GetTotalSeconds() const
{
    return m_iSeconds;
}

double CTimeSpan::GetTotalMinutes() const
{
    return (double)m_iSeconds / 60.0;
}
double CTimeSpan::GetTotalHours() const
{
    return GetTotalMinutes() / 60.0;
}

double CTimeSpan::GetTotalDays() const
{
    return GetTotalHours() / 24.0;
}

void CTimeSpan::SetSeconds(const unsigned long long &_iSeconds)
{
    m_iSeconds = _iSeconds;
}
