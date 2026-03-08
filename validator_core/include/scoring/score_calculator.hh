#pragma once
#include "models/instance.hh"
#include "models/submission.hh"
#include "models/validation_response.hh"

namespace validator {

class ScoreCalculator {
public:
    ScoreBreakdown compute(const Instance& instance,
                           const Submission& submission) const;
};

} // namespace validator