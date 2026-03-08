#pragma once
#include "models/instance.hh"
#include "models/validation_response.hh"

namespace validator {

class InstanceValidator {
public:
    bool validate(const Instance& instance, ValidationResponse& response) const;
};

} // namespace validator