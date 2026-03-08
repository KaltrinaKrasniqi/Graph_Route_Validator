#include "validation/submission_format_validator.hh"

#include <string>

namespace validator {

namespace {

void addError(
    ValidationResponse& response,
    const std::string& code,
    const std::string& message,
    const std::string& context = "") {

    response.diagnostics.push_back({
        DiagnosticSeverity::Error,
        code,
        message,
        context
    });
}

} // namespace

bool SubmissionFormatValidator::validate(
    const Instance& instance,
    const Submission& submission,
    ValidationResponse& response) const {

    if (submission.declared_vehicle_count != instance.vehicles) {
        addError(response,
                 "SUBFMT_VEHICLE_COUNT_MISMATCH",
                 "Submission vehicle count does not match instance vehicle count.",
                 "declared=" + std::to_string(submission.declared_vehicle_count) +
                 ", expected=" + std::to_string(instance.vehicles));
        return false;
    }

    if (static_cast<int>(submission.routes.size()) != submission.declared_vehicle_count) {
        addError(response,
                 "SUBFMT_ROUTE_BLOCK_COUNT_MISMATCH",
                 "Number of parsed route blocks does not match declared vehicle count.",
                 "routes=" + std::to_string(submission.routes.size()) +
                 ", declared=" + std::to_string(submission.declared_vehicle_count));
        return false;
    }

    for (int i = 0; i < static_cast<int>(submission.routes.size()); ++i) {
        const auto& route = submission.routes[i];

        for (int node : route.route_nodes) {
            if (node < 0 || node >= instance.nodes) {
                addError(response,
                         "SUBFMT_ROUTE_NODE_OUT_OF_RANGE",
                         "Route node index is out of range.",
                         "vehicle=" + std::to_string(i) +
                         ", node=" + std::to_string(node));
                return false;
            }
        }

        for (int street_id : route.cleaned_street_ids) {
            if (street_id < 0 || street_id >= instance.streets) {
                addError(response,
                         "SUBFMT_STREET_ID_OUT_OF_RANGE",
                         "Cleaned street id is out of range.",
                         "vehicle=" + std::to_string(i) +
                         ", street_id=" + std::to_string(street_id));
                return false;
            }
        }
    }

    return true;
}

} // namespace validator