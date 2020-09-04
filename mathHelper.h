#ifndef RANDOM_TERRAIN_MATHHELPER_H
#define RANDOM_TERRAIN_MATHHELPER_H

#include "structs.h"
#include <cmath>
#include <vector>

// This file contains general math helper functions

const double PI = 3.14159265358979323846;


// Assuming n > 0, this returns the integer closest to n that
// is a perfect square
int nearestPerfectSquare(int n);

// Returns the square root of n, assuming that n is a perfect square
int isqrt(int n);

// Need mod since % can return negatives. No.
int mod(int a, int m);

// Convert between chunkID's and Point2D's
Point2D chunkIDtoPoint2D(int n);
int point2DtoChunkID(Point2D p);

// Get the chunkIDs adjacent to the input
int getChunkIDAbove(int chunkID);
int getChunkIDBelow(int chunkID);
int getChunkIDLeft(int chunkID);
int getChunkIDRight(int chunkID);

// Euclidean distance
double distanceFormula(double x1, double y1, double x2, double y2);

// Returns the distance in the xz plane, ignoring the y-coordinate
// (calls distanceFormula())
double distance2d(Point p1, Point p2);

// Returns the ints corresponding to to all chunks that are within radius of this one,
// using the taxicab metric
std::vector<int> getChunkIDsAroundPoint(Point2D p, int radius);
std::vector<Point2D> getChunkTopLeftCornersAroundPoint(Point2D p, int radius);
// Wrappers
std::vector<int> getChunkIDsAroundPoint(int chunkID, int radius);
std::vector<Point2D> getChunkTopLeftCornersAroundPoint(int chunkID, int radius);

int getChunkIDContainingPoint(Point p, int chunkSize);

// Vector functions
double dotProduct(Point p1, Point p2);
Point crossProduct(Point p1, Point p2);

#endif //RANDOM_TERRAIN_MATHHELPER_H
