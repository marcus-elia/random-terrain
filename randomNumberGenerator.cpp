#include "randomNumberGenerator.h"

int RandomNumberGenerator::currentValue = time(NULL);
RandomNumberGenerator::RandomNumberGenerator()
{

}

// Returns a random number between 0 and 1 and updates current value
double RandomNumberGenerator::getRandom()
{
    double result = static_cast<double>(currentValue) / modulus;
    currentValue = (RandomNumberGenerator::currentValue * multiplier + additive) % modulus;
    return result;
}