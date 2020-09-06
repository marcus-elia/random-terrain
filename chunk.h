#ifndef RANDOM_TERRAIN_CHUNK_H
#define RANDOM_TERRAIN_CHUNK_H

#include <experimental/optional>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include "graphics.h"
#include "structs.h"
#include "mathHelper.h"
#include "building.h"
#include "randomNumberGenerator.h"

enum TerrainType {Snow, Grass, Rock, Sand, Water};

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

    double snowLimit;  // draw snow above here
    double rockLimit;  // draw rock above here
    double grassLimit; // draw grass above here, sand below
    double waterLevel;
    std::vector<std::vector<TerrainType>> squareTerrainType;
    std::unordered_map<TerrainType, RGBAcolor> terrainToColor;
    std::vector<std::vector<RGBAcolor>> squareColors;
    std::vector<std::vector<bool>> drawWaterAt;

    std::vector<std::shared_ptr<Building>> buildings;
    Point cityCenter; // where the game tries to put buildings within this chunk

public:
    Chunk();
    Chunk(Point2D inputTopLeft, int inputSideLength, int inputPointsPerSide,
          std::vector<std::vector<double>> terrainHeights, double inputHeightScaleFactor, double inputPerlinSeed,
          const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
          const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight,
          double inputSnowLimit, double inputRockLimit, double inputGrassLimit, double inputWaterLevel,
          RGBAcolor inputSnowColor, RGBAcolor inputRockColor, RGBAcolor inputGrassColor, RGBAcolor inputSandColor,
          RGBAcolor inputWaterColor, bool hasCity);

    void initializeCenter();
    void initializeChunkID();
    void initializeTerrainPoints(std::vector<std::vector<double>> terrainHeights);
    void initializeNormalVectors();
    void overwriteBorderHeights(const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
                                const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight);
    void initializeTerrainColorMap(RGBAcolor snowColor, RGBAcolor rockColor, RGBAcolor grassColor, RGBAcolor sandColor, RGBAcolor waterColor);
    void initializeSquareTerrainType();
    void initializeSquareColors();
    void initializeDrawWaterAt();
    void initializeRandomCityCenter();
    void initializeBuildings();

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

    // Check the 4 corners for the lowest height
    double getMinSquareHeight(int i, int j) const;

    // Use the perlin seed and the height scale factor to convert a double
    // between 0 and 1 (the relative height) to the actual height
    double relativeToAbsoluteHeight(double y) const;
    double absoluteToRelativeHeight(double y) const;

    RGBAcolor chooseColor(double y) const;
    void draw() const;
    void drawWater() const;
};

#endif //RANDOM_TERRAIN_CHUNK_H
