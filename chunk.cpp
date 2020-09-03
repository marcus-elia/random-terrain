#include "chunk.h"

Chunk::Chunk()
{
    topLeft = {0, 0};
    sideLength = 512;
    groundColor = {1,1,1,1};
    initializeCenter();
    initializeChunkID();
}
Chunk::Chunk(Point2D inputTopLeft, int inputSideLength, RGBAcolor inputGroundColor)
{
    topLeft = inputTopLeft;
    sideLength = inputSideLength;
    groundColor = inputGroundColor;
    initializeCenter();
    initializeChunkID();
}

void Chunk::initializeCenter()
{
    center = {sideLength*topLeft.x + sideLength/2.0, 0,sideLength*topLeft.z + sideLength/2.0};
}
void Chunk::initializeChunkID()
{
    chunkID = point2DtoChunkID(topLeft);
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

void Chunk::draw() const
{
    setGLColor(groundColor);
    glBegin(GL_QUADS);
    glVertex3f(center.x - sideLength/2, 0, center.z - sideLength/2);
    glVertex3f(center.x - sideLength/2, 0, center.z + sideLength/2);
    glVertex3f(center.x + sideLength/2, 0, center.z + sideLength/2);
    glVertex3f(center.x + sideLength/2, 0, center.z - sideLength/2);
    glEnd();
}