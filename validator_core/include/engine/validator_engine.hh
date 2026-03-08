#pragma once

#include <string>
#include "models/validation_response.hh"

namespace validator {

class ValidatorEngine {
public:
    ValidationResponse evaluate(const std::string& instance_text,
                                const std::string& submission_text);
};

} // namespace validator