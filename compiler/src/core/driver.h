#pragma once
#include "defines.h"
#include <iostream>
#include <string>
#include <vector>

namespace core {

class Driver {
public:
    Driver(std::vector<std::string> args)
        : m_args(args) {}

    void process_args();
private:
    std::vector<std::string> m_args;
};

}
