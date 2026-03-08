#include "validation/instance_validator.hh"

#include <set>
#include <string>
#include <utility>

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

std::pair<int, int> normalizedPair(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}

bool isValidRequirement(int value) {
    return value == 0 || value == 10 || value == 20 || value == 30;
}

} // namespace

bool InstanceValidator::validate(
    const Instance& instance,
    ValidationResponse& response) const {

    // Step entry should already be added by engine.
    // This method only appends diagnostics and returns pass/fail.

    // Header checks
    if (instance.nodes <= 0) {
        addError(response,
                 "INST_HEADER_INVALID",
                 "Number of nodes must be positive.",
                 "nodes=" + std::to_string(instance.nodes));
        return false;
    }

    if (instance.streets < 0) {
        addError(response,
                 "INST_HEADER_INVALID",
                 "Number of streets cannot be negative.",
                 "streets=" + std::to_string(instance.streets));
        return false;
    }

    if (instance.vehicles <= 0) {
        addError(response,
                 "INST_HEADER_INVALID",
                 "Number of vehicles must be positive.",
                 "vehicles=" + std::to_string(instance.vehicles));
        return false;
    }

    if (instance.time_limit < 0) {
        addError(response,
                 "INST_HEADER_INVALID",
                 "Time limit cannot be negative.",
                 "time_limit=" + std::to_string(instance.time_limit));
        return false;
    }

    if (instance.depot < 0 || instance.depot >= instance.nodes) {
        addError(response,
                 "INST_INVALID_DEPOT",
                 "Depot index is out of range.",
                 "depot=" + std::to_string(instance.depot));
        return false;
    }

    // Vehicle type count check
    if (static_cast<int>(instance.vehicle_types.size()) != instance.vehicles) {
        addError(response,
                 "INST_VEHICLE_TYPE_COUNT_MISMATCH",
                 "Vehicle type count does not match declared vehicle count.",
                 "declared=" + std::to_string(instance.vehicles) +
                 ", actual=" + std::to_string(instance.vehicle_types.size()));
        return false;
    }

    // Street count check
    if (static_cast<int>(instance.street_list.size()) != instance.streets) {
        addError(response,
                 "INST_STREET_COUNT_MISMATCH",
                 "Parsed street count does not match declared street count.",
                 "declared=" + std::to_string(instance.streets) +
                 ", actual=" + std::to_string(instance.street_list.size()));
        return false;
    }

    std::set<std::pair<int, int>> seen_pairs;

    for (const auto& street : instance.street_list) {
        // Endpoint range
        if (street.from < 0 || street.from >= instance.nodes) {
            addError(response,
                     "INST_INVALID_NODE_INDEX",
                     "Street 'from' endpoint is out of range.",
                     "street=" + std::to_string(street.id) +
                     ", from=" + std::to_string(street.from));
            return false;
        }

        if (street.to < 0 || street.to >= instance.nodes) {
            addError(response,
                     "INST_INVALID_NODE_INDEX",
                     "Street 'to' endpoint is out of range.",
                     "street=" + std::to_string(street.id) +
                     ", to=" + std::to_string(street.to));
            return false;
        }

        // Self-loop
        if (street.from == street.to) {
            addError(response,
                     "INST_SELF_LOOP",
                     "Street cannot connect a node to itself.",
                     "street=" + std::to_string(street.id));
            return false;
        }

        // Traversal time
        if (street.traversal_time < 0) {
            addError(response,
                     "INST_INVALID_TRAVERSAL_TIME",
                     "Street traversal time cannot be negative.",
                     "street=" + std::to_string(street.id));
            return false;
        }

        // Length
        if (street.length < 0) {
            addError(response,
                     "INST_INVALID_LENGTH",
                     "Street length cannot be negative.",
                     "street=" + std::to_string(street.id));
            return false;
        }

        // Requirement
        if (!isValidRequirement(street.requirement)) {
            addError(response,
                     "INST_INVALID_REQUIREMENT",
                     "Street requirement must be one of {0, 10, 20, 30}.",
                     "street=" + std::to_string(street.id) +
                     ", requirement=" + std::to_string(street.requirement));
            return false;
        }

        // Category consistency
        switch (street.category) {
            case StreetCategory::Connector:
                if (street.length != 0 || street.requirement != 0) {
                    addError(response,
                             "INST_CONNECTOR_INCONSISTENT",
                             "Connector streets must have length 0 and requirement 0.",
                             "street=" + std::to_string(street.id));
                    return false;
                }
                break;

            case StreetCategory::Mandatory:
            case StreetCategory::Optional:
                if (street.requirement == 0) {
                    addError(response,
                             "INST_INVALID_REQUIREMENT",
                             "Mandatory/optional streets must have requirement in {10, 20, 30}.",
                             "street=" + std::to_string(street.id));
                    return false;
                }
                break;
        }

        // Duplicate unordered pair rule
        auto p = normalizedPair(street.from, street.to);
        if (seen_pairs.count(p)) {
            addError(response,
                     "INST_DUPLICATE_EDGE",
                     "More than one street exists for the same node pair.",
                     "street=" + std::to_string(street.id) +
                     ", pair=(" + std::to_string(p.first) +
                     "," + std::to_string(p.second) + ")");
            return false;
        }
        seen_pairs.insert(p);
    }

    return true;
}

} // namespace validator