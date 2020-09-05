#ifndef RANDOM_TERRAIN_RANDOMNUMBERGENERATOR_H
#define RANDOM_TERRAIN_RANDOMNUMBERGENERATOR_H

#include <time.h>

class RandomNumberGenerator
{
public:
    static int currentValue;
    int modulus = 65536;
    int additive = 6561;
    int multiplier = 17;

    RandomNumberGenerator();

    // Returns a random number between 0 and 1 and updates current value
    double getRandom();

};
#endif //RANDOM_TERRAIN_RANDOMNUMBERGENERATOR_H
