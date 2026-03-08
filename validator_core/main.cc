#include <iostream>
#include <fstream>
#include <sstream>

#include "engine/validator_engine.hh"
#include "serialization/json_serialization.hh"

using namespace validator;

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cout << "Usage: validator instance.txt submission.txt\n";
        return 1;
    }

    std::ifstream f1(argv[1]);
    std::ifstream f2(argv[2]);

    std::stringstream s1, s2;

    s1 << f1.rdbuf();
    s2 << f2.rdbuf();

    ValidatorEngine engine;

    auto result = engine.evaluate(s1.str(), s2.str());

    std::cout << JsonSerializer::serialize(result) << std::endl;

    return 0;
}