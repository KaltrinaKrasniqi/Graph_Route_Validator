#pragma once
#include <optional>
#include <string>
#include <vector>

namespace validator {

enum class ValidatorStatus {
    ErrorInstance,
    ErrorSubmissionFormat,
    Invalid,
    Valid
};

enum class DiagnosticSeverity {
    Info,
    Warning,
    Error
};

struct ParsedSummary {
    int nodes = 0;
    int streets = 0;
    int vehicles = 0;
    int depot = 0;
    int timeLimit = 0;
    int wastePenalty = 0;
    int mandatoryStreetCount = 0;
    int optionalStreetCount = 0;
    int connectorStreetCount = 0;
};

struct EvaluationStep {
    std::string name;
    bool success = false;
    std::string details;
};

struct Diagnostic {
    DiagnosticSeverity severity = DiagnosticSeverity::Info;
    std::string code;
    std::string message;
    std::string context;
};

struct ScoreBreakdown {
    int cleanedMandatoryCount = 0;
    int cleanedOptionalCount = 0;
    double distinctCleanedLength = 0.0;
    double totalWaste = 0.0;
    int wastePenalty = 0;
    double finalScore = 0.0;
};

struct ValidationResponse {
    ValidatorStatus status = ValidatorStatus::ErrorInstance;
    std::string summary;
    double score = 0.0;
    std::optional<ParsedSummary> parsedSummary;
    std::vector<EvaluationStep> steps;
    std::vector<Diagnostic> diagnostics;
    std::optional<ScoreBreakdown> scoreBreakdown;
};

} // namespace validator