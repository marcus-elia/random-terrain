#ifndef RANDOM_TERRAIN_PERLINNOISEGENERATOR_H
#define RANDOM_TERRAIN_PERLINNOISEGENERATOR_H

// This is based on code from https://www.youtube.com/watch?v=6-0UaeJBumA,
// a tutorial by javidx9
#include <vector>
#include <cmath>
#include "randomNumberGenerator.h"

class PerlinNoiseGenerator
{
private:
    int width;
    int height;

    std::vector<std::vector<double>> noiseSeed;
    std::vector<std::vector<double>> perlinNoise;

    double bias; // how sharp the noise is

public:
    PerlinNoiseGenerator();

    PerlinNoiseGenerator(int inputWidth, int inputHeight, double inputBias, std::vector<double> topInput={},
                         std::vector<double> bottomInput={}, std::vector<double> leftInput={}, std::vector<double> rightInput={});

    // This takes in preset values for the 4 sides, and fills the 2d array
    void fillNoiseSeed(std::vector<double> topInput={}, std::vector<double> bottomInput={},
            std::vector<double> leftInput={}, std::vector<double> rightInput={});

    // This overwrites the 4 borders (if values are specified)
    void setBorders(std::vector<double> topInput={}, std::vector<double> bottomInput={},
                    std::vector<double> leftInput={}, std::vector<double> rightInput={});

    std::vector<double> calculatePerlinNoise1D(int count, std::vector<double> seed, int numOctaves);

    std::vector<std::vector<double>> calculatePerlinNoise2D(int width, int height,
                                                            std::vector<std::vector<double>> seed, int numOctaves);

    // Linearly scales the perlin noise so that the smallest in the 2d array becomes minvalue
    // and the highest becomes maxvalue
    std::vector<std::vector<double>> getScaledNoise(double minValue, double maxValue) const;
    // Same thing, but with borders enforced
    std::vector<std::vector<double>> getScaledNoiseApplyBorders(double minValue, double maxValue,
                                    std::vector<double> topInput={}, std::vector<double> bottomInput={},
                                    std::vector<double> leftInput={}, std::vector<double> rightInput={}) const;

    std::vector<std::vector<double>> getPerlinNoise() const;
};
#endif //RANDOM_TERRAIN_PERLINNOISEGENERATOR_H
