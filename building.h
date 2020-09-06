#ifndef RANDOM_TERRAIN_BUILDING_H
#define RANDOM_TERRAIN_BUILDING_H

#include "recPrism.h"
#include <vector>
#include <memory>

enum typeOfBuilding {Plain};

class Building
{
protected:
    std::vector<std::shared_ptr<Solid>> solids;

    // The rectangular base of the building's property
    Point center;
    int sideLength;

    int height;
    RGBAcolor color;
    RGBAcolor edgeColor;

    typeOfBuilding buildingType;

    const static bool GRIDLINES_ENABLED = false;

public:
    Building();

    Building(Point inputCenter, int inputSideLength, int inputHeight,
             RGBAcolor inputColor, RGBAcolor inputEdgeColor, typeOfBuilding inputBuildingType);

    void initializeSolids();

    // Getters
    std::vector<std::shared_ptr<Solid>> getSolids() const;

    typeOfBuilding getBuildingType() const;

    void draw() const;

    // Check each solid for a collision with this point (or if the point is within
    // buffer of the solid). If it finds one, it returns that solid's corrected point,
    // and doesn't check any more. If none of the solids have a problem, it returns nullopt.
    std::experimental::optional<Point> correctCollision(Point p, int buffer);
};

#endif //RANDOM_TERRAIN_BUILDING_H
