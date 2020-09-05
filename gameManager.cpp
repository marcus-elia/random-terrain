#include "gameManager.h"

GameManager::GameManager()
{
    screenWidth = 1024;
    screenHeight = 512;
    renderRadius = 5;
    chunkSeeds = PerlinNoiseGenerator(PERLIN_SEED_SIZE, PERLIN_SEED_SIZE, 1);

    initializePlayer();
    updateCurrentChunks();
    initializeButtons();
    makeInstructions();
}
GameManager::GameManager(int inputScreenWidth, int inputScreenHeight, int inputRenderRadius)
{
    screenWidth = inputScreenWidth;
    screenHeight = inputScreenHeight;
    renderRadius = inputRenderRadius;
    chunkSeeds = PerlinNoiseGenerator(PERLIN_SEED_SIZE, PERLIN_SEED_SIZE, 100);

    initializePlayer();
    updateCurrentChunks();
    initializeButtons();
    makeInstructions();
}

// =================================
//
//     Initialization Functions
//
// =================================

void GameManager::initializePlayer()
{
    Point playerStartLoc = {0, PLAYER_HEIGHT/2, 0};
    Point playerStartLook = {0, PLAYER_HEIGHT/2, -10};
    Point playerStartUp = {0, PLAYER_HEIGHT, 0};
    player = Player(playerStartLoc, playerStartLook, playerStartUp, PLAYER_SPEED, MOUSE_SENSITIVITY,
                    PLAYER_HEIGHT, PLAYER_RADIUS, MAX_DISTANCE_FROM_SPAWN, GRAVITY, PLAYER_JUMP_AMOUNT);
    currentPlayerChunkID = getChunkIDContainingPoint(player.getLocation(), CHUNK_SIZE);
}

void GameManager::initializeButtons()
{
    playButton = Button(screenWidth/2, screenHeight/2, BUTTON_WIDTH, BUTTON_HEIGHT,
                        BUTTON_RADIUS, "Play", PLAY_BUTTON_COLOR, BUTTON_TEXT_COLOR, PLAY_BUTTON_COLOR_H);
    playAgainButton = Button(screenWidth/2, screenHeight/2, BUTTON_WIDTH, BUTTON_HEIGHT,
                             BUTTON_RADIUS, "Play Again", PLAY_BUTTON_COLOR, BUTTON_TEXT_COLOR, PLAY_BUTTON_COLOR_H);
    continueButton = Button(screenWidth/2, screenHeight/2, BUTTON_WIDTH, BUTTON_HEIGHT,
                            BUTTON_RADIUS, "Continue", PLAY_BUTTON_COLOR, BUTTON_TEXT_COLOR, PLAY_BUTTON_COLOR_H);
    quitButton = Button(screenWidth/2, screenHeight/2 - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT,
                        BUTTON_RADIUS, "Quit", QUIT_BUTTON_COLOR, BUTTON_TEXT_COLOR, QUIT_BUTTON_COLOR_H);
}

void GameManager::makeInstructions()
{
    instructions.push_back("Use w,a,s,d to move and spacebar to jump. Press p to pause.");
}

// ===========================
//
//          Getters
//
// ===========================

Player GameManager::getPlayer() const
{
    return player;
}
bool GameManager::getWKey() const
{
    return wKey;
}
bool GameManager::getAKey() const
{
    return aKey;
}
bool GameManager::getSKey() const
{
    return sKey;
}
bool GameManager::getDKey() const
{
    return dKey;
}
bool GameManager::getSpacebar() const
{
    return spacebar;
}
GameStatus GameManager::getCurrentStatus() const
{
    return currentStatus;
}
bool GameManager::getCloseWindow() const
{
    return closeWindow;
}
bool GameManager::getShowMouse() const
{
    return showMouse;
}

// =============================
//
//           Setters
//
// =============================
void GameManager::setWKey(bool input)
{
    wKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey);
}
void GameManager::setAKey(bool input)
{
    aKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey);
}
void GameManager::setSKey(bool input)
{
    sKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey);
}
void GameManager::setDKey(bool input)
{
    dKey = input;
    player.setVelocity(wKey, aKey, sKey, dKey);
}
void GameManager::setSpacebar(bool input)
{
    spacebar = input;
}
void GameManager::setCurrentStatus(GameStatus input)
{
    currentStatus = input;
}

// =============================
//
//           Chunks
//
// ============================
void GameManager::updateCurrentChunks()
{
    // Update the list of current chunks
    currentChunks = std::vector<std::shared_ptr<Chunk>>();
    std::vector<Point2D> chunksInRadius = getChunkTopLeftCornersAroundPoint(currentPlayerChunkID, renderRadius);
    for(Point2D p : chunksInRadius)
    {
        int index = point2DtoChunkID(p);
        if(allSeenChunks.count(index) == 0) // if the chunk has never been seen before
        {
            // Create and add a new Chunk
            // Get the borders to make sure the terrain is seamless
            std::vector<double> relativeHeightsAbove = getTerrainHeightsAbove(index, true);
            std::vector<double> relativeHeightsBelow = getTerrainHeightsBelow(index, true);
            std::vector<double> relativeHeightsLeft = getTerrainHeightsLeft(index, true);
            std::vector<double> relativeHeightsRight = getTerrainHeightsRight(index, true);
            std::vector<double> absoluteHeightsAbove = getTerrainHeightsAbove(index, false);
            std::vector<double> absoluteHeightsBelow = getTerrainHeightsBelow(index, false);
            std::vector<double> absoluteHeightsLeft = getTerrainHeightsLeft(index, false);
            std::vector<double> absoluteHeightsRight = getTerrainHeightsRight(index, false);
            // Make a generator for this chunk specifically
            PerlinNoiseGenerator png = PerlinNoiseGenerator(POINTS_PER_CHUNK, POINTS_PER_CHUNK, 1,
                                                            relativeHeightsAbove, relativeHeightsBelow,
                                                            relativeHeightsLeft, relativeHeightsRight);
            // Scale the noise
            std::vector<std::vector<double>> noise = png.getScaledNoiseApplyBorders(0,1,
                                                            relativeHeightsAbove, relativeHeightsBelow,
                                                            relativeHeightsLeft, relativeHeightsRight);
            // Create the chunk
            allSeenChunks[index] = std::make_shared<Chunk>(p, CHUNK_SIZE, POINTS_PER_CHUNK, noise,
                    TERRAIN_HEIGHT_FACTOR, getPerlinValue(p), absoluteHeightsAbove, absoluteHeightsBelow,
                                                           absoluteHeightsLeft, absoluteHeightsRight,
                                                           SNOW_LIMIT, ROCK_LIMIT, GRASS_LIMIT, WATER_LEVEL,
                                                           SNOW_COLOR, ROCK_COLOR, GRASS_COLOR, SAND_COLOR, WATER_COLOR);
        }
        currentChunks.push_back(allSeenChunks[index]);
    }
}
std::vector<double> GameManager::getTerrainHeightsAbove(int chunkID, bool isRelative) const
{
    int aboveID = getChunkIDAbove(chunkID);
    if(allSeenChunks.count(aboveID) > 0)
    {
        return allSeenChunks.at(aboveID)->getBottomTerrainHeights(isRelative);
    }
    else
    {
        return std::vector<double>();
    }
}
std::vector<double> GameManager::getTerrainHeightsBelow(int chunkID, bool isRelative) const
{
    int aboveID = getChunkIDBelow(chunkID);
    if(allSeenChunks.count(aboveID) > 0)
    {
        return allSeenChunks.at(aboveID)->getTopTerrainHeights(isRelative);
    }
    else
    {
        return std::vector<double>();
    }
}
std::vector<double> GameManager::getTerrainHeightsLeft(int chunkID, bool isRelative) const
{
    int aboveID = getChunkIDLeft(chunkID);
    if(allSeenChunks.count(aboveID) > 0)
    {
        return allSeenChunks.at(aboveID)->getRightTerrainHeights(isRelative);
    }
    else
    {
        return std::vector<double>();
    }
}
std::vector<double> GameManager::getTerrainHeightsRight(int chunkID, bool isRelative) const
{
    int aboveID = getChunkIDRight(chunkID);
    if(allSeenChunks.count(aboveID) > 0)
    {
        return allSeenChunks.at(aboveID)->getLeftTerrainHeights(isRelative);
    }
    else
    {
        return std::vector<double>();
    }
}

double GameManager::getPerlinValue(Point2D p)
{
    return chunkSeeds.getScaledNoise(0.1,1)[mod(p.x, PERLIN_SEED_SIZE)][mod(p.z, PERLIN_SEED_SIZE)];
}

// ====================================
//
//             Camera
//
// ===================================
Point GameManager::getCameraLocation() const
{
    return player.getLocation();
}
Point GameManager::getCameraLookingAt() const
{
    return player.getLookingAt();
}
Point GameManager::getCameraUp() const
{
    return player.getUp();
}

// Mouse
void GameManager::reactToMouseMovement(int mx, int my, double theta)
{
    if(currentStatus == Intro)
    {
        playButton.setIsHighlighted(playButton.containsPoint(mx, screenHeight - my));

        quitButton.setIsHighlighted(quitButton.containsPoint(mx, screenHeight - my));
    }
    else if(currentStatus == Playing)
    {
        player.updateAngles(theta);
        player.updateSphericalDirectionBasedOnAngles();
        player.setVelocity(wKey, aKey, sKey, dKey);
    }
    else if(currentStatus == Paused)
    {
        continueButton.setIsHighlighted(continueButton.containsPoint(mx, screenHeight - my));

        quitButton.setIsHighlighted(quitButton.containsPoint(mx, screenHeight - my));
    }
    else if(currentStatus == End)
    {
        playAgainButton.setIsHighlighted(playAgainButton.containsPoint(mx, screenHeight - my));

        quitButton.setIsHighlighted(quitButton.containsPoint(mx, screenHeight - my));
    }
}
void GameManager::reactToMouseClick(int mx, int my)
{
    if(currentStatus == Intro)
    {
        if(playButton.containsPoint(mx, screenHeight - my))
        {
            showMouse = false;
            currentStatus = Playing;
            resetGame();
        }
        else if(quitButton.containsPoint(mx, screenHeight - my))
        {
            closeWindow = true;
        }
    }
    else if(currentStatus == Playing)
    {
    }
    else if(currentStatus == Paused)
    {
        if(continueButton.containsPoint(mx, screenHeight - my))
        {
            showMouse = false;
            currentStatus = Playing;
        }
        else if(quitButton.containsPoint(mx,screenHeight -  my))
        {
            closeWindow = true;
        }
    }
    else if(currentStatus == End)
    {
        if(playAgainButton.containsPoint(mx, screenHeight - my))
        {
            showMouse = false;
            resetGame();
        }
        else if(quitButton.containsPoint(mx,screenHeight -  my))
        {
            closeWindow = true;
        }
    }
}

void GameManager::draw() const
{
    if(currentStatus == Playing || currentStatus == Paused)
    {
        for(std::shared_ptr<Chunk> c : currentChunks)
        {
            c->draw();
        }
    }
}

// Tick helper functions
void GameManager::tick()
{
    if(currentStatus == Playing)
    {
        playerTick();
    }
}
void GameManager::playerTick()
{
    player.tick();
    if(spacebar)
    {
        player.tryToJump();
    }

    // Check if the player has entered a new chunk
    int newPlayerChunkID = getChunkIDContainingPoint(player.getLocation(), CHUNK_SIZE);
    if(newPlayerChunkID != currentPlayerChunkID)
    {
        currentPlayerChunkID = newPlayerChunkID;
        updateCurrentChunks();
    }
    player.setCurrentTerrainHeight(allSeenChunks[currentPlayerChunkID]->getHeightAt(player.getLocation()));
}

// Game Management
void GameManager::resetGame()
{
    initializePlayer();
    currentStatus = Playing;
}
void GameManager::togglePaused()
{
    if(currentStatus == Paused)
    {
        currentStatus = Playing;
        showMouse = false;
    }
    else if(currentStatus == Playing)
    {
        currentStatus = Paused;
        showMouse = true;
    }
}

// UI
void GameManager::drawUI() const
{
    if(currentStatus == Intro)
    {
        playButton.draw();
        quitButton.draw();
        displayInstructions();
    }
    else if(currentStatus == Playing)
    {
        drawCursor();
    }
    else if(currentStatus == Paused)
    {
        continueButton.draw();
        quitButton.draw();
    }
    else if(currentStatus == End)
    {
        playAgainButton.draw();
        quitButton.draw();
    }
}
void GameManager::drawCursor() const
{
    setGLColor(CURSOR_COLOR);
    glBegin(GL_QUADS);    // Draw a + shape with two quads
    glVertex2f(screenWidth/2 - 5, screenHeight/2 + 2);
    glVertex2f(screenWidth/2 - 5, screenHeight/2 - 2);
    glVertex2f(screenWidth/2 + 5, screenHeight/2 - 2);
    glVertex2f(screenWidth/2 + 5, screenHeight/2 + 2);


    glVertex2f(screenWidth/2 - 2, screenHeight/2 + 5);
    glVertex2f(screenWidth/2 - 2, screenHeight/2 - 5);
    glVertex2f(screenWidth/2 + 2, screenHeight/2 - 5);
    glVertex2f(screenWidth/2 + 2, screenHeight/2 + 5);
    glEnd();
}

void GameManager::displayInstructions() const
{
    setGLColor(BLACK);
    for(int i = 0; i < instructions.size(); i++)
    {
        std::string s = instructions[i];
        glRasterPos2i(10, screenHeight - 15*i - 15);
        for(const char &letter : s)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
        }
    }
}
