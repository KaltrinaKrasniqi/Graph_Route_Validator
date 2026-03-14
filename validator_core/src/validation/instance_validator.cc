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

    bool is_valid = true;

    // Header checks
    if (instance.nodes < 1 || instance.nodes > 10000) {
        addError(response,
                 "INST_HEADER_INVALID_NODES",
                 "Number of nodes must be in [1, 10000].",
                 "nodes=" + std::to_string(instance.nodes));
        is_valid = false;
    }

    if (instance.streets < 1 || instance.streets > 100000) {
        addError(response,
                 "INST_HEADER_INVALID_STREETS",
                 "Number of streets must be in [1, 100000].",
                 "streets=" + std::to_string(instance.streets));
        is_valid = false;
    }

    if (instance.time_limit < 1 || instance.time_limit > 1000000) {
        addError(response,
                 "INST_HEADER_INVALID_TIME_LIMIT",
                 "Time limit must be in [1, 1000000].",
                 "time_limit=" + std::to_string(instance.time_limit));
        is_valid = false;
    }

    if (instance.vehicles < 1 || instance.vehicles > 100) {
        addError(response,
                 "INST_HEADER_INVALID_VEHICLES",
                 "Number of vehicles must be in [1, 100].",
                 "vehicles=" + std::to_string(instance.vehicles));
        is_valid = false;
    }

    // Only evaluate depot range if node count is itself usable.
    if (instance.nodes >= 1 && instance.nodes <= 10000) {
        if (instance.depot < 0 || instance.depot >= instance.nodes) {
            addError(response,
                     "INST_INVALID_DEPOT",
                     "Depot index is out of range.",
                     "depot=" + std::to_string(instance.depot));
            is_valid = false;
        }
    }

    if (instance.waste_penalty < 0) {
        addError(response,
                 "INST_HEADER_INVALID_WASTE_PENALTY",
                 "Waste penalty must be nonnegative.",
                 "waste_penalty=" + std::to_string(instance.waste_penalty));
        is_valid = false;
    }

    // Vehicle type count check
    if (static_cast<int>(instance.vehicle_types.size()) != instance.vehicles) {
        addError(response,
                 "INST_VEHICLE_TYPE_COUNT_MISMATCH",
                 "Vehicle type count does not match declared vehicle count.",
                 "declared=" + std::to_string(instance.vehicles) +
                 ", actual=" + std::to_string(instance.vehicle_types.size()));
        is_valid = false;
    }

    // Street count check
    if (static_cast<int>(instance.street_list.size()) != instance.streets) {
        addError(response,
                 "INST_STREET_COUNT_MISMATCH",
                 "Parsed street count does not match declared street count.",
                 "declared=" + std::to_string(instance.streets) +
                 ", actual=" + std::to_string(instance.street_list.size()));
        is_valid = false;
    }

    std::set<std::pair<int, int>> seen_pairs;

    const bool node_count_usable = (instance.nodes >= 1 && instance.nodes <= 10000);

    for (const auto& street : instance.street_list) {
        bool endpoints_valid = true;

        // Endpoint range
        if (node_count_usable) {
            if (street.from < 0 || street.from >= instance.nodes) {
                addError(response,
                         "INST_INVALID_NODE_INDEX_FROM",
                         "Street 'from' endpoint is out of range.",
                         "street=" + std::to_string(street.id) +
                         ", from=" + std::to_string(street.from));
                is_valid = false;
                endpoints_valid = false;
            }

            if (street.to < 0 || street.to >= instance.nodes) {
                addError(response,
                         "INST_INVALID_NODE_INDEX_TO",
                         "Street 'to' endpoint is out of range.",
                         "street=" + std::to_string(street.id) +
                         ", to=" + std::to_string(street.to));
                is_valid = false;
                endpoints_valid = false;
            }
        }

        // Self-loop
        if (street.from == street.to) {
            addError(response,
                     "INST_SELF_LOOP",
                     "Street cannot connect a node to itself.",
                     "street=" + std::to_string(street.id));
            is_valid = false;
        }

        // Traversal time
        if (street.traversal_time < 0) {
            addError(response,
                     "INST_INVALID_TRAVERSAL_TIME",
                     "Street traversal time cannot be negative.",
                     "street=" + std::to_string(street.id) +
                     ", traversal_time=" + std::to_string(street.traversal_time));
            is_valid = false;
        }

        // Length
        if (street.length < 0) {
            addError(response,
                     "INST_INVALID_LENGTH",
                     "Street length cannot be negative.",
                     "street=" + std::to_string(street.id) +
                     ", length=" + std::to_string(street.length));
            is_valid = false;
        }

        // Requirement
        if (!isValidRequirement(street.requirement)) {
            addError(response,
                     "INST_INVALID_REQUIREMENT",
                     "Street requirement must be one of {0, 10, 20, 30}.",
                     "street=" + std::to_string(street.id) +
                     ", requirement=" + std::to_string(street.requirement));
            is_valid = false;
        }

        // Category consistency
        switch (street.category) {
            case StreetCategory::Connector:
                if (street.length != 0 || street.requirement != 0) {
                    addError(response,
                             "INST_CONNECTOR_INCONSISTENT",
                             "Connector streets must have length 0 and requirement 0.",
                             "street=" + std::to_string(street.id) +
                             ", length=" + std::to_string(street.length) +
                             ", requirement=" + std::to_string(street.requirement));
                    is_valid = false;
                }
                break;

            case StreetCategory::Mandatory:
            case StreetCategory::Optional:
                if (street.requirement == 0) {
                    addError(response,
                             "INST_CATEGORY_REQUIREMENT_INCONSISTENT",
                             "Mandatory/optional streets must have requirement in {10, 20, 30}.",
                             "street=" + std::to_string(street.id) +
                             ", requirement=" + std::to_string(street.requirement));
                    is_valid = false;
                }
                break;
        }

        // Duplicate unordered pair rule
        // Only do this when endpoints are valid enough.
        if (endpoints_valid) {
            auto p = normalizedPair(street.from, street.to);
            if (seen_pairs.count(p)) {
                addError(response,
                         "INST_DUPLICATE_EDGE",
                         "More than one street exists for the same node pair.",
                         "street=" + std::to_string(street.id) +
                         ", pair=(" + std::to_string(p.first) +
                         "," + std::to_string(p.second) + ")");
                is_valid = false;
            } else {
                seen_pairs.insert(p);
            }
        }
    }

    return is_valid;
}

} // namespace validator