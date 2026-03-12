#pragma once
#include <string>
#include <vector>

#include "models/submission.hh"
#include "models/validation_response.hh"

namespace validator {

struct SubmissionParseResult {
    bool success = false;
    Submission submission;
    std::vector<Diagnostic> diagnostics;
};

class SubmissionParser {
public:
    SubmissionParseResult parse(const std::string& text) const;
};

} // namespace validator