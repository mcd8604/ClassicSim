#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>

class Random {
public:
    Random(const int min_range, const int max_range) {
        gen = new std::mt19937(std::time(0));
        distr = new std::uniform_int_distribution<>(min_range, max_range);
    }

    ~Random() {
        delete gen;
        delete distr;
    }

    int get_roll(void) {
        return (*distr)((*gen));
    }
protected:
private:
    std::mt19937* gen;
    std::uniform_int_distribution<>* distr;
};

#endif // RANDOM_H