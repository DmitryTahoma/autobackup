#pragma once

class CTimeSpan
{
public:
    CTimeSpan();
    CTimeSpan(const unsigned long long& _iSeconds);

    int GetTotalSeconds() const;
    double GetTotalMinutes() const;
    double GetTotalHours() const;
    double GetTotalDays() const;

    void SetSeconds(const unsigned long long& _iSeconds);

private:
    unsigned long long m_iSeconds;

};