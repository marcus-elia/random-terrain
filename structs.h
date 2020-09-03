#ifndef RANDOM_TERRAIN_STRUCTS_H
#define RANDOM_TERRAIN_STRUCTS_H

// This file contains some structs and enums to make
// including simpler


// int points used for the chunks
// in the xz plane
struct Point2D
{
    int x;
    int z;
    bool operator ==(const Point2D& p1)
    {
        return x == p1.x && z == p1.z;
    }
    bool operator !=(const Point2D& p1)
    {
        return x != p1.x || z != p1.z;
    }
};


struct Point
{
    double x;
    double y;
    double z;
};

struct RGBAcolor
{
    double r;
    double g;
    double b;
    double a;
};

#endif //RANDOM_TERRAIN_STRUCTS_H
