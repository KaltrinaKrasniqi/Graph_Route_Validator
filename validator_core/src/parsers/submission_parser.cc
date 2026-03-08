#include "parsers/submission_parser.hh"

#include <sstream>
#include <string>

namespace validator {

namespace {

// Trim only trailing '\r' for Windows line endings
std::string normalizeLine(std::string line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

bool parseSingleIntegerLine(const std::string& line, int& value) {
    std::stringstream ss(line);
    if (!(ss >> value)) {
        return false;
    }

    // Reject extra tokens
    std::string extra;
    if (ss >> extra) {
        return false;
    }

    return true;
}

bool parseIntegerListLine(const std::string& line, std::vector<int>& values) {
    values.clear();
    std::stringstream ss(line);

    int x = 0;
    while (ss >> x) {
        values.push_back(x);
    }

    // If parsing stopped because of invalid token, fail
    if (!ss.eof()) {
        return false;
    }

    return true;
}

} // namespace

SubmissionParseResult SubmissionParser::parse(const std::string& text) const {
    SubmissionParseResult result;

    std::stringstream input(text);
    Submission submission;

    std::string line;

    // Line 1: declared vehicle count
    if (!std::getline(input, line)) {
        result.error_message = "Missing submission vehicle count line";
        return result;
    }

    line = normalizeLine(line);

    if (!parseSingleIntegerLine(line, submission.declared_vehicle_count)) {
        result.error_message = "Failed to parse submission vehicle count";
        return result;
    }

    if (submission.declared_vehicle_count < 0) {
        result.error_message = "Submission vehicle count cannot be negative";
        return result;
    }

    // For each vehicle, read 3 lines:
    // 1) n
    // 2) route nodes
    // 3) cleaned street ids (possibly empty)
    for (int vehicle_index = 0; vehicle_index < submission.declared_vehicle_count; ++vehicle_index) {
        VehicleRoute route;
        route.vehicle_index = vehicle_index;

        int n = 0;

        // Read n
        if (!std::getline(input, line)) {
            result.error_message =
                "Missing route length line for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        line = normalizeLine(line);

        if (!parseSingleIntegerLine(line, n)) {
            result.error_message =
                "Failed to parse route length for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        if (n < 0) {
            result.error_message =
                "Negative route length for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        // Read route nodes line
        if (!std::getline(input, line)) {
            result.error_message =
                "Missing route nodes line for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        line = normalizeLine(line);

        if (!parseIntegerListLine(line, route.route_nodes)) {
            result.error_message =
                "Failed to parse route nodes for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        if (static_cast<int>(route.route_nodes.size()) != n) {
            result.error_message =
                "Route node count mismatch for vehicle " + std::to_string(vehicle_index) +
                ": expected " + std::to_string(n) +
                ", got " + std::to_string(route.route_nodes.size());
            return result;
        }

        // Read cleaned street ids line
        if (!std::getline(input, line)) {
            result.error_message =
                "Missing cleaned streets line for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        line = normalizeLine(line);

        if (!parseIntegerListLine(line, route.cleaned_street_ids)) {
            result.error_message =
                "Failed to parse cleaned street ids for vehicle " + std::to_string(vehicle_index);
            return result;
        }

        submission.routes.push_back(route);
    }

    result.success = true;
    result.submission = submission;
    return result;
}

} // namespace validator