#include "perlinNoiseGenerator.h"

PerlinNoiseGenerator::PerlinNoiseGenerator()
{
    width = 10;
    height = 10;
    bias = 1;
    fillNoiseSeed();
    perlinNoise = calculatePerlinNoise2D(width, height, noiseSeed,
                                         (int)floor(log(width)));
    scaleNoise(0,1);
}
PerlinNoiseGenerator::PerlinNoiseGenerator(int inputWidth, int inputHeight, double inputBias,
        std::vector<double> topInput, std::vector<double> bottomInput,
        std::vector<double> leftInput, std::vector<double> rightInput)
{
    width = inputWidth;
    height = inputHeight;
    bias = inputBias;
    if(bias < 0)
    {
        bias = 0.2;
    }
    fillNoiseSeed(topInput, bottomInput, leftInput, rightInput);
    perlinNoise = calculatePerlinNoise2D(width, height, noiseSeed,
                                         (int)floor(log(width)));
    scaleNoise(0,1);
    setBorders(topInput, bottomInput, leftInput, rightInput);
}

void PerlinNoiseGenerator::fillNoiseSeed(std::vector<double> topInput, std::vector<double> bottomInput,
                                         std::vector<double> leftInput, std::vector<double> rightInput)
{
    // Fill in the 2d array with random values
    srand(time(NULL));
    for(int i = 0; i < width; i++)
    {
        noiseSeed.emplace_back(std::vector<double>());
        for(int j = 0; j < height; j++)
            noiseSeed[i].push_back((double)(rand() % 100) / 100);
    }
    setBorders(topInput, bottomInput, leftInput, rightInput);
}

void PerlinNoiseGenerator::setBorders(std::vector<double> topInput, std::vector<double> bottomInput,
                                      std::vector<double> leftInput, std::vector<double> rightInput)
{
    // Replace the 4 sides with preset values, if applicable
    if(topInput.size() == width)
    {
        for(int i = 0; i < width; i++)
        {
            noiseSeed[i][0] = topInput[i];
        }
    }
    if(bottomInput.size() == width)
    {
        for(int i = 0; i < width; i++)
        {
            noiseSeed[i][height-1] = bottomInput[i];
        }
    }
    if(leftInput.size() == height)
    {
        for(int j = 0; j < height; j++)
        {
            noiseSeed[0][j] = leftInput[j];
        }
    }
    if(rightInput.size() == height)
    {
        for(int j = 0; j < height; j++)
        {
            noiseSeed[width-1][j] = rightInput[j];
        }
    }
}

std::vector<double> PerlinNoiseGenerator::calculatePerlinNoise1D(int count, std::vector<double> seed, int numOctaves)
{
    std::vector<double> output;
    for(int i = 0; i < count; i++)
    {
        double noise = 0;
        double scale = 1;
        int pitch = count;
        double scaleSum = 0.0;
        for(int oct = 0; oct < numOctaves; oct++)
        {
            int sample1 = (i / pitch) * pitch;
            int sample2 = (sample1 + pitch) % count;

            double blend = (i - sample1) / (double)pitch;
            double sample = (1 - blend) * seed[sample1] + blend * seed[sample2];
            noise += scale*sample;

            pitch /= 2;

            scaleSum += scale;
            scale = scale / 2;
        }
        output.push_back(noise / scaleSum);
    }
    return output;
}

std::vector<std::vector<double>> PerlinNoiseGenerator::calculatePerlinNoise2D(int w, int h,
                                                                              std::vector<std::vector<double>> seed, int numOctaves)
{
    std::vector<std::vector<double>> output;
    for(int i = 0; i < w; i++)
    {
        output.emplace_back(std::vector<double>());
        for(int j = 0; j < h; j++)
        {
            double noise = 0;
            double scale = 1;
            int pitch = w;
            double scaleSum = 0.0;
            for(int oct = 0; oct < numOctaves; oct++)
            {
                int sampleX1 = (i / pitch) * pitch;
                int sampleY1 = (j / pitch) * pitch;

                int sampleX2 = (sampleX1 + pitch) % w;
                int sampleY2 = (sampleY1 + pitch) % w;

                double blendX = (i - sampleX1) / (double)pitch;
                double blendY = (j - sampleY1) / (double)pitch;
                double sampleT = (1 - blendX) * seed[sampleY1][sampleX1] + blendX * seed[sampleY1][sampleX2];
                double sampleB = (1 - blendX) * seed[sampleY2][sampleX1] + blendX * seed[sampleY2][sampleX2];

                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;

                pitch /= 2;

                scaleSum += scale;
                scale = scale / bias;
            }
            output[i].push_back(noise / scaleSum);
        }
    }
    return output;
}

void PerlinNoiseGenerator::scaleNoise(double minValue, double maxValue)
{
    double a = 1;
    double b = 0;

    // Iterate through to find smallest and largest
    for(std::vector<double> &vec : perlinNoise)
    {
        for(double d : vec)
        {
            if(d < a)
            {
                a = d;
            }
            if(d > b)
            {
                b = d;
            }
        }
    }
    // In the rare case of division by zero, don't do anything
    if(a == b)
    {
        return;
    }

    // Iterate through again, and scale each value
    for(std::vector<double> &vec : perlinNoise)
    {
        for(int i = 0; i < vec.size(); i++)
        {
            // Scale between 0 and 1
            vec[i] = (vec[i] - a) / (b - a);
            // Scale between minvalue and maxvalue
            vec[i] = (maxValue - minValue)*vec[i] + minValue;
        }
    }
}

std::vector<std::vector<double>> PerlinNoiseGenerator::getPerlinNoise()
{
    return perlinNoise;
}
