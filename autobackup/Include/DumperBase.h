#pragma once

#include <memory>
#include "SDatabaseConfig.h"

class DumperBase
{
public:
    DumperBase(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump() = 0;
    
    std::string GetCurrentDateString();

protected:
    std::shared_ptr<SDatabaseConfig> pConfig;

};