#ifndef RANDOM_TERRAIN_CHUNK_H
#define RANDOM_TERRAIN_CHUNK_H

#include <experimental/optional>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "structs.h"
#include "mathHelper.h"

class Chunk
{
private:
    // The top left coordinate divided by sideLength
    // So if sideLength = 512, then (3,2) describes the
    // chunk whose top left corner is (1536, 1024)
    Point2D topLeft;
    int sideLength;

    int pointsPerSide;
    double heightScaleFactor;  // The average height of terrain in the world
    double perlinSeed;         // The average height of this chunk
    std::vector<std::vector<Point>> terrainPoints;
    // Store the normal vector of the plane containing each triangle
    std::vector<std::vector<Point>> upperNormals;
    std::vector<std::vector<Point>> lowerNormals;


    Point center;   // The actual center (y-coordinate = 0)

    // The number of the chunk based on its location
    int chunkID;

    RGBAcolor groundColor;

public:
    Chunk();
    Chunk(Point2D inputTopLeft, int inputSideLength, int inputPointsPerSide, RGBAcolor inputGroundColor,
          std::vector<std::vector<double>> terrainHeights, double inputHeightScaleFactor, double inputPerlinSeed,
          const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
          const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight);

    void initializeCenter();
    void initializeChunkID();
    void initializeTerrainPoints(std::vector<std::vector<double>> terrainHeights);
    void initializeNormalVectors();
    void overwriteBorderHeights(const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
                                const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight);

    // Getters
    Point2D getTopLeft() const;
    int getSideLength() const;
    Point getCenter() const;
    int getChunkID();
    double getPerlinSeed() const;
    std::vector<double> getTopTerrainHeights(bool isRelative) const;
    std::vector<double> getBottomTerrainHeights(bool isRelative) const;
    std::vector<double> getLeftTerrainHeights(bool isRelative) const;
    std::vector<double> getRightTerrainHeights(bool isRelative) const;

    // Returns the height of the terrain at the given point,
    // assuming that the point is in this chunk
    double getHeightAt(Point p) const;

    // Use the perlin seed and the height scale factor to convert a double
    // between 0 and 1 (the relative height) to the actual height
    double relativeToAbsoluteHeight(double y) const;
    double absoluteToRelativeHeight(double y) const;

    void draw() const;
};

#endif //RANDOM_TERRAIN_CHUNK_H
