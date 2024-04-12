#pragma once

class CTimeSpan
{
public:
    CTimeSpan();
    CTimeSpan(const long long& _iSeconds);

    int GetTotalSeconds() const;
    double GetTotalMinutes() const;
    double GetTotalHours() const;
    double GetTotalDays() const;

    void SetSeconds(const long long& _iSeconds);

private:
    long long m_iSeconds;

};