#include "building.h"

Building::Building()
{
    center = {0, 100, 0};
    sideLength = 32;

    height = 200;
    color = {0,0.4,1,1};
    edgeColor = {.9,.9,.9, 1};

    buildingType = PlainRectangle;

    initializeSolids();
}

Building::Building(Point inputCenter, int inputSideLength, int inputHeight,
        RGBAcolor inputColor, RGBAcolor inputEdgeColor, typeOfBuilding inputBuildingType)
{
    center = inputCenter;
    sideLength = inputSideLength;
    height = inputHeight;
    color = inputColor;
    edgeColor = inputEdgeColor;
    buildingType = inputBuildingType;

    initializeSolids();
}

void Building::initializeSolids()
{
    Point center = {center.x, (double)height/2, center.z};
    solids.push_back(std::make_shared<RecPrism>(RecPrism(center, color,
                                                         sideLength, height, sideLength, edgeColor)));
}

// Getters
std::vector<std::shared_ptr<Solid>> Building::getSolids() const
{
    return solids;
}

typeOfBuilding Building::getBuildingType() const
{
    return buildingType;
}

void Building::draw() const
{
    for(std::shared_ptr<Solid> s : solids)
    {
        s->draw();
    }
}

// Check each solid for a collision with this point (or if the point is within
// buffer of the solid). If it finds one, it returns that solid's corrected point,
// and doesn't check any more. If none of the solids have a problem, it returns nullopt.
std::experimental::optional<Point> Building::correctCollision(Point p, int buffer)
{
    return std::experimental::nullopt;
}