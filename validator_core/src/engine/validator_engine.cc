#include "engine/validator_engine.hh"

#include "parsers/instance_parser.hh"
#include "parsers/submission_parser.hh"

#include "validation/instance_validator.hh"
#include "validation/submission_format_validator.hh"
#include "validation/submission_semantic_validator.hh"
#include "scoring/score_calculator.hh"

namespace validator {

ValidationResponse ValidatorEngine::evaluate(
    const std::string& instance_text,
    const std::string& submission_text) {

    ValidationResponse response;

    // ------------------------------------------------
    // Step 1: Parse instance
    // ------------------------------------------------

    response.steps.push_back({
        "Parse instance",
        false,
        ""
    });

    InstanceParser instance_parser;
    auto instance_result = instance_parser.parse(instance_text);

    if (!instance_result.success) {

        response.status = ValidatorStatus::ErrorInstance;
        response.summary = instance_result.error_message;

        response.steps.back().success = false;
        response.steps.back().details = instance_result.error_message;

        response.diagnostics.push_back({
            DiagnosticSeverity::Error,
            "INST_PARSE_FAILED",
            instance_result.error_message,
            ""
        });

        return response;
    }

    response.steps.back().success = true;
    response.steps.back().details = "Instance parsed successfully.";

    const Instance& instance = instance_result.instance;

    // ------------------------------------------------
    // Step 2: Validate instance
    // ------------------------------------------------

    response.steps.push_back({
        "Validate instance",
        false,
        ""
    });

    InstanceValidator instance_validator;

    bool instance_ok =
        instance_validator.validate(instance, response);

    if (!instance_ok) {

        response.status = ValidatorStatus::ErrorInstance;

        response.summary =
            response.diagnostics.empty()
                ? "Instance validation failed."
                : response.diagnostics.back().message;

        response.steps.back().success = false;
        response.steps.back().details = response.summary;

        return response;
    }

    response.steps.back().success = true;
    response.steps.back().details = "All instance-level checks passed.";

    // ------------------------------------------------
    // Build parsed summary for UI
    // ------------------------------------------------

    ParsedSummary summary;

    summary.nodes = instance.nodes;
    summary.streets = instance.streets;
    summary.vehicles = instance.vehicles;
    summary.depot = instance.depot;
    summary.timeLimit = instance.time_limit;
    summary.wastePenalty = instance.waste_penalty;

    int mandatory_count = 0;
    int optional_count = 0;
    int connector_count = 0;

    for (const auto& street : instance.street_list) {

        switch (street.category) {

            case StreetCategory::Mandatory:
                ++mandatory_count;
                break;

            case StreetCategory::Optional:
                ++optional_count;
                break;

            case StreetCategory::Connector:
                ++connector_count;
                break;
        }
    }

    summary.mandatoryStreetCount = mandatory_count;
    summary.optionalStreetCount = optional_count;
    summary.connectorStreetCount = connector_count;

    response.parsedSummary = summary;

    // ------------------------------------------------
    // Step 3: Parse submission
    // ------------------------------------------------

    response.steps.push_back({
        "Parse submission",
        false,
        ""
    });

    SubmissionParser submission_parser;

    auto submission_result =
        submission_parser.parse(submission_text);

    if (!submission_result.success) {

        response.status = ValidatorStatus::ErrorSubmissionFormat;
        response.summary = submission_result.error_message;

        response.steps.back().success = false;
        response.steps.back().details = submission_result.error_message;

        response.diagnostics.push_back({
            DiagnosticSeverity::Error,
            "SUB_PARSE_FAILED",
            submission_result.error_message,
            ""
        });

        return response;
    }

    response.steps.back().success = true;
    response.steps.back().details = "Submission parsed successfully.";

    const Submission& submission = submission_result.submission;

    // ------------------------------------------------
    // Step 4: Validate submission format
    // ------------------------------------------------

    response.steps.push_back({
        "Validate submission format",
        false,
        ""
    });

    SubmissionFormatValidator format_validator;

    bool format_ok =
        format_validator.validate(instance, submission, response);

    if (!format_ok) {

        response.status = ValidatorStatus::ErrorSubmissionFormat;

        response.summary =
            response.diagnostics.empty()
                ? "Submission format validation failed."
                : response.diagnostics.back().message;

        response.steps.back().success = false;
        response.steps.back().details = response.summary;

        return response;
    }

    response.steps.back().success = true;
    response.steps.back().details = "Submission format is valid.";

    // ------------------------------------------------
    // Step 5: Validate submission semantics
    // ------------------------------------------------

    response.steps.push_back({
        "Validate submission semantics",
        false,
        ""
    });

    SubmissionSemanticValidator semantic_validator;

    bool semantic_ok =
        semantic_validator.validate(instance, submission, response);

    if (!semantic_ok) {

        response.status = ValidatorStatus::Invalid;

        response.summary =
            response.diagnostics.empty()
                ? "Submission semantic validation failed."
                : response.diagnostics.back().message;

        response.steps.back().success = false;
        response.steps.back().details = response.summary;

        return response;
    }

    response.steps.back().success = true;
    response.steps.back().details = "All semantic checks passed.";

    // ------------------------------------------------
    // Step 6: Compute score
    // ------------------------------------------------

    response.steps.push_back({
        "Compute score",
        false,
        ""
    });

    ScoreCalculator score_calculator;
    ScoreBreakdown breakdown =
        score_calculator.compute(instance, submission);

    response.scoreBreakdown = breakdown;
    response.score = breakdown.finalScore;

    response.steps.back().success = true;
    response.steps.back().details = "Score computed successfully.";

    // ------------------------------------------------
    // Final result
    // ------------------------------------------------

    response.status = ValidatorStatus::Valid;
    response.summary = "Submission is valid.";

    response.diagnostics.push_back({
        DiagnosticSeverity::Info,
        "VALIDATION_OK",
        "Submission passed semantic validation and scoring.",
        ""
    });

    return response;
}

} // namespace validator