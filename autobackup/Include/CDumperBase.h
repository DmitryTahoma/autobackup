#pragma once

#include <memory>
#include <ctime>

#include "SDatabaseConfig.h"

class CDumperBase
{
public:
    CDumperBase(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump() = 0;
    
    std::string GetCurrentDateString();
    std::string GetDumpFileName();

protected:
    std::shared_ptr<SDatabaseConfig> m_pConfig;
    std::string m_sDumpFileName;

};