#ifndef RANDOM_TERRAIN_PLAYER_H
#define RANDOM_TERRAIN_PLAYER_H

#include "structs.h"
#include "mathHelper.h"

class Player
{
private:
    Point location;
    Point lookingAt;
    Point up;
    // The direction the player is facing in spherical coordinates
    // Must be on the unit sphere
    Point sphericalDirection;

    double xzAngle; // Where the player is looking in the xz-plane
    double yAngle;  // How much the player is looking up (in [-Pi/2, Pi/2])

    double speed;     // how fast the player can move
    Point velocity;   // current x y and z velocity

    double jumpAmount;
    double gravity;
    bool isGrounded;

    double sensitivity; // turning speed for mouse movement

    // Treat the Player as a Cylinder
    double height;
    double radius;

    // Keep the Player restricted to a square around (0,0)
    int maxDistanceFromSpawn;

    // Can't look perfectly vertically or it will glitch
    double VERTICAL_LIMIT = 0.01;
public:
    Player();
    Player(Point inputLocation, Point inputLookingAt, Point inputUp,
           double inputSpeed, double inputSensitivity, double inputHeight, double inputRadius,
           int inputMaxDistanceFromSpawn, double inputGravity, double inputJumpAmount);

    void initializeAngles();
    void initializeSphericalDirection();

    // Getters
    Point getLocation() const;
    Point getLookingAt() const;
    Point getVelocity() const;
    Point getUp() const;
    double getSpeed() const;
    double getXZAngle() const;
    double getYAngle() const;
    double getHeight() const;
    double getRadius() const;

    // Setters
    void setLocation(Point inputLocation);
    void setLookingAt(Point inputLookingAt);
    void setUp(Point inputUp);
    void setSpeed(double inputSpeed);
    void setSensitivity(double inputSensitivity);
    void setXZAngle(double inputXZAngle);
    void setYAngle(double inputYAngle);

    // Movement

    void move();

    // Keep the Player from going through the ground
    void correctGround();

    void applyGravity();

    // Don't allow the Player to move past maxDistanceFromSpawn
    void stayWithinBoundary();

    // Based on which keys are pressed, set the velocity
    void setVelocity(bool wKey, bool aKey, bool sKey, bool dKey);

    // Update the xzAngle and yAngle based on theta resulting from a mouse movement
    void updateAngles(double theta);

    // Use xzAngle, yAngle, and location to determine the spherical direction.
    void updateSphericalDirectionBasedOnAngles();

    void tryToJump();

    void tick();
};

#endif //RANDOM_TERRAIN_PLAYER_H
