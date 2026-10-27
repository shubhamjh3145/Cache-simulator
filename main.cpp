#include <iostream>
#include <fstream>
#include <sstream>
#include "Cache.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./cache_sim <config_file> <access_file>\n";
        std::cout << "Example: ./cache_sim cache.config cache.access\n";
        return 1;
    }

    std::string configFile = argv[1];
    std::string accessFile = argv[2];

    std::ifstream cfgStream(configFile);
    if (!cfgStream.is_open()) {
        std::cerr << "Error: Could not open config file " << configFile << "\n";
        return 1;
    }

    int cacheSize, blockSize, associativity;
    cfgStream >> cacheSize >> blockSize >> associativity;
    cfgStream.close();

    Cache cache(cacheSize, blockSize, associativity);

    std::ifstream accStream(accessFile);
    if (!accStream.is_open()) {
        std::cerr << "Error: Could not open access file " << accessFile << "\n";
        return 1;
    }

    std::string line;
    while (std::getline(accStream, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        uint64_t address;
        ss >> std::hex >> address;
        cache.access(address);
    }

    accStream.close();
    cache.printStats();

    return 0;
}