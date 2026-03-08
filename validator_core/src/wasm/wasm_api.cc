#include "wasm/wasm_api.hh"

#include <string>

#include "engine/validator_engine.hh"
#include "serialization/json_serialization.hh"

namespace {
    std::string g_last_result_json;
}

extern "C" const char* validate(const char* instance_text, const char* submission_text) {
    validator::ValidatorEngine engine;

    const std::string instance =
        (instance_text != nullptr) ? std::string(instance_text) : std::string();

    const std::string submission =
        (submission_text != nullptr) ? std::string(submission_text) : std::string();

    validator::ValidationResponse response = engine.evaluate(instance, submission);

    g_last_result_json = validator::JsonSerializer::serialize(response);

    return g_last_result_json.c_str();
}