#pragma once
#include "models/instance.hh"
#include "models/submission.hh"
#include "models/validation_response.hh"

namespace validator {

class SubmissionFormatValidator {
public:
    bool validate(const Instance& instance,
                  const Submission& submission,
                  ValidationResponse& response) const;
};

} // namespace validator