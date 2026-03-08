#pragma once
#include <string>

namespace validator {

enum class StreetCategory {
    Mandatory,
    Optional,
    Connector
};

enum class Direction {
    OneWay,
    TwoWay
};

struct Street {
    int id = -1;
    int from = -1;
    int to = -1;
    Direction direction = Direction::OneWay;
    int traversal_time = 0;
    int length = 0;
    StreetCategory category = StreetCategory::Connector;
    int requirement = 0;
};

} // namespace validator