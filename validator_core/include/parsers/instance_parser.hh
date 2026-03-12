#pragma once
#include <string>
#include <vector>

#include "models/instance.hh"
#include "models/validation_response.hh"

namespace validator {

struct InstanceParseResult {
    bool success = false;
    Instance instance;
    std::vector<Diagnostic> diagnostics;
};

class InstanceParser {
public:
    InstanceParseResult parse(const std::string& text) const;
};

} // namespace validator