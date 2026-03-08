#pragma once
#include <string>
#include "models/submission.hh"

namespace validator {

struct SubmissionParseResult {
    bool success = false;
    std::string error_message;
    Submission submission;
};

class SubmissionParser {
public:
    SubmissionParseResult parse(const std::string& text) const;
};

} // namespace validator