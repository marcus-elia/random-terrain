#include "player.h"

Player::Player()
{
    location = {0, 10, 0};
    lookingAt = {0, 10, -10};
    up = {0, 20, 0};
    speed = 2;
    velocity = {0, 0, 0};
    sensitivity = 0.1;
    height = 20;
    radius = 5;
    initializeAngles();
    initializeSphericalDirection();
    maxDistanceFromSpawn = 5120;
    isGrounded = true;
    gravity = -0.1;
    jumpAmount = 10;
}
Player::Player(Point inputLocation, Point inputLookingAt, Point inputUp,
               double inputSpeed, double inputSensitivity, double inputHeight, double inputRadius, int inputMaxDistanceFromSpawn,
               double inputGravity, double inputJumpAmount)
{
    location = inputLocation;
    lookingAt = inputLookingAt;
    up = inputUp;
    speed = inputSpeed;
    velocity = {0, 0, 0};
    sensitivity = inputSensitivity;
    height = inputHeight;
    radius = inputRadius;
    initializeAngles();
    initializeSphericalDirection();
    maxDistanceFromSpawn = inputMaxDistanceFromSpawn;
    isGrounded = true;
    gravity = inputGravity;
    jumpAmount = inputJumpAmount;
}

void Player::initializeAngles()
{
    xzAngle = atan2(lookingAt.z - location.z, lookingAt.x - location.x);
    yAngle = atan2(lookingAt.y - location.y, distance2d(location, lookingAt));
}
void Player::initializeSphericalDirection()
{
    sphericalDirection.x = cos(xzAngle);
    sphericalDirection.z = sin(xzAngle);

    // Must scale the component in the xz plane for spherical coordinates
    double scaleAmount = cos(yAngle) / sqrt(sphericalDirection.x*sphericalDirection.x + sphericalDirection.z*sphericalDirection.z);
    sphericalDirection.x *= scaleAmount;
    sphericalDirection.z *= scaleAmount;
    sphericalDirection.y = sin(yAngle);
}

// ==========================
//
//          Getters
//
// ==========================
Point Player::getLocation() const
{
    return location;
}
Point Player::getLookingAt() const
{
    return lookingAt;
}
Point Player::getUp() const
{
    return up;
}
Point Player::getVelocity() const
{
    return velocity;
}
double Player::getSpeed() const
{
    return speed;
}
double Player::getXZAngle() const
{
    return xzAngle;
}
double Player::getYAngle() const
{
    return yAngle;
}
double Player::getHeight() const
{
    return height;
}
double Player::getRadius() const
{
    return radius;
}

// ==========================
//
//          Setters
//
// ==========================
void Player::setLocation(Point inputLocation)
{
    location = inputLocation;
}
void Player::setLookingAt(Point inputLookingAt)
{
    lookingAt = inputLookingAt;
}
void Player::setUp(Point inputUp)
{
    up = inputUp;
}
void Player::setSpeed(double inputSpeed)
{
    speed = inputSpeed;
}
void Player::setSensitivity(double inputSensitivity)
{
    sensitivity = inputSensitivity;
}
void Player::setXZAngle(double inputXZAngle)
{
    xzAngle = inputXZAngle;
}
void Player::setYAngle(double inputYAngle)
{
    yAngle = inputYAngle;
}

// ===============================
//
//           Movement
//
// ==============================

void Player::move()
{
    location.x += velocity.x;
    lookingAt.x += velocity.x;
    location.y += velocity.y;
    lookingAt.y += velocity.y;
    location.z += velocity.z;
    lookingAt.z += velocity.z;
}

void Player::correctGround()
{
    if(location.y < height/2)
    {
        lookingAt.y += height/2 - location.y;
        location.y = height/2;
        isGrounded = true;
        velocity.y = 0;
    }
}

void Player::applyGravity()
{
    if(!isGrounded)
    {
        velocity.y += gravity;
    }
}

void Player::stayWithinBoundary()
{
    if(location.x > maxDistanceFromSpawn)
    {
        lookingAt.x -= maxDistanceFromSpawn - location.x;
        location.x = maxDistanceFromSpawn;
    }
    else if(location.x < -maxDistanceFromSpawn)
    {
        lookingAt.x += maxDistanceFromSpawn + location.x;
        location.x = -maxDistanceFromSpawn;
    }
    if(location.z > maxDistanceFromSpawn)
    {
        lookingAt.z -= maxDistanceFromSpawn - location.z;
        location.z = maxDistanceFromSpawn;
    }
    else if(location.z < -maxDistanceFromSpawn)
    {
        lookingAt.z += maxDistanceFromSpawn + location.z;
        location.z = -maxDistanceFromSpawn;
    }
}


// Based on which keys are pressed, set the velocity
void Player::setVelocity(bool wKey, bool aKey, bool sKey, bool dKey)
{
    bool forward = wKey && !sKey;
    bool backward = !wKey && sKey;
    bool left = aKey && !dKey;
    bool right = !aKey && dKey;

    // The angle the player should move based on input
    double angleToMove;
    if(forward)
    {
        if(right)
        {
            angleToMove = xzAngle + PI/4;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI/4;
        }
        else
        {
            angleToMove = xzAngle;
        }
    }
    else if(backward)
    {
        if(right)
        {
            angleToMove = xzAngle - PI - PI/4;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI + PI/4;
        }
        else
        {
            angleToMove = xzAngle - PI;
        }
    }
    else
    {
        if(right)
        {
            angleToMove = xzAngle + PI/2;
        }
        else if(left)
        {
            angleToMove = xzAngle - PI/2;
        }
        else // If no arrow keys are being held
        {
            velocity.x = 0;
            velocity.z = 0;
            return;
        }
    }
    velocity.x = speed * cos(angleToMove);
    velocity.z = speed * sin(angleToMove);
}

// Update the xzAngle and yAngle based on theta resulting from a mouse movement
void Player::updateAngles(double theta)
{
    double horizontalAmount = sensitivity * cos(theta);
    xzAngle += horizontalAmount;
    if(xzAngle > 2*PI)
    {
        xzAngle -= 2*PI;
    }
    else if(xzAngle < 0)
    {
        xzAngle += 2*PI;
    }

    double verticalAmount = sensitivity * sin(theta);
    yAngle -= verticalAmount; // negative sign since Glut's mouse functions treat +y as down
    if(yAngle > PI/2 - VERTICAL_LIMIT)
    {
        yAngle = PI/2 - VERTICAL_LIMIT;
    }
    else if(yAngle < -PI/2 + VERTICAL_LIMIT)
    {
        yAngle = -PI/2 + VERTICAL_LIMIT;
    }
}

// Use xzAngle, yAngle, and location to determine the spherical direction.
void Player::updateSphericalDirectionBasedOnAngles()
{
    sphericalDirection.x = cos(xzAngle);
    sphericalDirection.z = sin(xzAngle);

    // Must scale the component in the xz plane for spherical coordinates
    double scaleAmount = cos(yAngle) / sqrt(sphericalDirection.x*sphericalDirection.x + sphericalDirection.z*sphericalDirection.z);
    sphericalDirection.x *= scaleAmount;
    sphericalDirection.z *= scaleAmount;
    sphericalDirection.y = sin(yAngle);

    lookingAt.x = location.x + sphericalDirection.x;
    lookingAt.y = location.y + sphericalDirection.y;
    lookingAt.z = location.z + sphericalDirection.z;
}

void Player::tryToJump()
{
    if(isGrounded)
    {
        velocity.y = jumpAmount;
        isGrounded = false;
    }
}

void Player::tick()
{
    move();

    correctGround();

    applyGravity();

    stayWithinBoundary();
}
