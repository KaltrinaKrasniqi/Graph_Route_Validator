#pragma once
#include <vector>
#include "street.hh"
#include "vehicle.hh"
#include "junction.hh"

namespace validator {

struct Instance {
    int nodes = 0;
    int streets = 0;
    int time_limit = 0;
    int vehicles = 0;
    int depot = 0;
    int waste_penalty = 0;

    std::vector<Junction> junctions;
    std::vector<Street> street_list;
    std::vector<VehicleType> vehicle_types;
};

} // namespace validator