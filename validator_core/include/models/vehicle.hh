#pragma once
#include <vector>

namespace validator {

enum class VehicleType {
    Small,
    Medium,
    Large
};

struct VehicleRoute {
    int vehicle_index = -1;
    VehicleType type = VehicleType::Small;
    std::vector<int> route_nodes;
    std::vector<int> cleaned_street_ids;
};

} // namespace validator