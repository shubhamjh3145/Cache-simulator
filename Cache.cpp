#include "Cache.h"

int Cache::log2i(int n) {
    int res = 0;
    while (n >>= 1) res++;
    return res;
}

Cache::Cache(int cSize, int bSize, int assoc) {
    cacheSize = cSize;
    blockSize = bSize;
    associativity = assoc;
    
    int numBlocks = cacheSize / blockSize;
    if (associativity == 0) {
        numSets = 1;
        associativity = numBlocks;
    } else {
        numSets = numBlocks / associativity;
    }

    offsetBits = log2i(blockSize);
    indexBits = log2i(numSets);

    sets.resize(numSets);

    totalAccesses = 0;
    hitCount = 0;
    missCount = 0;
}

void Cache::access(uint64_t address) {
    totalAccesses++;
    uint64_t blockAddress = address >> offsetBits;
    uint64_t index = blockAddress & ((1ULL << indexBits) - 1);
    uint64_t tag = blockAddress >> indexBits;

    auto& currentSet = sets[index];

    bool hit = false;
    for (auto it = currentSet.begin(); it != currentSet.end(); ++it) {
        if (*it == tag) {
            hit = true;
            currentSet.erase(it);
            currentSet.push_back(tag);
            break;
        }
    }

    if (hit) {
        hitCount++;
    } else {
        missCount++;
        if (currentSet.size() == (size_t)associativity) {
            currentSet.pop_front();
        }
        currentSet.push_back(tag);
    }
}

void Cache::printStats() const {
    std::cout << "\n==============================\n";
    std::cout << "   CACHE SIMULATION REPORT    \n";
    std::cout << "==============================\n";
    std::cout << "Total Accesses : " << totalAccesses << "\n";
    std::cout << "Hits           : " << hitCount << "\n";
    std::cout << "Misses         : " << missCount << "\n";
    
    double hitRate = totalAccesses > 0 ? (double)hitCount / totalAccesses * 100.0 : 0.0;
    double missRate = totalAccesses > 0 ? (double)missCount / totalAccesses * 100.0 : 0.0;
    
    std::cout << "Hit Rate       : " << hitRate << "%\n";
    std::cout << "Miss Rate      : " << missRate << "%\n";
    std::cout << "==============================\n";
}

void Cache::resetStats() {
    totalAccesses = 0;
    hitCount = 0;
    missCount = 0;
    for (auto& s : sets) s.clear();
}