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
        std::vector<std::vector<double>> terrainHeights, double inputHeightScaleFactor)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    pointsPerSide = inputPointsPerSide;
    heightScaleFactor = inputHeightScaleFactor;
    groundColor = inputGroundColor;
    initializeCenter();
    initializeChunkID();
    initializeTerrainPoints(terrainHeights);
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
    int squareSize = sideLength / (pointsPerSide-1);
    for(int i = 0; i < pointsPerSide; i++)
    {
        terrainPoints.emplace_back(std::vector<Point>());
        for(int j = 0; j < pointsPerSide; j++)
        {
            double x = center.x - sideLength/2 + i*squareSize;
            double y = terrainHeights[i][j]*heightScaleFactor;
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
std::vector<double> Chunk::getTopTerrainHeights() const
{
    std::vector<double> top;
    for(int i = 0; i < pointsPerSide; i++)
    {
        top.push_back(terrainPoints[i][0].y);
    }
    return top;
}
std::vector<double> Chunk::getBottomTerrainHeights() const
{
    std::vector<double> bottom;
    for(int i = 0; i < pointsPerSide; i++)
    {
        bottom.push_back(terrainPoints[i][pointsPerSide-1].y);
    }
    return bottom;
}
std::vector<double> Chunk::getLeftTerrainHeights() const
{
    std::vector<double> left;
    for(int j = 0; j < pointsPerSide; j++)
    {
        left.push_back(terrainPoints[0][j].y);
    }
    return left;
}
std::vector<double> Chunk::getRightTerrainHeights() const
{
    std::vector<double> right;
    for(int j = 0; j < pointsPerSide; j++)
    {
        right.push_back(terrainPoints[pointsPerSide-1][j].y);
    }
    return right;
}

double Chunk::getHeightAt(Point p) const
{
    int squareSize = sideLength / (pointsPerSide-1);
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
        Point normal = lowerNormals[topLeftI+1][topLeftJ+1];
        double b = normal.y;
        if(b != 0)
        {
            return (dotProduct(normal, squareBottomRight) - normal.x*p.x - normal.z*p.z) / b;
        }
        return squareBottomRight.y;
    }

}




void Chunk::draw() const
{
    glDisable(GL_CULL_FACE);
    setGLColor(groundColor);
    for(int j = 0; j < pointsPerSide - 1; j++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        drawPoint(terrainPoints[0][j]);
        drawPoint(terrainPoints[0][j+1]);
        for(int i = 1; i < pointsPerSide; i++)
        {
            drawPoint(terrainPoints[i][j]);
            drawPoint(terrainPoints[i][j+1]);
        }
        glEnd();
    }
    glEnable(GL_CULL_FACE);
}