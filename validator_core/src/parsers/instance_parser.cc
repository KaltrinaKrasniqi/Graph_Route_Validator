#include "parsers/instance_parser.hh"

#include <sstream>

namespace validator {

InstanceParseResult InstanceParser::parse(const std::string& text) const {

    InstanceParseResult result;

    std::stringstream ss(text);

    Instance instance;

    if (!(ss >> instance.nodes
         >> instance.streets
         >> instance.time_limit
         >> instance.vehicles
         >> instance.depot)) {

    result.error_message = "Failed to parse instance header";
    return result;
    }

    if (!(ss >> instance.waste_penalty)) {
    instance.waste_penalty = 0;
   }

//     for (int i = 0; i < instance.nodes; i++) {
//     Junction junction;

//     if (!(ss >> junction.x >> junction.y)) {
//         result.error_message = "Failed to parse junction data";
//         return result;
//     }

//     instance.junctions.push_back(junction);
//   }
    
    // Parse streets

    for (int i = 0; i < instance.streets; i++) {

        Street street;

        street.id = i;

        int direction;
        char category;

        if (!(ss >> street.from
                 >> street.to
                 >> direction
                 >> street.traversal_time
                 >> street.length
                 >> category
                 >> street.requirement)) {

            result.error_message = "Failed to parse street definition";
            return result;
        }

        // Direction

        if (direction == 1)
            street.direction = Direction::OneWay;
        else if (direction == 2)
            street.direction = Direction::TwoWay;
        else {
            result.error_message = "Invalid street direction";
            return result;
        }

        // Category

        if (category == 'M')
            street.category = StreetCategory::Mandatory;
        else if (category == 'O')
            street.category = StreetCategory::Optional;
        else if (category == 'C')
            street.category = StreetCategory::Connector;
        else {
            result.error_message = "Invalid street category";
            return result;
        }

        instance.street_list.push_back(street);
    }

    // Parse vehicle types

    for (int i = 0; i < instance.vehicles; i++) {

        char type;

        if (!(ss >> type)) {

            result.error_message = "Failed to read vehicle types";
            return result;
        }

        if (type == 'S')
            instance.vehicle_types.push_back(VehicleType::Small);
        else if (type == 'M')
            instance.vehicle_types.push_back(VehicleType::Medium);
        else if (type == 'L')
            instance.vehicle_types.push_back(VehicleType::Large);
        else {
            result.error_message = "Invalid vehicle type";
            return result;
        }
    }

    result.success = true;
    result.instance = instance;

    return result;
}

}