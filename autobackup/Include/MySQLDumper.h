#pragma once

#include "DumperBase.h"

class MySQLDumper : public DumperBase
{
public:
    MySQLDumper(const std::shared_ptr<SDatabaseConfig>& _rConfig);

    virtual void Dump();

private:
    void MakeDirectory(const std::string& _sPath);

};