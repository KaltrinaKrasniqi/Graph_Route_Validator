#pragma once

#include <string>
#include "models/validation_response.hh"

namespace validator {

class JsonSerializer {
public:
    static std::string serialize(const ValidationResponse& response);
};

} // namespace validator