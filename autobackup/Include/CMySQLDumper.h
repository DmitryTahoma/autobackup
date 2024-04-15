#pragma once

#include <sstream>

#include "CDumperBase.h"
#include "CDirectory.h"

class CMySQLDumper : public CDumperBase
{
public:
    CMySQLDumper(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump();

};