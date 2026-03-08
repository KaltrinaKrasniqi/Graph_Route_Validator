#pragma once
#include <string>
#include "models/instance.hh"

namespace validator {

struct InstanceParseResult {
    bool success = false;
    std::string error_message;
    Instance instance;
};

class InstanceParser {
public:
    InstanceParseResult parse(const std::string& text) const;
};

} // namespace validator