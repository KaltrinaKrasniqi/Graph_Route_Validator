#include "parsers/instance_parser.hh"

#include <sstream>

namespace validator {

namespace {

void addParseError(InstanceParseResult& result,
                   const std::string& code,
                   const std::string& message,
                   const std::string& context = "") {
    result.diagnostics.push_back({
        DiagnosticSeverity::Error,
        code,
        message,
        context
    });
}

} // namespace

InstanceParseResult InstanceParser::parse(const std::string& text) const {
    InstanceParseResult result;

    std::stringstream ss(text);
    Instance instance;

    std::string headerLine;
    if (!std::getline(ss, headerLine)) {
        addParseError(result,
                      "INST_PARSE_HEADER_MISSING",
                      "Missing instance header line.");
        return result;
    }

    std::stringstream header(headerLine);

    if (!(header >> instance.nodes
                 >> instance.streets
                 >> instance.time_limit
                 >> instance.vehicles
                 >> instance.depot)) {
        addParseError(result,
                      "INST_PARSE_HEADER_FAILED",
                      "Failed to parse instance header.");
        return result;
    }

    // Optional waste penalty
    if (!(header >> instance.waste_penalty)) {
        instance.waste_penalty = 0;
    }

    // Parse streets
    for (int i = 0; i < instance.streets; i++) {
        Street street;
        street.id = i;

        int direction = 0;
        char category = '\0';

        if (!(ss >> street.from
                 >> street.to
                 >> direction
                 >> street.traversal_time
                 >> street.length
                 >> category
                 >> street.requirement)) {
            addParseError(result,
                          "INST_PARSE_STREET_FAILED",
                          "Failed to parse street definition.",
                          "street_id=" + std::to_string(i));
            return result;
        }

        if (direction == 1) {
            street.direction = Direction::OneWay;
        } else if (direction == 2) {
            street.direction = Direction::TwoWay;
        } else {
            addParseError(result,
                          "INST_PARSE_DIRECTION_INVALID",
                          "Invalid street direction.",
                          "street_id=" + std::to_string(i));
            return result;
        }

        if (category == 'M') {
            street.category = StreetCategory::Mandatory;
        } else if (category == 'O') {
            street.category = StreetCategory::Optional;
        } else if (category == 'C') {
            street.category = StreetCategory::Connector;
        } else {
            addParseError(result,
                          "INST_PARSE_CATEGORY_INVALID",
                          "Invalid street category.",
                          "street_id=" + std::to_string(i));
            return result;
        }

        instance.street_list.push_back(street);
    }

    // Parse vehicle types
    for (int i = 0; i < instance.vehicles; i++) {
        char type = '\0';

        if (!(ss >> type)) {
            addParseError(result,
                          "INST_PARSE_VEHICLE_TYPE_MISSING",
                          "Failed to read vehicle types.",
                          "vehicle_index=" + std::to_string(i));
            return result;
        }

        if (type == 'S') {
            instance.vehicle_types.push_back(VehicleType::Small);
        } else if (type == 'M') {
            instance.vehicle_types.push_back(VehicleType::Medium);
        } else if (type == 'L') {
            instance.vehicle_types.push_back(VehicleType::Large);
        } else {
            addParseError(result,
                          "INST_PARSE_VEHICLE_TYPE_INVALID",
                          "Invalid vehicle type.",
                          "vehicle_index=" + std::to_string(i));
            return result;
        }
    }

    result.success = true;
    result.instance = instance;
    return result;
}

} // namespace validator