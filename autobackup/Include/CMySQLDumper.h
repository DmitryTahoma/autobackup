#pragma once

#include <iostream>
#include <sstream>

#include "CDumperBase.h"

class CMySQLDumper : public CDumperBase
{
public:
    CMySQLDumper(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump();

private:
    void MakeDirectory(const std::string& _sPath);

};