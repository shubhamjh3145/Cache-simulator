#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <list>
#include <cmath>
#include <cstdint>
#include <iostream>

class Cache {
private:
    int cacheSize;
    int blockSize;
    int associativity;
    int numSets;
    int offsetBits;
    int indexBits;

    std::vector<std::list<uint64_t>> sets;

    long long totalAccesses;
    long long hitCount;
    long long missCount;

    int log2i(int n);

public:
    Cache(int cSize, int bSize, int assoc);
    void access(uint64_t address);
    void printStats() const;
    void resetStats();
};

#endif