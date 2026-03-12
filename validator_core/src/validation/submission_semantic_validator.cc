#include "validation/submission_semantic_validator.hh"

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

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

int vehicleCapacity(VehicleType type) {
    switch (type) {
        case VehicleType::Small:
            return 10;
        case VehicleType::Medium:
            return 20;
        case VehicleType::Large:
            return 30;
    }
    return 0;
}

using DirectedEdge = std::pair<int, int>;

std::map<DirectedEdge, int> buildStreetLookup(const Instance& instance) {
    std::map<DirectedEdge, int> lookup;

    for (const auto& street : instance.street_list) {
        lookup[{street.from, street.to}] = street.id;

        if (street.direction == Direction::TwoWay) {
            lookup[{street.to, street.from}] = street.id;
        }
    }

    return lookup;
}

} // namespace

bool SubmissionSemanticValidator::validate(
    const Instance& instance,
    const Submission& submission,
    ValidationResponse& response) const {

    bool is_valid = true;

    std::set<int> globally_cleaned_mandatory;
    const auto street_lookup = buildStreetLookup(instance);

    for (int vehicle_index = 0;
         vehicle_index < static_cast<int>(submission.routes.size());
         ++vehicle_index) {

        const auto& route = submission.routes[vehicle_index];
        VehicleType vehicle_type = instance.vehicle_types[vehicle_index];
        int capacity = vehicleCapacity(vehicle_type);

        bool route_has_usable_structure = true;
        bool route_moves_are_valid = true;

        if (route.route_nodes.empty()) {
            addError(response,
                     "SUB_INVALID_EMPTY_ROUTE",
                     "Vehicle route cannot be empty.",
                     "vehicle=" + std::to_string(vehicle_index));
            is_valid = false;
            route_has_usable_structure = false;
        }

        if (!route.route_nodes.empty()) {
            if (route.route_nodes.front() != instance.depot) {
                addError(response,
                         "SUB_INVALID_START",
                         "Vehicle route must start at the depot.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", expected_depot=" + std::to_string(instance.depot) +
                         ", actual_start=" + std::to_string(route.route_nodes.front()));
                is_valid = false;
            }

            if (route.route_nodes.back() != instance.depot) {
                addError(response,
                         "SUB_INVALID_END",
                         "Vehicle route must end at the depot.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", expected_depot=" + std::to_string(instance.depot) +
                         ", actual_end=" + std::to_string(route.route_nodes.back()));
                is_valid = false;
            }
        }

        if (!route_has_usable_structure) {
            continue;
        }

        int total_time = 0;
        std::vector<int> traversed_street_ids;
        traversed_street_ids.reserve(
            route.route_nodes.size() > 0 ? route.route_nodes.size() - 1 : 0);

        for (int i = 0; i + 1 < static_cast<int>(route.route_nodes.size()); ++i) {
            int u = route.route_nodes[i];
            int v = route.route_nodes[i + 1];

            auto it = street_lookup.find({u, v});
            if (it == street_lookup.end()) {
                addError(response,
                         "SUB_INVALID_MOVE",
                         "Route contains an invalid move or wrong direction.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", from=" + std::to_string(u) +
                         ", to=" + std::to_string(v));
                is_valid = false;
                route_moves_are_valid = false;
                continue;
            }

            int street_id = it->second;
            traversed_street_ids.push_back(street_id);
            total_time += instance.street_list[street_id].traversal_time;
        }

        if (total_time > instance.time_limit) {
            addError(response,
                     "SUB_TIME_LIMIT",
                     "Vehicle exceeds the time limit.",
                     "vehicle=" + std::to_string(vehicle_index) +
                     ", time=" + std::to_string(total_time) +
                     ", limit=" + std::to_string(instance.time_limit));
            is_valid = false;
        }

        // If route moves are invalid, traversed_street_ids may be incomplete or unreliable.
        //Skipping cleaned-street semantic checks for this vehicle to avoid cascaded fake errors.
        if (!route_moves_are_valid) {
            continue;
        }

        std::set<int> traversed_set(
            traversed_street_ids.begin(),
            traversed_street_ids.end());

        for (int street_id : route.cleaned_street_ids) {
            if (!traversed_set.count(street_id)) {
                addError(response,
                         "SUB_CLEAN_NOT_TRAVERSED",
                         "Vehicle cleans a street it did not traverse.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", street_id=" + std::to_string(street_id));
                is_valid = false;
                continue;
            }

            const Street& street = instance.street_list[street_id];

            if (street.category == StreetCategory::Connector) {
                addError(response,
                         "SUB_CONNECTOR_CLEANED",
                         "Connector streets cannot be cleaned.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", street_id=" + std::to_string(street_id));
                is_valid = false;
            }

            if (capacity < street.requirement) {
                addError(response,
                         "SUB_INCOMPATIBLE_CLEAN",
                         "Vehicle type is not capable of cleaning this street.",
                         "vehicle=" + std::to_string(vehicle_index) +
                         ", street_id=" + std::to_string(street_id) +
                         ", capacity=" + std::to_string(capacity) +
                         ", requirement=" + std::to_string(street.requirement));
                is_valid = false;
            }

            if (street.category == StreetCategory::Mandatory) {
                globally_cleaned_mandatory.insert(street_id);
            }
        }
    }

    for (const auto& street : instance.street_list) {
        if (street.category == StreetCategory::Mandatory &&
            !globally_cleaned_mandatory.count(street.id)) {
            addError(response,
                     "SUB_MANDATORY_MISSING",
                     "A mandatory street was not cleaned.",
                     "street_id=" + std::to_string(street.id));
            is_valid = false;
        }
    }

    return is_valid;
}

} // namespace validator