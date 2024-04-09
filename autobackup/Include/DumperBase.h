#pragma once

#include <memory>
#include "SDatabaseConfig.h"

class DumperBase
{
public:
    DumperBase(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump() = 0;
    
    std::string GetCurrentDateString();
    std::string GetDumpFileName();

protected:
    std::shared_ptr<SDatabaseConfig> pConfig;
    std::string sDumpFileName;

};