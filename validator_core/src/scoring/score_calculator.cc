#include "scoring/score_calculator.hh"

#include <set>
#include <vector>

namespace validator {

namespace {

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

} // namespace

ScoreBreakdown ScoreCalculator::compute(
    const Instance& instance,
    const Submission& submission) const {

    ScoreBreakdown breakdown;

    breakdown.alpha = instance.alpha;

    std::set<int> distinct_cleaned_streets;

    double total_waste = 0.0;
    int cleaned_mandatory_count = 0;
    int cleaned_optional_count = 0;
    double distinct_cleaned_length = 0.0;

    double l_max = 0.0;
    double w_max = 0.0;

    // Compute normalization bounds from the instance
    for (const auto& street : instance.street_list) {
        if (street.category == StreetCategory::Mandatory ||
            street.category == StreetCategory::Optional) {

            l_max += static_cast<double>(street.length);

            double street_length_km = static_cast<double>(street.length) / 1000.0;
            w_max += static_cast<double>(30 - street.requirement) * street_length_km;
        }
    }

    // Compute submission-based totals
    for (int vehicle_index = 0;
         vehicle_index < static_cast<int>(submission.routes.size());
         ++vehicle_index) {

        const auto& route = submission.routes[vehicle_index];
        int capacity = vehicleCapacity(instance.vehicle_types[vehicle_index]);

        for (int street_id : route.cleaned_street_ids) {
            const Street& street = instance.street_list[street_id];

            double street_length_km = static_cast<double>(street.length) / 1000.0;

            // Waste counts for every cleaning action
            double waste_for_action =
                static_cast<double>(capacity - street.requirement) * street_length_km;

            total_waste += waste_for_action;

            // Distinct cleaned length counts only once per street
            if (!distinct_cleaned_streets.count(street_id)) {
                distinct_cleaned_streets.insert(street_id);

                if (street.category == StreetCategory::Mandatory) {
                    ++cleaned_mandatory_count;
                    distinct_cleaned_length += static_cast<double>(street.length);
                } else if (street.category == StreetCategory::Optional) {
                    ++cleaned_optional_count;
                    distinct_cleaned_length += static_cast<double>(street.length);
                }
            }
        }
    }

    double coverage = 0.0;
    if (l_max > 0.0) {
        coverage = distinct_cleaned_length / l_max;
    }

    double efficiency = 1.0;
    if (w_max > 0.0) {
        efficiency = 1.0 - (total_waste / w_max);
    }

    double final_score =
        instance.alpha * coverage +
        (1.0 - instance.alpha) * efficiency;

    breakdown.cleanedMandatoryCount = cleaned_mandatory_count;
    breakdown.cleanedOptionalCount = cleaned_optional_count;
    breakdown.distinctCleanedLength = distinct_cleaned_length;
    breakdown.totalWaste = total_waste;
    breakdown.lMax = l_max;
    breakdown.wMax = w_max;
    breakdown.coverage = coverage;
    breakdown.efficiency = efficiency;
    breakdown.finalScore = final_score;

    return breakdown;
}

} // namespace validator