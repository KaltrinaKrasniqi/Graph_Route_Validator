#include "serialization/json_serialization.hh"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace validator {

namespace {

std::string statusToString(ValidatorStatus s) {
    switch (s) {
        case ValidatorStatus::ErrorInstance:
            return "ERROR_INSTANCE";
        case ValidatorStatus::ErrorSubmissionFormat:
            return "ERROR_SUBMISSION_FORMAT";
        case ValidatorStatus::Invalid:
            return "INVALID";
        case ValidatorStatus::Valid:
            return "VALID";
    }

    return "ERROR_INSTANCE";
}

std::string severityToString(DiagnosticSeverity s) {
    switch (s) {
        case DiagnosticSeverity::Info:
            return "info";
        case DiagnosticSeverity::Warning:
            return "warning";
        case DiagnosticSeverity::Error:
            return "error";
    }

    return "error";
}

} // namespace

std::string JsonSerializer::serialize(const ValidationResponse& response) {
    json j;

    j["status"] = statusToString(response.status);
    j["summary"] = response.summary;
    j["score"] = response.score;

    if (response.parsedSummary.has_value()) {
        const auto& ps = response.parsedSummary.value();
        j["parsedSummary"] = {
            {"nodes", ps.nodes},
            {"streets", ps.streets},
            {"vehicles", ps.vehicles},
            {"depot", ps.depot},
            {"timeLimit", ps.timeLimit},
            {"wastePenalty", ps.wastePenalty},
            {"mandatoryStreetCount", ps.mandatoryStreetCount},
            {"optionalStreetCount", ps.optionalStreetCount},
            {"connectorStreetCount", ps.connectorStreetCount}
        };
    } else {
        j["parsedSummary"] = nullptr;
    }

    j["steps"] = json::array();
    for (const auto& step : response.steps) {
        j["steps"].push_back({
            {"name", step.name},
            {"success", step.success},
            {"details", step.details}
        });
    }

    j["diagnostics"] = json::array();
    for (const auto& diagnostic : response.diagnostics) {
        j["diagnostics"].push_back({
            {"severity", severityToString(diagnostic.severity)},
            {"code", diagnostic.code},
            {"message", diagnostic.message},
            {"context", diagnostic.context}
        });
    }

    if (response.scoreBreakdown.has_value()) {
        const auto& sb = response.scoreBreakdown.value();
        j["scoreBreakdown"] = {
            {"cleanedMandatoryCount", sb.cleanedMandatoryCount},
            {"cleanedOptionalCount", sb.cleanedOptionalCount},
            {"distinctCleanedLength", sb.distinctCleanedLength},
            {"totalWaste", sb.totalWaste},
            {"wastePenalty", sb.wastePenalty},
            {"finalScore", sb.finalScore}
        };
    } else {
        j["scoreBreakdown"] = nullptr;
    }

    return j.dump(2);
}

} // namespace validator