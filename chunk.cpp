#include "chunk.h"

Chunk::Chunk()
{
    topLeft = {0, 0};
    sideLength = 512;
    groundColor = {1,1,1,1};
    initializeCenter();
    initializeChunkID();
}
Chunk::Chunk(Point2D inputTopLeft, int inputSideLength, int inputPointsPerSide, RGBAcolor inputGroundColor,
        std::vector<std::vector<double>> terrainHeights, double inputHeightScaleFactor, double inputPerlinSeed,
             const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
             const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    pointsPerSide = inputPointsPerSide;
    heightScaleFactor = inputHeightScaleFactor;
    groundColor = inputGroundColor;
    perlinSeed = inputPerlinSeed > 0 ? inputPerlinSeed : 0.1; // can't be zero, gets divided by
    initializeCenter();
    initializeChunkID();
    initializeTerrainPoints(terrainHeights);
    overwriteBorderHeights(absoluteHeightsAbove, absoluteHeightsBelow, absoluteHeightsLeft, absoluteHeightsRight);
    initializeNormalVectors();
}

void Chunk::initializeCenter()
{
    center = {sideLength*topLeft.x + sideLength/2.0, 0,sideLength*topLeft.z + sideLength/2.0};
}
void Chunk::initializeChunkID()
{
    chunkID = point2DtoChunkID(topLeft);
}
void Chunk::initializeTerrainPoints(std::vector<std::vector<double>> terrainHeights)
{
    double squareSize = sideLength / (pointsPerSide-1.0);
    for(int i = 0; i < pointsPerSide; i++)
    {
        terrainPoints.emplace_back(std::vector<Point>());
        for(int j = 0; j < pointsPerSide; j++)
        {
            // A perlin seed of 0.5 will make the max height in this chunk be heightScaleFactor
            double x = center.x - sideLength/2 + i*squareSize;
            double y = relativeToAbsoluteHeight(terrainHeights[i][j]);
            double z = center.z - sideLength/2 + j*squareSize;
            terrainPoints[i].push_back({x, y, z});
        }
    }
}
void Chunk::initializeNormalVectors()
{
    for(int i = 0; i < pointsPerSide - 1; i++)
    {
        upperNormals.emplace_back(std::vector<Point>());
        lowerNormals.emplace_back(std::vector<Point>());
        for(int j = 0; j < pointsPerSide - 1; j++)
        {
            Point p1 = terrainPoints[i][j];
            Point p2 = terrainPoints[i+1][j];
            Point p3 = terrainPoints[i][j+1];
            Point p4 = terrainPoints[i+1][j+1];
            Point v1 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
            Point v2 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
            upperNormals[i].push_back(crossProduct(v1, v2));
            Point v3 = {p2.x - p4.x, p2.y - p4.y, p2.z - p4.z};
            Point v4 = {p3.x - p4.x, p3.y - p4.y, p3.z - p4.z};
            lowerNormals[i].push_back(crossProduct(v3, v4));
        }
    }
}
void Chunk::overwriteBorderHeights(const std::vector<double> &absoluteHeightsAbove, const std::vector<double> &absoluteHeightsBelow,
                            const std::vector<double> &absoluteHeightsLeft, const std::vector<double> &absoluteHeightsRight)
{
    if(absoluteHeightsAbove.size() == pointsPerSide)
    {
        for(int i = 0; i < pointsPerSide; i++)
        {
            terrainPoints[i][0].y = absoluteHeightsAbove[i];
        }
    }
    if(absoluteHeightsBelow.size() == pointsPerSide)
    {
        for(int i = 0; i < pointsPerSide; i++)
        {
            terrainPoints[i][pointsPerSide-1].y = absoluteHeightsBelow[i];
        }
    }
    if(absoluteHeightsLeft.size() == pointsPerSide)
    {
        for(int j = 0; j < pointsPerSide; j++)
        {
            terrainPoints[0][j].y = absoluteHeightsLeft[j];
        }
    }
    if(absoluteHeightsRight.size() == pointsPerSide)
    {
        for(int j = 0; j < pointsPerSide; j++)
        {
            terrainPoints[pointsPerSide-1][j].y = absoluteHeightsRight[j];
        }
    }
}






// Getters
Point2D Chunk::getTopLeft() const
{
    return topLeft;
}
int Chunk::getSideLength() const
{
    return sideLength;
}
Point Chunk::getCenter() const
{
    return center;
}
int Chunk::getChunkID()
{
    return chunkID;
}
double Chunk::getPerlinSeed() const
{
    return perlinSeed;
}
std::vector<double> Chunk::getTopTerrainHeights(bool isRelative) const
{
    std::vector<double> top;
    for(int i = 0; i < pointsPerSide; i++)
    {
        if(isRelative)
        {
            top.push_back(absoluteToRelativeHeight(terrainPoints[i][0].y));
        }
        else
        {
            top.push_back(terrainPoints[i][0].y);
        }
    }
    return top;
}
std::vector<double> Chunk::getBottomTerrainHeights(bool isRelative) const
{
    std::vector<double> bottom;
    for(int i = 0; i < pointsPerSide; i++)
    {
        if(isRelative)
        {
            bottom.push_back(absoluteToRelativeHeight(terrainPoints[i][pointsPerSide-1].y));
        }
        else
        {
            bottom.push_back(terrainPoints[i][pointsPerSide-1].y);
        }
    }
    return bottom;
}
std::vector<double> Chunk::getLeftTerrainHeights(bool isRelative) const
{
    std::vector<double> left;
    for(int j = 0; j < pointsPerSide; j++)
    {
        if(isRelative)
        {
            left.push_back(absoluteToRelativeHeight(terrainPoints[0][j].y));
        }
        else
        {
            left.push_back(terrainPoints[0][j].y);
        }
    }
    return left;
}
std::vector<double> Chunk::getRightTerrainHeights(bool isRelative) const
{
    std::vector<double> right;
    for(int j = 0; j < pointsPerSide; j++)
    {
        if(isRelative)
        {
            right.push_back(absoluteToRelativeHeight(terrainPoints[pointsPerSide-1][j].y));
        }
        else
        {
            right.push_back(terrainPoints[pointsPerSide-1][j].y);
        }
    }
    return right;
}


double Chunk::getHeightAt(Point p) const
{
    double squareSize = sideLength / (pointsPerSide-1.0);
    int topLeftI = (p.x - topLeft.x*sideLength) / squareSize;
    int topLeftJ = (p.z - topLeft.z*sideLength) / squareSize;
    Point squareTopLeft = terrainPoints[topLeftI][topLeftJ];
    Point squareBottomRight = terrainPoints[topLeftI+1][topLeftJ+1];
    if(distance2d(squareTopLeft, p) < distance2d(squareBottomRight, p))
    {
        Point normal = upperNormals[topLeftI][topLeftJ];
        double b = normal.y;
        if(b != 0)
        {
            return (dotProduct(normal, squareTopLeft) - normal.x*p.x - normal.z*p.z) / b;
        }
        return squareTopLeft.y;
    }
    else
    {
        Point normal = lowerNormals[topLeftI][topLeftJ];
        double b = normal.y;
        if(b != 0)
        {
            return (dotProduct(normal, squareBottomRight) - normal.x*p.x - normal.z*p.z) / b;
        }
        return squareBottomRight.y;
    }
}
double Chunk::relativeToAbsoluteHeight(double y) const
{
    return perlinSeed*heightScaleFactor*(y + 1);
}
double Chunk::absoluteToRelativeHeight(double y) const
{
    return y / (perlinSeed*heightScaleFactor) - 1;
}




void Chunk::draw() const
{
    glDisable(GL_CULL_FACE);
    double r,g,b,a;
    a = 1.0;
    for(int j = 0; j < pointsPerSide - 1; j++)
    {
        r = groundColor.r*terrainPoints[0][j].y/(2*heightScaleFactor);
        g = groundColor.g*terrainPoints[0][j].y/(2*heightScaleFactor);
        b = groundColor.b*terrainPoints[0][j].y/(2*heightScaleFactor);
        glColor4f(r,g,b,a);
        glBegin(GL_TRIANGLE_STRIP);
        drawPoint(terrainPoints[0][j]);
        drawPoint(terrainPoints[0][j+1]);
        for(int i = 1; i < pointsPerSide; i++)
        {
            r = groundColor.r*terrainPoints[i][j].y/(2*heightScaleFactor);
            g = groundColor.g*terrainPoints[i][j].y/(2*heightScaleFactor);
            b = groundColor.b*terrainPoints[i][j].y/(2*heightScaleFactor);
            glColor4f(r,g,b,a);
            drawPoint(terrainPoints[i][j]);
            drawPoint(terrainPoints[i][j+1]);
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
}