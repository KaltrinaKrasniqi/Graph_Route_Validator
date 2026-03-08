#pragma once
#include <vector>
#include "vehicle.hh"

namespace validator {

struct Submission {
    int declared_vehicle_count = 0;
    std::vector<VehicleRoute> routes;
};

} // namespace validator