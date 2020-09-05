#include "recPrism.h"

RecPrism::RecPrism() : Solid()
{
    initializeCorners();
    initializeLinePoints();
}
RecPrism::RecPrism(Point inputCenter, RGBAcolor inputColor,
                   double inputXWidth, double inputYWidth, double inputZWidth, RGBAcolor inputLineColor,
                   linesDrawnEnum inputLinesDrawn) :
        Solid(inputCenter, inputColor, inputXWidth, inputYWidth, inputZWidth, inputLineColor, inputLinesDrawn)
{
    initializeCorners();
    initializeLinePoints();
}

void RecPrism::initializeCorners()
{
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z + zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y + yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x + xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
    corners.push_back({center.x - xWidth/2, center.y - yWidth/2, center.z - zWidth/2});
}

void RecPrism::initializeLinePoints()
{
    initializeXLinePoints();
    initializeYLinePoints();
    initializeZLinePoints();
}

void RecPrism::initializeXLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(xWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(xWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(xWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The x lines (circling around the x-axis)
    double distanceBetweenPointsX = xWidth / numPoints;

    // Top front line
    x = corners[1].x;
    y = corners[1].y + lineOffset;
    z = corners[1].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom Front line
    x = corners[3].x;
    y = corners[3].y - lineOffset;
    z = corners[3].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Bottom back line
    x = corners[7].x;
    y = corners[7].y - lineOffset;
    z = corners[7].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }

    // Top Back line
    x = corners[5].x;
    y = corners[5].y + lineOffset;
    z = corners[5].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        x += distanceBetweenPointsX;
        xLinePoints.push_back({x, y, z});
    }
}

void RecPrism::initializeYLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(yWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(yWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(yWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The y lines (circling around the y-axis)
    double distanceBetweenPointsY = yWidth / numPoints;

    // Front left line
    x = corners[3].x - lineOffset;
    y = corners[3].y;
    z = corners[3].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Front Right line
    x = corners[2].x + lineOffset;
    y = corners[2].y;
    z = corners[2].z + lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Back right line
    x = corners[6].x + lineOffset;
    y = corners[6].y;
    z = corners[6].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }

    // Back Left line
    x = corners[7].x - lineOffset;
    y = corners[7].y;
    z = corners[7].z - lineOffset;
    for(int i = 0; i < numPoints - 1; i++)
    {
        y += distanceBetweenPointsY;
        yLinePoints.push_back({x, y, z});
    }
}

void RecPrism::initializeZLinePoints()
{
    int numPoints;
    double x,y,z;

    // Decide how far apart to make the lines
    if(linesDrawn == Low)
    {
        numPoints = floor(zWidth / distanceBetweenLowLines);
    }
    else if(linesDrawn == Medium)
    {
        numPoints = floor(zWidth / distanceBetweenMediumLines);
    }
    else if(linesDrawn == High)
    {
        numPoints = floor(zWidth / distanceBetweenHighLines);
    }
    else // If linesDrawn = Normal or NoLines, do not add any gridlines on the planes
    {
        return;
    }

    // The z lines (circling around the z-axis)
    double distanceBetweenPointsZ = zWidth / numPoints;

    // Top Right line
    x = corners[4].x + lineOffset;
    y = corners[4].y + lineOffset;
    z = corners[4].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom right line
    x = corners[6].x + lineOffset;
    y = corners[6].y - lineOffset;
    z = corners[6].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Bottom Left line
    x = corners[7].x - lineOffset;
    y = corners[7].y - lineOffset;
    z = corners[7].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }

    // Top Left line
    x = corners[5].x - lineOffset;
    y = corners[5].y + lineOffset;
    z = corners[5].z;
    for(int i = 0; i < numPoints - 1; i++)
    {
        z += distanceBetweenPointsZ;
        zLinePoints.push_back({x, y, z});
    }
}

void RecPrism::draw() const
{
    glDisable(GL_CULL_FACE);
    drawLines();
    drawFaces();
    glEnable(GL_CULL_FACE);
}

void RecPrism::drawLines() const
{
    if(linesDrawn == NoLines)
    {
        return;
    }

    setGLColor(lineColor);
    glBegin(GL_LINES);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[1]);
    drawPoint(corners[3]);

    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[2]);
    drawPoint(corners[0]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);

    drawPoint(corners[5]);
    drawPoint(corners[7]);

    drawPoint(corners[7]);
    drawPoint(corners[6]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);

    drawPoint(corners[0]);
    drawPoint(corners[4]);

    drawPoint(corners[2]);
    drawPoint(corners[6]);

    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[1]);
    drawPoint(corners[5]);

    glEnd();

    if(linesDrawn != Normal)
    {
        drawGridLines();
    }
}

void RecPrism::drawFaces() const
{
    glBegin(GL_QUADS);
    setGLColor(color);

    drawPoint(corners[0]);
    drawPoint(corners[1]);
    drawPoint(corners[3]);
    drawPoint(corners[2]);

    drawPoint(corners[5]);
    drawPoint(corners[4]);
    drawPoint(corners[6]);
    drawPoint(corners[7]);

    drawPoint(corners[6]);
    drawPoint(corners[4]);
    drawPoint(corners[0]);
    drawPoint(corners[2]);

    drawPoint(corners[4]);
    drawPoint(corners[5]);
    drawPoint(corners[1]);
    drawPoint(corners[0]);

    drawPoint(corners[6]);
    drawPoint(corners[2]);
    drawPoint(corners[3]);
    drawPoint(corners[7]);

    drawPoint(corners[3]);
    drawPoint(corners[1]);
    drawPoint(corners[5]);
    drawPoint(corners[7]);
    glEnd();
}

void RecPrism::drawGridLines() const
{
    setGLColor(lineColor);
    glBegin(GL_LINES);

    int pointsPerSide;

    // x lines
    pointsPerSide = xLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(xLinePoints[i]);
        drawPoint(xLinePoints[i + pointsPerSide]);
        drawPoint(xLinePoints[i + pointsPerSide]);
        drawPoint(xLinePoints[i + 2*pointsPerSide]);
        drawPoint(xLinePoints[i + 2*pointsPerSide]);
        drawPoint(xLinePoints[i + 3*pointsPerSide]);
        drawPoint(xLinePoints[i + 3*pointsPerSide]);
        drawPoint(xLinePoints[i]);
    }

    // y lines
    pointsPerSide = yLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(yLinePoints[i]);
        drawPoint(yLinePoints[i + pointsPerSide]);
        drawPoint(yLinePoints[i + pointsPerSide]);
        drawPoint(yLinePoints[i + 2*pointsPerSide]);
        drawPoint(yLinePoints[i + 2*pointsPerSide]);
        drawPoint(yLinePoints[i + 3*pointsPerSide]);
        drawPoint(yLinePoints[i + 3*pointsPerSide]);
        drawPoint(yLinePoints[i]);
    }

    // z lines
    pointsPerSide = zLinePoints.size()/4;
    for(int i = 0; i < pointsPerSide; i++)
    {
        drawPoint(zLinePoints[i]);
        drawPoint(zLinePoints[i + pointsPerSide]);
        drawPoint(zLinePoints[i + pointsPerSide]);
        drawPoint(zLinePoints[i + 2*pointsPerSide]);
        drawPoint(zLinePoints[i + 2*pointsPerSide]);
        drawPoint(zLinePoints[i + 3*pointsPerSide]);
        drawPoint(zLinePoints[i + 3*pointsPerSide]);
        drawPoint(zLinePoints[i]);
    }
}
