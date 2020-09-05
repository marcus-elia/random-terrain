#ifndef RANDOM_TERRAIN_GAMEMANAGER_H
#define RANDOM_TERRAIN_GAMEMANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "player.h"
#include "structs.h"
#include "chunk.h"
#include "mathHelper.h"
#include "button.h"
#include "perlinNoiseGenerator.h"

enum GameStatus {Intro, Playing, End, Paused};
enum ColorScheme {Plain, Majestic, Lava};

class GameManager
{
private:
    Player player;

    // Controls
    bool wKey, aKey, sKey, dKey, spacebar, hyperSpeed;

    // Chunks
    PerlinNoiseGenerator chunkSeeds;
    int renderRadius;
    std::unordered_map<int, std::shared_ptr<Chunk>> allSeenChunks;
    std::vector<std::shared_ptr<Chunk>> currentChunks;
    int currentPlayerChunkID;
    ColorScheme curColorScheme;
    RGBAcolor snowColor, rockColor, grassColor, sandColor, waterColor;

    GameStatus currentStatus;

    // UI
    int screenWidth, screenHeight;

    Button playButton;
    Button playAgainButton;
    Button quitButton;
    Button continueButton;
    Button cycleColorsButton;
    std::vector<std::string> instructions;

    bool closeWindow = false;
    bool showMouse = true;

    // Game parameters
    int CHUNK_SIZE = 512;
    int POINTS_PER_CHUNK = 30;
    int PERLIN_SEED_SIZE = 10;
    double TERRAIN_HEIGHT_FACTOR = 200;
    double SNOW_LIMIT = 350;
    double ROCK_LIMIT = 250;
    double GRASS_LIMIT = 75;
    double WATER_LEVEL = 65;
    RGBAcolor SNOW_COLOR_PLAIN = {1.0, 0.9, 1.0, 1.0};
    RGBAcolor ROCK_COLOR_PLAIN = {0.6, 0.64, 0.62, 1.0};
    RGBAcolor GRASS_COLOR_PLAIN = {0.2, 0.75, 0.08, 1.0};
    RGBAcolor SAND_COLOR_PLAIN = {1.0, 0.84, 0.33, 1.0};
    RGBAcolor WATER_COLOR_PLAIN = {0.0, 0.24, 1.0, 0.75};
    RGBAcolor SNOW_COLOR_MAJESTIC = {1.0, 0.9, 0.9, 1.0};
    RGBAcolor ROCK_COLOR_MAJESTIC = {0.4, 0.1, 0.42, 1.0};
    RGBAcolor GRASS_COLOR_MAJESTIC = {0.1, 0.48, 0.13, 1.0};
    RGBAcolor SAND_COLOR_MAJESTIC = {0.92, 0.82, 0.39, 1.0};
    RGBAcolor WATER_COLOR_MAJESTIC = {0.0, 0.54, 0.44, 0.75};
    RGBAcolor SNOW_COLOR_LAVA = {0.8, 0.2, 0.2, 0.99};
    RGBAcolor ROCK_COLOR_LAVA = {0.05, 0.0, 0.05, 1.0};
    RGBAcolor GRASS_COLOR_LAVA = {0.2, 0.25, 0.25, 1.0};
    RGBAcolor SAND_COLOR_LAVA = {0.15, 0.02, 0.01, 1.0};
    RGBAcolor WATER_COLOR_LAVA = {0.9, 0.5, 0.0, 0.87};
    double PLAYER_HEIGHT = 10;
    double PLAYER_RADIUS = 3;
    double PLAYER_SPEED = 1.5;
    double MOUSE_SENSITIVITY = 0.005;
    int MAX_DISTANCE_FROM_SPAWN = 20480; // 20 chunks
    double GRAVITY = -0.5;
    double PLAYER_JUMP_AMOUNT = 6;
    int HYPER_SPEED_FACTOR = 6;
    int BUTTON_WIDTH = 128;
    int BUTTON_HEIGHT = 64;
    int BUTTON_RADIUS = 16;
    RGBAcolor PLAY_BUTTON_COLOR = {0.0, 0.0, 0.7, 1.0};   // Slightly Dark Blue
    RGBAcolor QUIT_BUTTON_COLOR = {0.7, 0.0, 0.0, 1.0};   // Slightly Dark Red
    RGBAcolor CYCLE_BUTTON_COLOR = {0.1, 0.7, 0.0, 1.0};  // Slightly Dark Green
    RGBAcolor PLAY_BUTTON_COLOR_H = {0.0, 0.2, 1.0, 1.0}; // Lighter Blue
    RGBAcolor QUIT_BUTTON_COLOR_H = {1.0, 0.2, 0.0, 1.0}; // Lighter Red
    RGBAcolor CYCLE_BUTTON_COLOR_H = {0.1, 1.0, 0.1, 1.0};// Lighter Green
    RGBAcolor BUTTON_TEXT_COLOR = {1.0,1.0,1.0,1.0};      // White
    RGBAcolor CURSOR_COLOR = {0.3, 0.3, 0.3, 1.0};        // Dark Gray
    RGBAcolor BLACK = {0.0, 0.0, 0.0, 1.0};
public:
    GameManager();
    GameManager(int inputScreenWidth, int inputScreenHeight, int inputRenderRadius);

    // Helper functions for the constructors
    void initializePlayer();
    void initializeButtons();
    void makeInstructions();

    // Getters
    Player getPlayer() const;
    bool getWKey() const;
    bool getAKey() const;
    bool getSKey() const;
    bool getDKey() const;
    bool getSpacebar() const;
    GameStatus getCurrentStatus() const;
    bool getCloseWindow() const;
    bool getShowMouse() const;

    // Setters
    void setWKey(bool input);
    void setAKey(bool input);
    void setSKey(bool input);
    void setDKey(bool input);
    void setSpacebar(bool input);
    void setHyperSpeed(bool input);
    void setCurrentStatus(GameStatus input);

    // Chunks
    void updateCurrentChunks();
    // If the specified adjacent chunk has been created already, then
    // this returns the relevant border of terrain points.
    // Otherwise, returns empty vector
    std::vector<double> getTerrainHeightsAbove(int chunkID, bool isRelative) const;
    std::vector<double> getTerrainHeightsBelow(int chunkID, bool isRelative) const;
    std::vector<double> getTerrainHeightsLeft(int chunkID, bool isRelative) const;
    std::vector<double> getTerrainHeightsRight(int chunkID, bool isRelative) const;

    // Given the topLeft chunkCoords of a chunk, this returns the perlin seed
    // (between 0 and 1) for it given by the chunkSeeds P.N.G.
    double getPerlinValue(Point2D p);

    // Camera
    Point getCameraLocation() const;
    Point getCameraLookingAt() const;
    Point getCameraUp() const;

    // Mouse
    void reactToMouseMovement(int mx, int my, double theta, double distance);
    void reactToMouseClick(int mx, int my);

    void draw() const;

    // Tick helper functions
    void tick();
    void playerTick();

    // Game Management
    void checkForGameEnd();
    void resetGame();
    void togglePaused();
    void cycleColors();
    void updateColorScheme(ColorScheme inputScheme);

    // UI
    void drawUI() const;
    void drawCursor() const;
    void displayInstructions() const;
};

#endif //RANDOM_TERRAIN_GAMEMANAGER_H
