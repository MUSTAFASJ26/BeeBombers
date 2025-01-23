#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// Constants for scoring tiers
const int tierHeight = resolutionY / 10; // Divide the screen into 10 tiers


// Constants for bees
int maxBees = 20;                 // Maximum number of bees
int initialBees = 5;              // Number of bees at the start
const int beeSpeed = 5;                 // Speed of bee movement
Clock initialBeeSpawnClock;  // Controls the time gap for spawning initial bees
int initialBeesSpawned = 0; // Counter for spawned initial bees
Clock delayedBeeSpawnClock; // Clock to track the delay for spawning additional bees

//Lives Syestem
int playerLives = 3; // Player starts with 3 lives
Texture lifeTexture; // Texture for life sprites
std::vector<Sprite> lifeSprites; // Sprites to display lives
bool playerDied = false; // Variable to trigger player death logic
Clock gameOverClock; // Clock to handle game over screen duration

bool inMenu = true;

std::string playerName; // Player name
int playerScore = 0;    // Player's live score

// Leaderboard variables
std::vector<std::pair<std::string, int>> leaderboard;
const std::string leaderboardFile = "leaderboard.txt";

//music
Music menuMusic;
Music transitionMusic;
Music bgMusic;


// Replace static arrays with vectors
std::vector<float> beeX;
std::vector<float> beeY;
std::vector<bool> beeActive;
std::vector<bool> beeMovingRight;
std::vector<bool> beeFacingRight;

std::vector<float> honeycombX;
std::vector<float> honeycombY;
std::vector<bool> honeycombActive;

std::vector<float> randomHoneycombX;
std::vector<float> randomHoneycombY;
std::vector<bool> randomHoneycombActive;
std::vector<bool> isGeneratedHoneycomb;
std::vector<Clock> beeStopClock;  // Tracks individual timers for each bee
std::vector<bool> beeStopped;    // Tracks whether a bee is currently stopped
std::vector<Clock> beeResumeClock; // Tracks when a bee resumes after stopping

// New Variables for New Bee Type
int maxNewBees = 5;               // Maximum number of new bees
int initialNewBees = 1;            // Initial number of new bees
int activeNewBees = initialNewBees;
int totalNewBeesSpawned = initialNewBees;
Clock initialNewBeeSpawnClock;
Clock delayedNewBeeSpawnClock;

std::vector<float> newBeeX;
std::vector<float> newBeeY;
std::vector<bool> newBeeActive;
std::vector<bool> newBeeMovingRight;
std::vector<bool> newBeeFacingRight;

// New Honeycomb Variables for New Bees
std::vector<float> newHoneycombX;
std::vector<float> newHoneycombY;
std::vector<bool> newHoneycombActive;
std::vector<bool> isGeneratedNewHoneycomb;


// New Bee Stop and Resume Clocks
std::vector<Clock> newBeeStopClock;
std::vector<bool> newBeeStopped;
std::vector<Clock> newBeeResumeClock;


//bee animation
const int beeFrameWidth = 22;  // Width of a single frame (pixels)
const int beeFrameHeight = 46; // Height of a single frame (pixels)
const int beeFrameCount = 1;   // Number of frames in the sprite sheet
Clock beeAnimationClock;       // Clock to handle animation timing
int beeCurrentFrame = 0;       // Current animation frame


void generateInitialHoneycombs();
//random honey combs
int maxRandomHoneycombs = 3; // Number of randomly generated honeycombs
Texture newHoneycombTexture; // Texture for honeycombs from new bees



//bees logic
int activeBees = initialBees;           // Number of currently active bees
int totalBeesSpawned = initialBees;     // Track how many bees have been spawned so far
Clock beeMoveClock;                     // Clock to control bee movement timing
int fastbeeSpeed;

//lives message :
Font playerDiedFont;
Text playerDiedText;
Clock playerDiedClock;
bool showPlayerDiedMessage = false; // Flag to control text display


//background image
Texture backgroundTexture;
Sprite backgroundSprite;

//bird variables :
float birdX = resolutionX - boxPixelsX;  // Start position: right middle
float birdY = resolutionY / 2;
bool isTargetNewHoneycomb = false; // Indicates if the target honeycomb is new
int hummingbirdFrameWidth = 32;          // Width of each frame
int hummingbirdFrameHeight = 32;         // Height of each frame
int hummingbirdFrameCount = 2;           // Number of frames in the sprite sheet
int hummingbirdCurrentFrame = 0;         // Current frame index
Clock hummingbirdAnimationClock;         // Clock for animation timing

float hummingbirdSpeed = 2.0f;           // Speed of hummingbird movement
int targetHoneycombIndex = -1;           // Index of the honeycomb the bird is targeting
Clock honeycombWaitClock;                // Clock for wait time above honeycomb

Texture hummingbirdTexture;              // Hummingbird texture
Sprite hummingbirdSprite;                // Hummingbird sprite

//Beehives :
// Beehive Variables
int maxBeehives; // Maximum number of beehives per level
std::vector<float> beehiveX; // X-coordinates of beehives
std::vector<float> beehiveY; // Y-coordinates of beehives
std::vector<bool> beehiveActive; // Active state of beehives
std::vector<Sprite> beehiveSprites; // Beehive sprites
Texture beehiveTexture; // Texture for beehives

// Scoring Variables
int workerBeeHits = 0;
int killerBeeHits = 0;
int yellowHoneycombsEaten[3] = {0, 0, 0}; // Top, middle, bottom tiers
int redHoneycombsEaten[3] = {0, 0, 0};    // Top, middle, bottom tiers
int activeBeehivesByTier[3] = {0, 0, 0};  // Top, middle, bottom tiers

// Bonus Points Milestones
int bonusPoints[] = {20000, 40000, 80000};
int nextBonusIndex = 0;

//Powerup setup :

enum PowerUpType { SPEED_INC, SPEED_DEC, HEIGHT_INC, HEIGHT_DEC };

// Power-up textures and variables
Texture speedIncTexture, speedDecTexture, heightIncTexture, heightDecTexture;
struct PowerUp {
    Sprite sprite;
    PowerUpType type;
    Vector2f position;
    float velocityY = 2.0f; // Falling speed
    bool active = false;
    bool grounded = false;
};
std::vector<PowerUp> powerUps; // Store all active power-ups

// Timer variables for power-up effects
Clock powerUpTimer;
bool powerUpActive = false; // Whether a power-up is active
PowerUpType currentPowerUpType; // Currently active power-up type

// Default and current player speed
const float defaultPlayerSpeed = 5.0f; // Adjust as necessary
float playerSpeed = defaultPlayerSpeed;
sf::Text powerUpTimerText;
// Player sprite (if you want it as global)
Sprite playerSprite;


//sound effects
SoundBuffer fireSoundBuffer;
Sound fireSound;
SoundBuffer hitBuffer;
Sound hitSound;




//obstacle variables
vector<Vector2f> obstacles; // Stores the positions of obstacles
Texture obstacleTexture;

//Initialisation :
void initializeLevel(int maxBees, int maxRandomHoneycombs, int maxNewBees) {
    // Resize bee-related vectors
    beeX.resize(maxBees, 0.0f);
    beeY.resize(maxBees, 0.0f);
    beeActive.resize(maxBees, false);
    beeMovingRight.resize(maxBees, false);
    beeFacingRight.resize(maxBees, true);

    honeycombX.resize(maxBees, 0.0f);
    honeycombY.resize(maxBees, 0.0f);
    honeycombActive.resize(maxBees, false);

    // Resize the `isGeneratedHoneycomb` vector
    isGeneratedHoneycomb.resize(maxBees, false);
    isGeneratedNewHoneycomb.resize(maxNewBees, false);


    // Resize honeycomb-related vectors
    randomHoneycombX.resize(maxRandomHoneycombs, 0.0f);
    randomHoneycombY.resize(maxRandomHoneycombs, 0.0f);
    randomHoneycombActive.resize(maxRandomHoneycombs, false);
    
    beeStopClock.resize(maxBees);
    beeResumeClock.resize(maxBees);
    beeStopped.resize(maxBees, false);

    //new fast bees :
    newBeeX.resize(maxNewBees, 0.0f);
    newBeeY.resize(maxNewBees, 0.0f);
    newBeeActive.resize(maxNewBees, false);
    newBeeMovingRight.resize(maxNewBees, false);
    newBeeFacingRight.resize(maxNewBees, true);

    newHoneycombX.resize(maxNewBees, 0.0f);
    newHoneycombY.resize(maxNewBees, 0.0f);
    newHoneycombActive.resize(maxNewBees, false);

    newBeeStopClock.resize(maxNewBees);
    newBeeResumeClock.resize(maxNewBees);
    newBeeStopped.resize(maxNewBees, false);


    
}




int currentLevel = 1;


//bullet variables
void checkBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists);





/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                      Function Declarations                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void movePlayer(float& player_x, const Clock& playerClock, Sprite& playerSprite, const vector<Vector2f>& obstacles);
void fireBullet(float player_x, float player_y, float& bullet_x, float& bullet_y, bool& bullet_exists);
void moveBees(RectangleShape& groundRectangle, Texture& obstacleTexture);
void checkBulletCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Texture& honeycombTexture);
void drawBees(RenderWindow& window, Texture& beeTexture);
void drawHoneycombs(RenderWindow& window, Texture& honeycombTexture);
void checkBeePlayerCollision(float player_x, float player_y, Sprite& playerSprite);
void generateInitialHoneycombs();
void checkBulletRandomHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists);
void drawRandomHoneycombs(RenderWindow& window, Texture& honeycombTexture);
void setFixedBackgroundBrightness(Sprite& backgroundSprite, float brightnessFactor);
void spawnObstacle(float beeX, float beeY, float playerX, float playerY, vector<Vector2f>& obstacles);
void drawObstacles(RenderWindow& window, Texture& obstacleTexture, const vector<Vector2f>& obstacles);
void playLevel1(RenderWindow &window);
void playLevel2(RenderWindow &window);
void playLevel3(RenderWindow &window);
void playBossLevel(RenderWindow &window);
void adjustBrightness(Sprite& sprite, float brightnessFactor);
void initializeNewBees(int maxNewBees);
void spawnNewBees();
void moveNewBees(RectangleShape& groundRectangle, Texture& obstacleTexture);
void checkNewBeeBulletCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Texture& newHoneycombTexture);
void drawNewBees(RenderWindow& window, Texture& newBeeTexture);
void drawNewHoneycombs(RenderWindow& window); 
void checkBulletNewHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists); 
void initializeHummingbird(Texture& hummingbirdTexture, Sprite& hummingbirdSprite);
void updateHummingbirdAnimation(Sprite& hummingbirdSprite, Clock& animationClock, int& frameIndex);
void moveHummingbird(float& birdX, float& birdY, float speed, const vector<float>& honeycombX, const vector<float>& honeycombY, const vector<bool>& honeycombActive, int& targetIndex, Clock& waitClock);
void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite);
void loadLeaderboard();
void saveLeaderboard();
void updateLeaderboard(const std::string& name, int score);
void renderLeaderboard(RenderWindow& window, const Font& font);
void getPlayerName(RenderWindow& window);
void updateScoreForBeeHit(bool isKillerBee, float beeY);
void updateScoreForHoneycombEaten(bool isRed, float honeycombY);
void updateScoreForBeehive(float beehiveY);
void checkForBonus();
void spawnPowerUp(float x, float y);
void updatePowerUps();
void checkPlayerPowerUpCollision(Sprite& playerSprite);
void applyPowerUp(PowerUpType type);








//menu

class Menu {
private:
    Font font;
    vector<Text> menuOptions;
    int selectedLevel;
    Texture backgroundTexture; // Background texture for the menu
    Sprite backgroundSprite;  // Background sprite for the menu

 
    
public:
    Menu() : selectedLevel(-1) {
     
        // Load font for menu
        if (!font.loadFromFile("Fonts/arial.ttf")) {
            cout << "Error loading font!" << endl;
        }
        
        if (!menuMusic.openFromFile("Music/risk.ogg")) {
	    cout << "Error loading menu music!" << endl;
	}
	menuMusic.setVolume(50); // Set the volume (adjust as needed)
	menuMusic.setLoop(true); // Loop the music
	menuMusic.play();        // Start playing

        
        // Load the background image
        if (!backgroundTexture.loadFromFile("Textures/menubomb.png")) {
            cout << "Error loading menu background image!" << endl;
        }

        // Set the texture to the sprite
        backgroundSprite.setTexture(backgroundTexture);

        // Optionally scale the background to fit the window
        backgroundSprite.setScale(
            static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
            static_cast<float>(resolutionY) / backgroundTexture.getSize().y
        );
       
       // Adjust the brightness of the background
       adjustBrightness(backgroundSprite, 0.5f); // 50% brightness



        // Initialize menu options
        vector<string> options = {"Level 1", "Level 2", "Level 3", "Boss Level"};
        for (int i = 0; i < options.size(); i++) {
            Text option;
            option.setFont(font);
            option.setString(options[i]);
            option.setCharacterSize(50);
            option.setFillColor(Color::White);

            // Dynamically center text
            FloatRect textBounds = option.getLocalBounds();
            option.setOrigin(textBounds.width / 2, textBounds.height / 2);
            // Adjust vertical spacing, adding extra space before "Boss Level"
	    if (options[i] == "Boss Level") {
		option.setPosition(480, 150 + i * 80); // Increased space for "Boss Level"
	    } else {
		option.setPosition(480, 150 + i * 80); // Regular spacing
	    }
            menuOptions.push_back(option);
        }
    }
    
    Font getFont() const {
        return font;
    }

    // Render the menu
    void render(RenderWindow &window) {
    
    
        // Draw the background first
    	window.draw(backgroundSprite);

        for (auto &option : menuOptions) {
            // Highlight the menu option on hover
            if (option.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) {
                option.setFillColor(Color::Green); // Highlight hovered option
            } else {
                option.setFillColor(Color::White);
            }
            window.draw(option);
        }
    }

    // Check for user input and return selected level
    int handleInput(RenderWindow &window, Event &event) {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            for (int i = 0; i < menuOptions.size(); i++) {
                if (menuOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedLevel = i + 1; // Levels are 1-indexed
                    menuMusic.stop(); 
                    return selectedLevel;
                }
            }
        }
        return -1; // No selection
    }

    // Get the selected level
    int getSelectedLevel() const {
        return selectedLevel;
    }
};

// MAIN :
int main() {
    RenderWindow window(VideoMode(960, 640), "Bee Bombers", Style::Close | Style::Titlebar);
    
    window.setFramerateLimit(240);
    
    loadLeaderboard(); // Load leaderboard at game start
    getPlayerName(window); // Get player name
    
    Menu menu;

    inMenu = true;
    int selectedLevel = -1;
    bool isFullscreen = false;
    
    srand(time(0));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (inMenu) {
                selectedLevel = menu.handleInput(window, event);
                if (selectedLevel != -1) {
                    inMenu = false;
                    currentLevel = selectedLevel; // Set to the selected level

                }
            }
        }

        window.clear(Color::Black);
        if (inMenu) {
        if (menuMusic.getStatus() != Music::Playing) {
		menuMusic.play(); // Restart menu music if not already playing
	}
            menu.render(window);
            renderLeaderboard(window, menu.getFont()); // Render leaderboard in menu
            
        } else {

            switch (selectedLevel) {
                case 1:
                    playLevel1(window);
                    inMenu = true;
                    break;
                case 2:
                    playLevel2(window);
                    inMenu = true;
                    break;
                case 3:
                    playLevel3(window);
                    inMenu = true;
                    break;
                case 4:
                    playBossLevel(window);
                    inMenu = true;
                    break;
                default:
                    inMenu = true;
                    break;
            }
        }
        window.display();
    }

    return 0;
}




void moveBees(RectangleShape& groundRectangle, Texture& obstacleTexture) {
    if (beeMoveClock.getElapsedTime().asMilliseconds() < 4) // Control bee movement speed
        return;

    beeMoveClock.restart();

    for (int i = 0; i < beeX.size(); i++) {
        if (!beeActive[i]) continue;

        // Check if the bee should stop
        if (!beeStopped[i] && beeStopClock[i].getElapsedTime().asSeconds() > 8) {
            beeStopped[i] = true;  // Mark the bee as stopped
            beeResumeClock[i].restart();  // Start the stop duration timer
        }

        // Check if the bee should resume
        if (beeStopped[i] && beeResumeClock[i].getElapsedTime().asSeconds() > 2.5) {
            beeStopped[i] = false;  // Resume movement
            beeStopClock[i].restart();  // Reset the stop timer
        }

        // Skip movement if the bee is stopped
        if (beeStopped[i]) continue;

        // Handle horizontal movement and edge collisions
        if (beeMovingRight[i]) {
            beeX[i] += beeSpeed;
            if (beeX[i] + boxPixelsX >= resolutionX) { // Hits the right edge
                beeX[i] = resolutionX - boxPixelsX;   // Cap position
                beeY[i] += boxPixelsY;                // Drop down
                beeMovingRight[i] = false;            // Change direction
                beeFacingRight[i] = false;            // Flip sprite to face left
            }
        } else {
            beeX[i] -= beeSpeed;
            if (beeX[i] < 0) { // Hits the left edge
                beeX[i] = 0;
                beeY[i] += boxPixelsY;                // Drop down
                beeMovingRight[i] = true;             // Change direction
                beeFacingRight[i] = true;             // Flip sprite to face right
            }
        }

        // Check collision with honeycombs and reverse direction without dropping height
        for (int j = 0; j < beeX.size(); j++) {
            if (!honeycombActive[j]) continue;

            // Check if the bee intersects with the honeycomb
            if (beeX[i] < honeycombX[j] + boxPixelsX &&
                beeX[i] + boxPixelsX > honeycombX[j] &&
                beeY[i] < honeycombY[j] + boxPixelsY &&
                beeY[i] + boxPixelsY > honeycombY[j]) {

                // Reverse direction of the bee
                beeMovingRight[i] = !beeMovingRight[i];
                beeFacingRight[i] = beeMovingRight[i]; // Flip the sprite

                // Move the bee slightly away from the honeycomb to prevent sticking
                if (beeMovingRight[i]) {
                    beeX[i] += beeSpeed;
                } else {
                    beeX[i] -= beeSpeed;
                }
                break; // Exit the loop after handling collision
            }
        }
        // Check collision with initial honeycombs and reverse direction without dropping height
        for (int j = 0; j < beeX.size(); j++) {
            if (!randomHoneycombActive[j]) continue;

            // Check if the bee intersects with the honeycomb
            if (beeX[i] < randomHoneycombX[j] + boxPixelsX &&
                beeX[i] + boxPixelsX > randomHoneycombX[j] &&
                beeY[i] < randomHoneycombY[j] + boxPixelsY &&
                beeY[i] + boxPixelsY > randomHoneycombY[j]) {

                // Reverse direction of the bee
                beeMovingRight[i] = !beeMovingRight[i];
                beeFacingRight[i] = beeMovingRight[i]; // Flip the sprite

                // Move the bee slightly away from the honeycomb to prevent sticking
                if (beeMovingRight[i]) {
                    beeX[i] += beeSpeed;
                } else {
                    beeX[i] -= beeSpeed;
                }
                break; // Exit the loop after handling collision
            }
        }
        
        // Check collision with new honeycombs and reverse direction
        for (int j = 0; j < newHoneycombX.size(); j++) {
            if (!newHoneycombActive[j]) continue;

            if (beeX[i] < newHoneycombX[j] + boxPixelsX &&
                beeX[i] + boxPixelsX > newHoneycombX[j] &&
                beeY[i] < newHoneycombY[j] + boxPixelsY &&
                beeY[i] + boxPixelsY > newHoneycombY[j]) {

                // Reverse direction
                beeMovingRight[i] = !beeMovingRight[i];
                beeFacingRight[i] = beeMovingRight[i]; // Flip sprite

                // Slight movement adjustment to prevent sticking
                beeX[i] += beeMovingRight[i] ? beeSpeed : -beeSpeed;
                break;
            }
        }
    }
}


void checkBulletCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Texture& honeycombTexture) {
    for (int i = 0; i < beeX.size(); i++) {
        if (!beeActive[i]) continue;

        if (bullet_x < beeX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > beeX[i] &&
            bullet_y < beeY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > beeY[i]) {

            // Slightly adjust the honeycomb's spawn position to avoid overlaps
            float offsetX = beeMovingRight[i] ? 10.0f : -10.0f; // Offset based on bee direction
            honeycombX[i] = beeX[i] + offsetX;
            honeycombY[i] = beeY[i];
            honeycombActive[i] = true;
            isGeneratedHoneycomb[i] = false;

            beeActive[i] = false;
            bullet_exists = false;
            activeBees--;
            
            updateScoreForBeeHit(false, beeY[i]); // Pass `false` for worker bee
            
            break;
        }
    }
}




// Drawing Bees
void drawBees(RenderWindow& window, Texture& beeTexture) {
    for (int i = 0; i < beeX.size(); i++) {
        if (beeActive[i]) {
            Sprite beeSprite;
            beeSprite.setTexture(beeTexture);
            
        


            // Flip sprite based on the direction the bee is facing
            if (beeFacingRight[i]) {
                beeSprite.setScale(-0.7, 0.7); // Facing right (default orientation)
            } else {
                beeSprite.setScale(0.7, 0.7); // Facing left (flipped horizontally)
                beeSprite.setPosition(beeX[i] + boxPixelsX, beeY[i]); // Adjust position for flipping
            }

            beeSprite.setPosition(beeX[i], beeY[i]);
            window.draw(beeSprite);
        }
    }
}




// Drawing Honeycombs
void drawHoneycombs(RenderWindow& window, Texture& honeycombTexture) {
    for (int i = 0; i < beeX.size(); i++) {
        if (honeycombActive[i]) {
            Sprite honeycombSprite;
            honeycombSprite.setTexture(honeycombTexture);
            honeycombSprite.setScale(0.6, 0.6);
            honeycombSprite.setPosition(honeycombX[i], honeycombY[i]);
            window.draw(honeycombSprite);
        }
    }
}



void movePlayer(float& player_x, const Clock& playerClock, Sprite& playerSprite, const vector<Vector2f>& obstacles) {
    static Clock moveClock;
    if (moveClock.getElapsedTime().asMilliseconds() < 65) {
        return;
    }

    // Get the player's bounding box
    FloatRect playerBounds = playerSprite.getGlobalBounds();

    // Check for obstacles on the left and right
    bool canMoveLeft = true, canMoveRight = true;

    for (const auto& obstacle : obstacles) {
        FloatRect obstacleBounds(obstacle.x, obstacle.y, boxPixelsX, boxPixelsY);

        if (obstacleBounds.intersects(FloatRect(playerBounds.left - boxPixelsX, playerBounds.top, playerBounds.width, playerBounds.height))) {
            canMoveLeft = false;
        }
        if (obstacleBounds.intersects(FloatRect(playerBounds.left + boxPixelsX, playerBounds.top, playerBounds.width, playerBounds.height))) {
            canMoveRight = false;
        }
    }

    // Determine if the player is stuck (no left or right movement possible)
    if (!canMoveLeft && !canMoveRight) {
        cout << "Player is stuck between obstacles!" << endl;
        playerDied = true; // Trigger player death
        return;
    }

    // Detect movement direction
    if (Keyboard::isKeyPressed(Keyboard::Left) && canMoveLeft) {
        player_x -= boxPixelsX;
        if (player_x < 0) player_x = 0; // Prevent moving out of bounds
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) && canMoveRight) {
        player_x += boxPixelsX;
        if (player_x > resolutionX - playerSprite.getGlobalBounds().width)
            player_x = resolutionX - playerSprite.getGlobalBounds().width; // Prevent moving out of bounds
    }

    moveClock.restart();
}

void fireBullet(float player_x, float player_y, float& bullet_x, float& bullet_y, bool& bullet_exists) {
    if (!bullet_exists) {
        bullet_x = player_x + 24;
        bullet_y = player_y;
        bullet_exists = true;
        
        // Play the fire sound effect
        fireSound.play();
    }
}

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 5)
        return;

    bulletClock.restart();
    bullet_y -= 26;
    if (bullet_y < -32)
        bullet_exists = false;
}

void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
}



void checkBulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists) {
    for (int i = 0; i < beeX.size(); i++) {
        if (!honeycombActive[i]) continue;

        // Check collision between bullet and honeycomb
        if (bullet_x < honeycombX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > honeycombX[i] &&
            bullet_y < honeycombY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > honeycombY[i]) {

            bullet_exists = false; // Deactivate the bullet
            honeycombActive[i] = false; // Deactivate the honeycomb
            // Play hit sound effect
            hitSound.play();
            break; // Stop checking after the first collision
        }
    }
    // Check collision with new honeycombs
    for (int i = 0; i < newHoneycombX.size(); i++) {
        if (!newHoneycombActive[i]) continue;

        // Check collision between bullet and new honeycomb
        if (bullet_x < newHoneycombX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > newHoneycombX[i] &&
            bullet_y < newHoneycombY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > newHoneycombY[i]) {

            bullet_exists = false;          // Deactivate the bullet
            
            spawnPowerUp(newHoneycombX[i], newHoneycombY[i]);
            
            newHoneycombActive[i] = false; // Deactivate the new honeycomb
            // Play hit sound effect
            hitSound.play();
            break;                         // Stop checking after the first collision
        }
    }
}


void generateInitialHoneycombs() {
    // Define the threshold for honeycomb placement (above player's position)
    int maxHoneycombY = (gameColumns - 4) * boxPixelsY; // 4 rows above the player's Y-axis

    for (int i = 0; i < randomHoneycombX.size(); i++) {
        int randomX = rand() % (resolutionX - boxPixelsX); // Full width
        int randomY = rand() % maxHoneycombY; // Restrict to upper portion of the screen

        randomHoneycombX[i] = randomX;
        randomHoneycombY[i] = randomY;
        randomHoneycombActive[i] = true; // Mark as active
    }
}


void drawRandomHoneycombs(RenderWindow& window, Texture& honeycombTexture) {
    for (int i = 0; i < randomHoneycombX.size(); i++) {
        if (randomHoneycombActive[i]) {
            Sprite honeycombSprite;
            honeycombSprite.setTexture(honeycombTexture);
            honeycombSprite.setScale(0.6, 0.6);
            honeycombSprite.setPosition(randomHoneycombX[i], randomHoneycombY[i]);
            window.draw(honeycombSprite);
        }
    }
}

void checkBulletRandomHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists) {
    for (int i = 0; i < randomHoneycombX.size(); i++) {
        if (!randomHoneycombActive[i]) continue;

        // Check collision between bullet and random honeycomb
        if (bullet_x < randomHoneycombX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > randomHoneycombX[i] &&
            bullet_y < randomHoneycombY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > randomHoneycombY[i]) {

            bullet_exists = false; // Deactivate the bullet
            randomHoneycombActive[i] = false; // Deactivate the honeycomb
            // Play hit sound effect
            hitSound.play();
            break; // Stop after first collision
        }
    }
}




void checkBeePlayerCollision(float player_x, float player_y, Sprite& playerSprite) {
    for (int i = 0; i < beeX.size(); i++) {
        if (!beeActive[i]) continue;

        // Check collision between player and bee
        if (player_x < beeX[i] + boxPixelsX &&
            player_x + playerSprite.getGlobalBounds().width > beeX[i] &&
            player_y < beeY[i] + boxPixelsY &&
            player_y + playerSprite.getGlobalBounds().height > beeY[i]) {

            // Reverse the bee's direction
            beeMovingRight[i] = !beeMovingRight[i];
            beeFacingRight[i] = beeMovingRight[i]; // Update facing direction

            // Move the bee slightly away from the player to prevent sticking
            if (beeMovingRight[i]) {
                beeX[i] += beeSpeed; // Move right
            } else {
                beeX[i] -= beeSpeed; // Move left
            }

            break; // Stop after handling one collision
        }
    }
    
    // Check for collision with new bees
    for (int i = 0; i < newBeeX.size(); i++) {
        if (!newBeeActive[i]) continue;

        if (player_x < newBeeX[i] + boxPixelsX &&
            player_x + playerSprite.getGlobalBounds().width > newBeeX[i] &&
            player_y < newBeeY[i] + boxPixelsY &&
            player_y + playerSprite.getGlobalBounds().height > newBeeY[i]) {

            // Reverse the new bee's direction
            newBeeMovingRight[i] = !newBeeMovingRight[i];
            newBeeFacingRight[i] = newBeeMovingRight[i];
            newBeeX[i] += newBeeMovingRight[i] ? beeSpeed : -beeSpeed;
            break;
        }
    }
}

void checkBulletNewBeeCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Texture& honeycombTexture) {
    for (int i = 0; i < newBeeX.size(); i++) {
        if (!newBeeActive[i]) continue;

        // Check collision between bullet and new bee
        if (bullet_x < newBeeX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > newBeeX[i] &&
            bullet_y < newBeeY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > newBeeY[i]) {

            // Create a new honeycomb at the new bee's position
            newHoneycombX[i] = newBeeX[i];
            newHoneycombY[i] = newBeeY[i];
            newHoneycombActive[i] = true;

            // Deactivate the hit new bee
            newBeeActive[i] = false;
            bullet_exists = false;
            
            updateScoreForBeeHit(true, newBeeY[i]); // Pass `true` for killer bee

            break; // Process only one collision per bullet
        }
    }
}

// Function to set the background brightness
void setFixedBackgroundBrightness(Sprite& backgroundSprite, float brightnessFactor) {
    // Clamp brightnessFactor between 0.0 (completely dark) and 1.0 (original brightness)
    brightnessFactor = std::max(0.0f, std::min(brightnessFactor, 1.0f));

    // Calculate the brightness color
    Color brightnessColor(
        static_cast<Uint8>(255 * brightnessFactor), // Red
        static_cast<Uint8>(255 * brightnessFactor), // Green
        static_cast<Uint8>(255 * brightnessFactor)  // Blue
    );

    // Apply the brightness to the background sprite
    backgroundSprite.setColor(brightnessColor);
}

// Spawn obstacles based on bee position
void spawnObstacle(float beeX, float beeY, float playerX, float playerY, vector<Vector2f>& obstacles) {
    const float leftBoundary = 0;
    const float rightBoundary = resolutionX - boxPixelsX;

    // Check if the bee landed on the ground
    if (beeY >= resolutionY - 2 * boxPixelsY) {
        if (beeX <= leftBoundary + boxPixelsX) {
            // Left side: Add obstacles sequentially
            float currentX = leftBoundary;
            int obstacleCount = 0;

            // Count existing obstacles on the left side
            for (const auto& obstacle : obstacles) {
                if (obstacle.x >= leftBoundary && obstacle.x < leftBoundary + resolutionX / 2 &&
                    obstacle.y == resolutionY - 2 * boxPixelsY - 32) {
                    obstacleCount++;
                }
            }

            // Add new obstacle at the next available position
            if(obstacleCount != 0){
            	currentX = leftBoundary + obstacleCount * boxPixelsX;
            	obstacles.push_back(Vector2f(currentX, resolutionY - 2 * boxPixelsY - 32));
            }
            else{
            	currentX = leftBoundary + obstacleCount * boxPixelsX;
            	obstacles.push_back(Vector2f(currentX, resolutionY - 2 * boxPixelsY - 32));
            	}
 
            // Add the second obstacle for the first bee landing
            if (obstacleCount == 0) {
                obstacles.push_back(Vector2f(currentX + boxPixelsX, resolutionY - 2 * boxPixelsY - 32));
            }
        } else if (beeX >= rightBoundary - boxPixelsX) {
            // Right side: Add obstacles sequentially
            float currentX = rightBoundary;
            int obstacleCount = 0;

            // Count existing obstacles on the right side
            for (const auto& obstacle : obstacles) {
                if (obstacle.x <= rightBoundary && obstacle.x > rightBoundary - resolutionX / 2 &&
                    obstacle.y == resolutionY - 2 * boxPixelsY - 32) {
                    obstacleCount++;
                }
            }

            // Add new obstacle at the next available position
            currentX = rightBoundary - obstacleCount * boxPixelsX;
            obstacles.push_back(Vector2f(currentX , resolutionY - 2 * boxPixelsY - 32));

            // Add the second obstacle for the first bee landing
            if (obstacleCount == 0) {
                obstacles.push_back(Vector2f(currentX - boxPixelsX , resolutionY - 2 * boxPixelsY - 32));
            }
        } else {
            // Middle of the screen: Add obstacle at the exact landing position
            obstacles.push_back(Vector2f(beeX, resolutionY - 2 * boxPixelsY - 32));
        }
    }
}

void adjustBrightness(Sprite& sprite, float brightnessFactor) {
    brightnessFactor = std::max(0.0f, std::min(brightnessFactor, 1.0f));

    // Calculate the brightness color
    Color brightnessColor(
        static_cast<Uint8>(255 * brightnessFactor), // Red
        static_cast<Uint8>(255 * brightnessFactor), // Green
        static_cast<Uint8>(255 * brightnessFactor)  // Blue
    );

    // Apply the brightness to the sprite
    sprite.setColor(brightnessColor);
}

void drawObstacles(RenderWindow& window, Texture& obstacleTexture, const vector<Vector2f>& obstacles) {
    for (const auto& obstaclePos : obstacles) {
        Sprite obstacleSprite;
        obstacleSprite.setTexture(obstacleTexture);

        // Set the scale (width and height multipliers)
        float scaleX = 1.0f; // Width multiplier (e.g., 1.5 makes it 50% larger)
        float scaleY = 1.0f; // Height multiplier
        obstacleSprite.setScale(scaleX, scaleY);

        obstacleSprite.setPosition(obstaclePos);
        window.draw(obstacleSprite);
    }
}

//Humming Bird :
void initializeHummingbird(Texture& hummingbirdTexture, Sprite& hummingbirdSprite) {
    if (!hummingbirdTexture.loadFromFile("Textures/bird.png")) {
        cout << "Error loading hummingbird sprite sheet!" << endl;
    }

    hummingbirdSprite.setTexture(hummingbirdTexture);
    hummingbirdSprite.setTextureRect(IntRect(0, 0, hummingbirdFrameWidth, hummingbirdFrameHeight));
    hummingbirdSprite.setScale(1.5f, 1.5f);  // Scale the sprite
    hummingbirdSprite.setPosition(birdX, birdY);
}

void updateHummingbirdAnimation(Sprite& hummingbirdSprite, Clock& animationClock, int& frameIndex) {
    if (animationClock.getElapsedTime().asMilliseconds() > 200) {  // Switch frames every 200ms
        frameIndex = (frameIndex + 1) % hummingbirdFrameCount;  // Cycle through frames
        int frameX = frameIndex * hummingbirdFrameWidth;  // Calculate X position on the sprite sheet
        hummingbirdSprite.setTextureRect(IntRect(frameX, 0, hummingbirdFrameWidth, hummingbirdFrameHeight));
        animationClock.restart();
    }
}

void moveHummingbird(float& birdX, float& birdY, float speed, const vector<float>& honeycombX, const vector<float>& honeycombY, vector<bool>& honeycombActive, const vector<float>& newHoneycombX, const vector<float>& newHoneycombY, vector<bool>& newHoneycombActive, int& targetIndex, bool& isTargetNewHoneycomb, Clock& waitClock) {
    // If the bird has a target honeycomb
    if (targetIndex != -1) {
        float targetX = isTargetNewHoneycomb ? newHoneycombX[targetIndex] : honeycombX[targetIndex];
        float targetY = isTargetNewHoneycomb ? newHoneycombY[targetIndex] : honeycombY[targetIndex];
        
        
        // Validate if the target is still active
        if ((isTargetNewHoneycomb && !newHoneycombActive[targetIndex]) || (!isTargetNewHoneycomb && !honeycombActive[targetIndex])) {
            targetIndex = -1; // Reset the target
            return;           // Exit early since the target is no longer valid
        }

        // Move the bird towards the target honeycomb
        if (birdX < targetX) birdX += speed;
        if (birdX > targetX) birdX -= speed;
        if (birdY < targetY) birdY += speed;
        if (birdY > targetY) birdY -= speed;

        // If the bird reaches the target honeycomb
        if (abs(birdX - targetX) < speed && abs(birdY - targetY) < speed) {
            birdX = targetX;
            birdY = targetY;

            // Start or check the wait timer
            if (waitClock.getElapsedTime().asSeconds() == 0) {
                waitClock.restart(); // Restart the clock when first reaching the honeycomb
            }

            // Wait for 5 seconds before deactivating the honeycomb
            if (waitClock.getElapsedTime().asSeconds() >= 5.0f) {
                if (isTargetNewHoneycomb) {
                    spawnPowerUp(targetX, targetY); // Drop power-up from honeycomb location
                    newHoneycombActive[targetIndex] = false; // Deactivate new honeycomb
                } else {
                    spawnPowerUp(targetX, targetY); // Drop power-up from honeycomb location
                    honeycombActive[targetIndex] = false; // Deactivate previous honeycomb
                }
                
                // Update the score and deactivate the honeycomb
		if (isTargetNewHoneycomb) {
		    updateScoreForHoneycombEaten(true, newHoneycombY[targetIndex]); // Red honeycomb
		    newHoneycombActive[targetIndex] = false; // Mark as inactive
		} else {
		    updateScoreForHoneycombEaten(false, honeycombY[targetIndex]); // Yellow honeycomb
		    honeycombActive[targetIndex] = false; // Mark as inactive
		}
   
                targetIndex = -1; // Clear the target
                waitClock.restart(); // Reset the clock for the next target
            }
        }
    } else {
        // Find a new honeycomb to target (prioritize previous honeycombs)
        for (int i = 0; i < honeycombX.size(); ++i) {
            if (honeycombActive[i]) {
                targetIndex = i;
                isTargetNewHoneycomb = false;
                waitClock.restart(); // Reset the wait clock for a new target
                break;
            }
        }

        // If no previous honeycombs are active, check new honeycombs
        if (targetIndex == -1) {
            for (int i = 0; i < newHoneycombX.size(); ++i) {
                if (newHoneycombActive[i]) {
                    targetIndex = i;
                    isTargetNewHoneycomb = true;
                    waitClock.restart(); // Reset the wait clock for a new target
                    break;
                }
            }
        }
        



        // If no honeycombs are active, move the bird to the middle right
        if (targetIndex == -1) {
            float targetX = resolutionX - 1.5f * boxPixelsX;
            float targetY = resolutionY / 2;

            if (birdX < targetX) birdX += speed;
            if (birdX > targetX) birdX -= speed;
            if (birdY < targetY) birdY += speed;
            if (birdY > targetY) birdY -= speed;
        }
    }
}


void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite) {
    hummingbirdSprite.setPosition(birdX, birdY);
    window.draw(hummingbirdSprite);
}


void resetLevelState() {
    for (int i = 0; i < beeX.size(); i++) {
        beeX[i] = 0;
        beeY[i] = 0;
        beeActive[i] = false;
        beeMovingRight[i] = rand() % 2;
        beeFacingRight[i] = true;
        honeycombX[i] = 0;
        honeycombY[i] = 0;
        honeycombActive[i] = false;
        isGeneratedHoneycomb[i] = false;
        beeStopped[i] = false;
        beeStopClock[i].restart();
        beeResumeClock[i].restart();
    }
    
    // Reset new bees
    for (int i = 0; i < newBeeX.size(); i++) {
        newBeeX[i] = resolutionX - boxPixelsX; // Spawn at the right edge
        newBeeY[i] = 0;                        // Spawn at the top
        newBeeActive[i] = false;
        newBeeMovingRight[i] = false;          // Initially moving left
        newBeeFacingRight[i] = false;          // Initially facing left
        newHoneycombX[i] = 0;
        newHoneycombY[i] = 0;
        newHoneycombActive[i] = false;
        isGeneratedNewHoneycomb[i] = false;    // Reset generated honeycomb
        newBeeStopped[i] = false;
        newBeeStopClock[i].restart();
        newBeeResumeClock[i].restart();
    }


    for (int i = 0; i < randomHoneycombX.size(); i++) {
        randomHoneycombX[i] = 0;
        randomHoneycombY[i] = 0;
        randomHoneycombActive[i] = false;
    }
    
    //reset beehives:
    for (int i = 0; i < maxBeehives; i++) {
        beehiveActive[i] = true;
        beehiveX[i] = rand() % (resolutionX - boxPixelsX);
        beehiveY[i] = rand() % ((gameColumns - 4) * boxPixelsY);
        beehiveSprites[i].setPosition(beehiveX[i], beehiveY[i]);
    }

    initialBeesSpawned = 0;
    totalBeesSpawned = initialBees;
    
    initialNewBees = 5;  // Reset initial new bees to spawn
    totalNewBeesSpawned = 0;
    
    activeBees = initialBees;
    initialBeeSpawnClock.restart();
    delayedBeeSpawnClock.restart();
    initialNewBeeSpawnClock.restart();
    delayedNewBeeSpawnClock.restart();
    
    //reset scoring
    workerBeeHits = 0;
    killerBeeHits = 0;
    std::fill(std::begin(yellowHoneycombsEaten), std::end(yellowHoneycombsEaten), 0);
    std::fill(std::begin(redHoneycombsEaten), std::end(redHoneycombsEaten), 0);
    std::fill(std::begin(activeBeehivesByTier), std::end(activeBeehivesByTier), 0);
    
    

    obstacles.clear();
}

// Add a global variable to track the last direction
bool spawnFromRight = true; // Start with spawning from the right

void spawnNewBees() {
    // Spawn the initial bees
    if (initialNewBees > 0 && initialNewBeeSpawnClock.getElapsedTime().asMilliseconds() > 200) {
        int currentBeeIndex = totalNewBeesSpawned;

        if (currentBeeIndex >= maxNewBees) return; // Ensure within bounds

        newBeeActive[currentBeeIndex] = true;

        // Alternate spawn direction using `spawnFromRight`
        if (spawnFromRight) {
            newBeeX[currentBeeIndex] = resolutionX - boxPixelsX; // Right edge
            newBeeMovingRight[currentBeeIndex] = false;          // Move left
            newBeeFacingRight[currentBeeIndex] = false;          // Face left
        } else {
            newBeeX[currentBeeIndex] = 0; // Left edge
            newBeeMovingRight[currentBeeIndex] = true;           // Move right
            newBeeFacingRight[currentBeeIndex] = true;           // Face right
        }

        newBeeY[currentBeeIndex] = 0; // Spawn at the top
        spawnFromRight = !spawnFromRight; // Toggle direction

        totalNewBeesSpawned++;
        initialNewBees--;
        initialNewBeeSpawnClock.restart();
    }

    // Spawn additional new bees after delay
    if (totalNewBeesSpawned < maxNewBees && delayedNewBeeSpawnClock.getElapsedTime().asSeconds() > 3) {
        int currentBeeIndex = totalNewBeesSpawned;

        if (currentBeeIndex >= maxNewBees) return; // Ensure within bounds

        newBeeActive[currentBeeIndex] = true;

        // Alternate spawn direction using `spawnFromRight`
        if (spawnFromRight) {
            newBeeX[currentBeeIndex] = resolutionX - boxPixelsX; // Right edge
            newBeeMovingRight[currentBeeIndex] = false;          // Move left
            newBeeFacingRight[currentBeeIndex] = false;          // Face left
        } else {
            newBeeX[currentBeeIndex] = 0; // Left edge
            newBeeMovingRight[currentBeeIndex] = true;           // Move right
            newBeeFacingRight[currentBeeIndex] = true;           // Face right
        }

        newBeeY[currentBeeIndex] = 0; // Spawn at the top
        spawnFromRight = !spawnFromRight; // Toggle direction

        totalNewBeesSpawned++;
        delayedNewBeeSpawnClock.restart();
    }
}



void checkNewBeeBulletCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, Texture& honeycombTexture) {
    for (int i = 0; i < newBeeX.size(); i++) {
        if (!newBeeActive[i]) continue;

        // Check collision
        if (bullet_x < newBeeX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > newBeeX[i] &&
            bullet_y < newBeeY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > newBeeY[i]) {

            // Generate new honeycomb
            newHoneycombX[i] = newBeeX[i];
            newHoneycombY[i] = newBeeY[i];
            newHoneycombActive[i] = true;

            // Deactivate bee
            spawnPowerUp(newHoneycombX[i], newHoneycombY[i]); // Drop power-up
            newBeeActive[i] = false;
            bullet_exists = false;
            activeNewBees--;
            break;
        }
    }
}

void moveNewBees(RectangleShape& groundRectangle, Texture& obstacleTexture) {

    for (int i = 0; i < newBeeX.size(); i++) {
        if (!newBeeActive[i]) continue;

        // Stopping logic for new bees
        if (!newBeeStopped[i] && newBeeStopClock[i].getElapsedTime().asSeconds() > 8) {
            newBeeStopped[i] = true;
            newBeeResumeClock[i].restart();
        }
        if (newBeeStopped[i] && newBeeResumeClock[i].getElapsedTime().asSeconds() > 1.5) {
            newBeeStopped[i] = false;
            newBeeStopClock[i].restart();
        }

        // Skip movement if stopped
        if (newBeeStopped[i]) continue;

        // Movement logic
        if (newBeeMovingRight[i]) {
            newBeeX[i] += fastbeeSpeed;
            if (newBeeX[i] + boxPixelsX >= resolutionX) {
                newBeeX[i] = resolutionX - boxPixelsX;
                newBeeY[i] += boxPixelsY;
                newBeeMovingRight[i] = false;
                newBeeFacingRight[i] = false;
            }
        } else {
            newBeeX[i] -= fastbeeSpeed;
            if (newBeeX[i] < 0) {
                newBeeX[i] = 0;
                newBeeY[i] += boxPixelsY;
                newBeeMovingRight[i] = true;
                newBeeFacingRight[i] = true;
            }
        }
    }
}

void drawNewBees(RenderWindow& window, Texture& newBeeTexture) {
    for (int i = 0; i < newBeeX.size(); i++) {
        if (newBeeActive[i]) {
            Sprite newBeeSprite;
            newBeeSprite.setTexture(newBeeTexture);
            if (newBeeFacingRight[i]) {
                newBeeSprite.setScale(-0.7, 0.7); // Facing right
            } else {
                newBeeSprite.setScale(0.7, 0.7);  // Facing left
                newBeeSprite.setPosition(newBeeX[i] + boxPixelsX, newBeeY[i]); // Flip position
            }
            newBeeSprite.setPosition(newBeeX[i], newBeeY[i]);
            window.draw(newBeeSprite);
        }
    }
}

void drawNewHoneycombs(RenderWindow& window) {
    for (int i = 0; i < newHoneycombX.size(); i++) {
        if (newHoneycombActive[i]) {
            Sprite newHoneycombSprite;
            newHoneycombSprite.setTexture(newHoneycombTexture);
            newHoneycombSprite.setScale(0.6, 0.6); // Adjust scale if needed
            newHoneycombSprite.setPosition(newHoneycombX[i], newHoneycombY[i]);
            window.draw(newHoneycombSprite);
        }
    }
}

//Beehives :
void generateBeehives() {
    beehiveX.resize(maxBeehives);
    beehiveY.resize(maxBeehives);
    beehiveActive.resize(maxBeehives, true);
    beehiveSprites.resize(maxBeehives);

    for (int i = 0; i < maxBeehives; i++) {
        beehiveX[i] = rand() % (resolutionX - boxPixelsX); // Random X position
        beehiveY[i] = rand() % ((gameColumns - 4) * boxPixelsY); // Random Y position, above the player's area
        beehiveActive[i] = true;

        // Set up sprite
        beehiveSprites[i].setTexture(beehiveTexture);
        beehiveSprites[i].setScale(0.6f, 0.6f);
        beehiveSprites[i].setPosition(beehiveX[i], beehiveY[i]);
    }
}

void updateBeehives(float& bullet_x, float& bullet_y, bool& bullet_exists) {
    for (int i = 0; i < beehiveX.size(); i++) {
        if (!beehiveActive[i]) continue;

        // Check for collision with bullet
        if (bullet_x < beehiveX[i] + boxPixelsX &&
            bullet_x + boxPixelsX > beehiveX[i] &&
            bullet_y < beehiveY[i] + boxPixelsY &&
            bullet_y + boxPixelsY > beehiveY[i]) {

            // Deactivate the beehive and bullet
            beehiveActive[i] = false;
            bullet_exists = false;
            hitSound.play();

            break; // Only handle one collision per frame
        }
    }
}

void drawBeehives(RenderWindow& window) {
    for (int i = 0; i < beehiveSprites.size(); i++) {
        if (beehiveActive[i]) {
            window.draw(beehiveSprites[i]);
        }
    }
}

//Level complete logic :

bool isLevelComplete() {
    // Check if at least one bee was active at any point
    bool beesSpawned = (initialBeesSpawned > 0 || totalNewBeesSpawned > 0);

    // Ensure level completion logic only applies when bees have spawned
    if (!beesSpawned) return false;

    // Check if all regular bees are inactive
    for (int i = 0; i < maxBees; i++) {
        if (beeActive[i]) return false;
    }

    // Check if all new fast bees are inactive
    for (int i = 0; i < maxNewBees; i++) {
        if (newBeeActive[i]) return false;
    }

    return true; // All bees inactive and level is complete
}

void showTransitionScreen(RenderWindow &window, int nextLevel) {
    Texture transitionTexture;
    if (!transitionTexture.loadFromFile("Textures/transition.png")) {
        cout << "Error loading transition texture!" << endl;
        return;
    }
    
    if (!transitionMusic.openFromFile("Music/magical_journey.ogg")) {
    cout << "Error loading transition music!" << endl;
}
    transitionMusic.setVolume(50); // Set volume
    transitionMusic.setLoop(false); // Play once
    transitionMusic.play();         // Start playing


    Sprite transitionSprite;
    transitionSprite.setTexture(transitionTexture);
    transitionSprite.setScale(
        static_cast<float>(resolutionX) / transitionTexture.getSize().x,
        static_cast<float>(resolutionY) / transitionTexture.getSize().y
    );

    // Display the transition screen for 5 seconds
    Clock transitionClock;
    while (transitionClock.getElapsedTime().asSeconds() < 5) {
        window.clear();
        window.draw(transitionSprite);

        window.display();
    }
    transitionMusic.stop();

}



void advanceToNextLevel(RenderWindow &window) {

    bgMusic.stop(); // Stop the level music
    if (!transitionMusic.openFromFile("Music/magical_journey.ogg")) {
    cout << "Error loading transition music!" << endl;
}
    transitionMusic.setVolume(50); // Set volume
    transitionMusic.setLoop(false); // Play once
    transitionMusic.play();         // Start playing
    
    if (currentLevel >= 4) {
        cout << "Game Complete!" << endl;
        // Show game completion screen
        Texture gameCompleteTexture;
        if (!gameCompleteTexture.loadFromFile("Textures/gameover.png")) {
            cout << "Error loading game complete texture!" << endl;
            return;
        }
        Sprite gameCompleteSprite;
        gameCompleteSprite.setTexture(gameCompleteTexture);
        gameCompleteSprite.setScale(
            static_cast<float>(resolutionX) / gameCompleteTexture.getSize().x,
            static_cast<float>(resolutionY) / gameCompleteTexture.getSize().y
        );

        Clock gameCompleteClock;
        while (gameCompleteClock.getElapsedTime().asSeconds() < 5) {
            window.clear();
            window.draw(gameCompleteSprite);
            window.display();
        }
        // Return to the main menu
        inMenu = true; // Set the flag to return to the menu
        return; // No further levels
    }
    
    
    // Add a 3-second delay before clearing the current screen or showing any transition
    Clock delayClock1;
    while (delayClock1.getElapsedTime().asSeconds() < 5) {
        // Simply do nothing here; avoid clearing the window or drawing anything new
        // This keeps the current frame displayed for 5 seconds
    }
   

    
    // Add 3-second delay before showing the transition screen
    Clock delayClock;
    while (delayClock.getElapsedTime().asSeconds() < 3) {
        // Keep rendering the current game frame or display a message
        window.clear();
        // Optional: Show a message like "Preparing Next Level..."
        Font font;
        if (font.loadFromFile("Fonts/arial.ttf")) {
            Text preparingText;
            preparingText.setFont(font);
            preparingText.setString("Preparing Next Level...");
            preparingText.setCharacterSize(50);
            preparingText.setFillColor(Color::White);

            // Center the text
            FloatRect textBounds = preparingText.getLocalBounds();
            preparingText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            preparingText.setPosition(resolutionX / 2, resolutionY / 2);

            window.draw(preparingText);
        }
        window.display();
    }

    transitionMusic.stop();
    currentLevel++; // Increment to the next level
    showTransitionScreen(window, currentLevel);

    // Reset the game state and start the next level
    resetLevelState();
    switch (currentLevel) {
        case 2:
            playLevel2(window);
            break;
        case 3:
            playLevel3(window);
            break;
        case 4:
            playBossLevel(window);
            break;
        default:
            cout << "Unknown level!" << endl;
            break;
    }
}

//player reset state after death :
void resetPlayerState() {
    playerLives = 3; // Reset player lives
    playerDied = false; // Reset player death flag
    showPlayerDiedMessage = false; // Reset player died message flag
    lifeSprites.clear(); // Clear existing life sprites

    // Reinitialize life sprites
    for (int i = 0; i < playerLives - 1; i++) {
        Sprite lifeSprite;
        lifeSprite.setTexture(lifeTexture);
        lifeSprite.setScale(0.6f, 0.6f); // Scale down the sprite
        lifeSprite.setPosition(10 + i * 40, resolutionY - 50); // Position sprites
        lifeSprites.push_back(lifeSprite);
    }
}

//scoring syestem

// Function to load leaderboard from file
void loadLeaderboard() {
    leaderboard.clear();
    std::ifstream file(leaderboardFile);
    if (!file.is_open()) return;

    std::string name;
    int score;
    while (file >> name >> score) {
        leaderboard.emplace_back(name, score);
    }
    file.close();

    // Sort leaderboard in descending order of scores
    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
}

// Function to save leaderboard to file
void saveLeaderboard() {
    std::ofstream file(leaderboardFile);
    if (!file.is_open()) return;

    for (const auto& entry : leaderboard) {
        file << entry.first << " " << entry.second << "\n";
    }
    file.close();
}

// Function to update leaderboard with new score
void updateLeaderboard(const std::string& name, int score) {
    leaderboard.emplace_back(name, score);
    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Keep top 10 scores only
    if (leaderboard.size() > 10) {
        leaderboard.resize(10);
    }

    saveLeaderboard();
}


// Function to render the leaderboard
void renderLeaderboard(RenderWindow& window, const Font& font) {
    // Title for the leaderboard
    Text leaderboardTitle("Leaderboard", font, 40);
    leaderboardTitle.setFillColor(Color::Yellow);
    leaderboardTitle.setPosition(resolutionX - 900, 110); // Adjust as needed
    window.draw(leaderboardTitle);

    // Display top 10 scores
    int maxDisplayScores = 10;
    int yOffset = 165; // Vertical offset for positioning
    for (size_t i = 0; i < leaderboard.size() && i < maxDisplayScores; ++i) {
        // Format: "1. PlayerName: Score"
        std::string entryText = std::to_string(i + 1) + ". " + leaderboard[i].first + ": " + std::to_string(leaderboard[i].second);

        Text entry(entryText, font, 20);
        entry.setFillColor(Color::White);
        entry.setPosition(resolutionX - 900, yOffset); // Adjust position as needed
        window.draw(entry);

        yOffset += 30; // Increment y-position for next entry
    }
}


// Function to get player name
void getPlayerName(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        std::cout << "Error loading font!" << std::endl;
        return;
    }

    Text prompt("Enter your name: ", font, 30);
    prompt.setFillColor(Color::White);
    prompt.setPosition(50, resolutionY / 2 - 50);

    std::string input;
    Text inputText("", font, 30);
    inputText.setFillColor(Color::White);
    inputText.setPosition(50, resolutionY / 2);

    while (true) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                    input += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                playerName = input;
                return;
            }
        }

        window.clear();
        window.draw(prompt);

        inputText.setString(input);
        window.draw(inputText);
        window.display();
    }
}

// Function to display player's name and score at the end of the level
void displayEndLevelStats(RenderWindow& window, const std::string& playerName, int playerScore) { 
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        std::cout << "Error loading font!" << std::endl;
        return;
    }

    Text title("Level Complete!", font, 50);
    title.setFillColor(Color::Yellow);
    title.setStyle(Text::Bold);
    title.setPosition(resolutionX / 2 - title.getGlobalBounds().width / 2, 50);

    std::vector<std::string> stats = {
        "Player: " + playerName,
        "Score: " + std::to_string(playerScore),
        "Worker Bees Hit: " + std::to_string(workerBeeHits),
        "Killer Bees Hit: " + std::to_string(killerBeeHits),
        "Yellow Honeycombs Eaten: Top " + std::to_string(yellowHoneycombsEaten[0]) + ", Middle " + std::to_string(yellowHoneycombsEaten[1]) + ", Bottom " + std::to_string(yellowHoneycombsEaten[2]),
        "Red Honeycombs Eaten: Top " + std::to_string(redHoneycombsEaten[0]) + ", Middle " + std::to_string(redHoneycombsEaten[1]) + ", Bottom " + std::to_string(redHoneycombsEaten[2]),
        "Active Beehives: Top " + std::to_string(activeBeehivesByTier[0]) + ", Middle " + std::to_string(activeBeehivesByTier[1]) + ", Bottom " + std::to_string(activeBeehivesByTier[2])
    };

    std::vector<Text> statTexts;
    for (size_t i = 0; i < stats.size(); ++i) {
        Text stat(stats[i], font, 30);
        stat.setFillColor(Color::White);
        stat.setPosition(50, 150 + i * 30 + 15);
        statTexts.push_back(stat);
    }

    Clock displayClock;
    while (displayClock.getElapsedTime().asSeconds() < 5.0f) {
        window.clear();
        window.draw(title);
        for (const auto& statText : statTexts) {
            window.draw(statText);
        }
        window.display();
    }
}



void updateScoreForBeeHit(bool isKillerBee, float beeY) {
    int points = isKillerBee ? 1000 : 100;
    playerScore += points;

    if (isKillerBee)
        killerBeeHits++;
    else
        workerBeeHits++;
}

void updateScoreForHoneycombEaten(bool isRed, float honeycombY) {
    int tier = honeycombY / tierHeight;

    if (tier <= 1) {
        playerScore += isRed ? 2000 : 1000;
        (isRed ? redHoneycombsEaten[0] : yellowHoneycombsEaten[0])++;
    } else if (tier <= 4) {
        playerScore += isRed ? 1800 : 800;
        (isRed ? redHoneycombsEaten[1] : yellowHoneycombsEaten[1])++;
    } else {
        playerScore += isRed ? 1500 : 500;
        (isRed ? redHoneycombsEaten[2] : yellowHoneycombsEaten[2])++;
    }
}

void updateScoreForBeehive(float beehiveY) {
    int tier = beehiveY / tierHeight;

    if (tier <= 1) {
        playerScore += 2000;
        activeBeehivesByTier[0]++;
    } else if (tier <= 4) {
        playerScore += 1600;
        activeBeehivesByTier[1]++;
    } else {
        playerScore += 1000;
        activeBeehivesByTier[2]++;
    }
}

void checkForBonus(RenderWindow &window) {
    if (nextBonusIndex < 3 && playerScore >= bonusPoints[nextBonusIndex]) {
        playerLives++; // Increment player lives
        nextBonusIndex++;

        // Add a new life sprite dynamically
        Sprite lifeSprite;
        lifeSprite.setTexture(lifeTexture);
        lifeSprite.setScale(0.6f, 0.6f); // Adjust scale as needed
        lifeSprite.setPosition(10 + (playerLives - 2) * 40, resolutionY - 50); // Position next life sprite
        lifeSprites.push_back(lifeSprite);

        // Optional: Display a message indicating the bonus life
        Text bonusText;
        Font font;
        if (font.loadFromFile("Fonts/arial.ttf")) {
            bonusText.setFont(font);
            bonusText.setString("Bonus Life Awarded!");
            bonusText.setCharacterSize(30);
            bonusText.setFillColor(Color::Blue);
            FloatRect textBounds = bonusText.getLocalBounds();
            bonusText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            bonusText.setPosition(resolutionX / 2, resolutionY / 2);

            Clock bonusClock;
            while (bonusClock.getElapsedTime().asSeconds() < 1) {
                window.draw(bonusText);
                window.display();
            }
        }
    }
}


//powerup functions :
void spawnPowerUp(float x, float y) {
    PowerUp powerUp;
    int type = rand() % 4; // Randomly select power-up type

    switch (type) {
        
        case HEIGHT_DEC:
            powerUp.type = HEIGHT_DEC;
            powerUp.sprite.setTexture(heightDecTexture);
            break;
    }

    powerUp.position = Vector2f(x, y); // Start at the honeycomb or bird-eating location
    powerUp.sprite.setPosition(powerUp.position);
    powerUp.active = true;
    powerUp.grounded = false;
    powerUps.push_back(powerUp);
}

void updatePowerUps() {
    float groundY = resolutionY - 2 * boxPixelsY - 32; // Ground position

    for (auto& powerUp : powerUps) {
        if (!powerUp.active || powerUp.grounded) continue;

        // Move the power-up down
        powerUp.position.y += powerUp.velocityY;

        // Check if it has reached the ground
        if (powerUp.position.y >= groundY) {
            powerUp.position.y = groundY;
            powerUp.grounded = true; // Mark as grounded
        }

        powerUp.sprite.setPosition(powerUp.position); // Update sprite position
    }
}

void checkPlayerPowerUpCollision(Sprite& playerSprite) {
    for (auto& powerUp : powerUps) {
        if (!powerUp.active || !powerUp.grounded) continue;

        // Check if the player intersects the power-up
        if (playerSprite.getGlobalBounds().intersects(powerUp.sprite.getGlobalBounds())) {
            applyPowerUp(powerUp.type); // Activate power-up effect
            powerUp.active = false;    // Deactivate power-up
        }
    }
}


void applyPowerUp(PowerUpType type) {
    powerUpActive = true;
    powerUpTimer.restart();
    currentPowerUpType = type;

    // Get the player's current bounding box
    FloatRect playerBounds = playerSprite.getGlobalBounds();

    // Calculate the player's current bottom position (Y-coordinate)
    float currentBottom = playerBounds.top + playerBounds.height;

    switch (type) {

        case HEIGHT_DEC:
            playerSprite.setScale(1.0f, 0.5f); // Half height
            // Leave HEIGHT_DEC behavior as-is (no adjustment)
            break;
    }
}




void levelSetUp(RenderWindow &window ,string music , string background){

    initializeLevel(maxBees, maxRandomHoneycombs,maxNewBees);
    
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }
    
    Text scoreText("Score: 0", font, 30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(resolutionX - 200, resolutionY - 50);
    
    bool exitLevel = false; // Flag for exiting the level

    // Initializing Background Music.
    if (!bgMusic.openFromFile(music)) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();
    
    
    //initialising background
    if (!backgroundTexture.loadFromFile(background)) {
    cout << "Error loading background image!" << endl;
	}

	// Set the texture to the sprite
	backgroundSprite.setTexture(backgroundTexture);

	// Optionally scale the background to fit the window if needed
	backgroundSprite.setScale(
	    static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
	    static_cast<float>(resolutionY) / backgroundTexture.getSize().y
	);
	


    // Initializing Player and Player Sprites.
    float player_x = ((gameRows / 2) * boxPixelsX);
    float player_y = ((gameColumns - 4) * boxPixelsY);

    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/spray.png");
    playerSprite.setTexture(playerTexture);
    
    //bullet sfx :
    if (!fireSoundBuffer.loadFromFile("Sound Effects/fire.wav")) {
	    cout << "Error loading fire.wav!" << endl;
	}
	fireSound.setBuffer(fireSoundBuffer);
	fireSound.setVolume(60); 
    if (!hitBuffer.loadFromFile("Sound Effects/hit.wav")) {
	    cout << "Error loading hit sound!" << endl;
	}
	hitSound.setBuffer(hitBuffer);

    
        // Load life sprite texture
	if (!lifeTexture.loadFromFile("Textures/spray.png")) {
	    cout << "Error loading life sprite texture!" << endl;
	}

	// Initialize life sprites
	lifeSprites.clear();
	for (int i = 0; i < playerLives - 1; i++) { // Display 2 lives (extra ones)
	    Sprite lifeSprite;
	    lifeSprite.setTexture(lifeTexture);
	    lifeSprite.setScale(0.6f, 0.6f); 
	    lifeSprite.setPosition(10 + i * 40, resolutionY - 50); // Position sprites
	    lifeSprites.push_back(lifeSprite);
	}
	
	
    if (!playerDiedFont.loadFromFile("Fonts/arial.ttf")) { 
    cout << "Error loading player died font!" << endl;
	}

	playerDiedText.setFont(playerDiedFont);
	playerDiedText.setString("Player Died!");
	playerDiedText.setCharacterSize(50); 
	playerDiedText.setFillColor(Color::Red);
	playerDiedText.setStyle(Text::Bold);

	FloatRect textBounds = playerDiedText.getLocalBounds();
	playerDiedText.setOrigin(textBounds.width / 2, textBounds.height / 2);
	playerDiedText.setPosition(resolutionX / 2, resolutionY / 2);


    
    // Initializing Bullet and Bullet Sprites
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(1, 1);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Worker bees loading and initialization
    Texture beeTexture;
    if (!beeTexture.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Error loading bee texture!" << endl;
    }
    
     // New Fast Killer Bee Initialization
    Texture newBeeTexture;
    if (!newBeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading new bee texture!" << endl;
    }
    
    // Initialize bees
    for (int i = 0; i < maxBees; i++) {
        beeX[i] = 0; // Initially placing all bees at top-left
        beeY[i] = 0;
        beeActive[i] = false; // Set all bees inactive initially
        beeMovingRight[i] = rand() % 1;
        beeFacingRight[i] = true; // Default to facing right
        honeycombX[i] = 0;
        honeycombY[i] = 0;
        honeycombActive[i] = false;
    }
    
    // Initialize new Fast bees
    for (int i = 0; i < maxNewBees; i++) {
        newBeeX[i] = resolutionX - boxPixelsX; // Spawn at the right edge
        newBeeY[i] = 0;                        // Spawn at the top
        newBeeActive[i] = false;
        newBeeMovingRight[i] = false;          // Initially moving left
        newBeeFacingRight[i] = false;          // Initially facing left
    }
    
    // Honeycomb texture
    Texture honeycombTexture;
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
    	cout << "Error loading honeycomb texture!" << endl;
    }
    
    // Load new honeycomb texture
	if (!newHoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
	    cout << "Error loading new honeycomb texture!" << endl;
	}


    // Initial honeycomb 
    generateInitialHoneycombs();
    
    
    //initialising obstacles
    if (!obstacleTexture.loadFromFile("Textures/obstacles.png")) {
    cout << "Error loading obstacle texture!" << endl;
    }
    
    //behive texture
    if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
    cout << "Error loading hive texture!" << endl;
    }
    generateBeehives();

    
    //humming bird:
    initializeHummingbird(hummingbirdTexture, hummingbirdSprite);
    
    // The ground on which player moves
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    bool canFire = true; // this variable to controls firing(fire when space bar released)

    while (window.isOpen() && !exitLevel) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                exitLevel = true; // Exit level and return to menu
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                bgMusic.stop(); 
                exitLevel = true; // Return to menu when Escape is pressed
            }
        }


        // Spawn the first five bees one by one
        if (initialBeesSpawned < initialBees && initialBeeSpawnClock.getElapsedTime().asMilliseconds() > 800) {
            beeActive[initialBeesSpawned] = true;    // Activate the bee
            beeX[initialBeesSpawned] = 0;           // Spawn at the top-left
            beeY[initialBeesSpawned] = 0;           // At the top
            beeMovingRight[initialBeesSpawned] = true;  // Set movement direction
            beeFacingRight[initialBeesSpawned] = true;  // Ensure it faces right
            initialBeesSpawned++;                   // Increment the counter
            initialBeeSpawnClock.restart();         // Reset the clock
        }
        // Spawn additional bees after a delay of 2.5 seconds
	if (totalBeesSpawned < maxBees && delayedBeeSpawnClock.getElapsedTime().asSeconds() >= 2.5) {
	    beeActive[totalBeesSpawned] = true;     // Activate the bee
	    beeX[totalBeesSpawned] = 0;             // Spawn at the top-left
	    beeY[totalBeesSpawned] = 0;             // At the top
	    beeMovingRight[totalBeesSpawned] = rand() % 2; // Random direction
	    beeFacingRight[totalBeesSpawned] = beeMovingRight[totalBeesSpawned]; // Set direction
	    totalBeesSpawned++;                     // Increment the total spawned counter
	    delayedBeeSpawnClock.restart();         // Reset the spawn delay clock
	}
	
	// Spawn initial new Fast bees
	if (totalNewBeesSpawned < initialNewBees && initialNewBeeSpawnClock.getElapsedTime().asMilliseconds() > 700) {
	    spawnNewBees(); 
	}

	// Spawn additional new Fast bees after delay
	if (totalNewBeesSpawned < maxNewBees && delayedNewBeeSpawnClock.getElapsedTime().asSeconds() >= 3) {
	    spawnNewBees(); 
	}

        //Player Lives Syestem :
        
        if (playerDied) {
	    playerLives--; // Decrease lives
	    
	    // Trigger the "Player Died!" message
	    showPlayerDiedMessage = true;
	    playerDiedClock.restart();

	    if (playerLives > 0) {
		// Reset the level state
		 obstacles.clear();

		// Update life sprites
		lifeSprites.pop_back();

		// Reset player position
		player_x = ((gameRows / 2) * boxPixelsX);
		player_y = ((gameColumns - 4) * boxPixelsY);

		// Reset playerDied flag
		playerDied = false;
	    } else {
		// Game Over Logic
		bgMusic.stop(); // Stop background music
		gameOverClock.restart(); // Start the clock for game over screen

		// Game Over Screen
		while (gameOverClock.getElapsedTime().asSeconds() < 3) {
		    window.clear();

		    // Display custom game over screen
		    Texture gameOverTexture;
		    Sprite gameOverSprite;
		    if (gameOverTexture.loadFromFile("Textures/gameover.jpg")) {
		        gameOverSprite.setTexture(gameOverTexture);
		        gameOverSprite.setScale(
		            static_cast<float>(resolutionX) / gameOverTexture.getSize().x,
		            static_cast<float>(resolutionY) / gameOverTexture.getSize().y
		        );
		        window.draw(gameOverSprite);
		    }
		    window.display();
		}

		// Return to main menu
		resetPlayerState(); // Reset lives, flags, and sprites
	        inMenu = true;      // Set the state to return to the main menu
	        return;             // Exit the current level loop
	    }
	}
        
        // Draw the background
        window.draw(backgroundSprite);
        setFixedBackgroundBrightness(backgroundSprite, 0.3f); // Set brightness to 70%

        // Handle collisions for new bees with bullets
	if (bullet_exists) {
	    checkBulletNewBeeCollision(bullet_x, bullet_y, bullet_exists, honeycombTexture);
	}
        // Check for collisions between bees and the player
	checkBeePlayerCollision(player_x, player_y, playerSprite);


        // Handle player movement
        // Call movePlayer with obstacles vector passed as an argument
	movePlayer(player_x, bulletClock, playerSprite, obstacles);


            // Handle bullet firing
            // Handle bullet firing
	    if (Keyboard::isKeyPressed(Keyboard::Space) && canFire) {
		fireBullet(player_x, player_y, bullet_x, bullet_y, bullet_exists);
		canFire = false; // Prevent firing until the key is released
	    }

	    // Reset canFire when space is released
	    if (!Keyboard::isKeyPressed(Keyboard::Space)) {
		canFire = true;
	    }

        if (bullet_exists == true) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        } else {
            bullet_x = player_x;
            bullet_y = player_y;
        }
        //Honey comb collssion with bullet:
        if (bullet_exists) {
	    moveBullet(bullet_y, bullet_exists, bulletClock);
	    drawBullet(window, bullet_x, bullet_y, bulletSprite);
	    checkBulletHoneycombCollision(bullet_x, bullet_y, bullet_exists); 
	} else {
	    bullet_x = player_x;
	    bullet_y = player_y;
	}
	
	// Check collision of bullet with random honeycombs
	if (bullet_exists) {
	    checkBulletRandomHoneycombCollision(bullet_x, bullet_y, bullet_exists);
	    updateBeehives(bullet_x, bullet_y, bullet_exists);
	}



        drawPlayer(window, player_x, player_y, playerSprite);

        // Handle bee movement
        moveBees(groundRectangle, obstacleTexture);
        
        // New bee updates
        moveNewBees(groundRectangle, obstacleTexture); 
        drawNewBees(window, newBeeTexture);           



        // Check bullet collision with bees
        if (bullet_exists) {
            checkBulletCollision(bullet_x, bullet_y, bullet_exists, honeycombTexture);
        }
        
        // Check if the level is complete
        if (isLevelComplete()) {
            //beehive scoring
            for (int i = 0; i < maxBeehives; i++) {
		    if (beehiveActive[i]) {
			updateScoreForBeehive(beehiveY[i]); // Pass the Y position of the active beehive
		    }
	    }

            // Display player's name and score
            displayEndLevelStats(window, playerName, playerScore);
            advanceToNextLevel(window);
            return; // Exit the current level loop
        }

        // Draw bees
        drawBees(window, beeTexture);
        drawHoneycombs(window, honeycombTexture);
        // Draw honeycombs from new bees
	drawNewHoneycombs(window);
        // Draw random honeycombs
	drawRandomHoneycombs(window, honeycombTexture);
	// Draw obstacles
	drawObstacles(window, obstacleTexture, obstacles);
	
	// Spawn obstacles when new bees hit the ground
	for (int i = 0; i < maxNewBees; i++) {
	    if (newBeeActive[i] && newBeeY[i] >= resolutionY - 2 * boxPixelsY) {
		spawnObstacle(newBeeX[i], newBeeY[i], player_x, player_y, obstacles);
		spawnObstacle(newBeeX[i], newBeeY[i], player_x, player_y, obstacles);
		newBeeActive[i] = false; // Deactivate the new bee
	    }
	}
	// Spawn obstacles when bees hit the ground
	for (int i = 0; i < maxBees; i++) {
	    if (beeActive[i] && beeY[i] >= resolutionY - 2 * boxPixelsY) {
		spawnObstacle(beeX[i], beeY[i], player_x, player_y, obstacles);
		beeActive[i] = false; // Deactivate the bee
	    }
	}
	
	// Update hummingbird animation
	updateHummingbirdAnimation(hummingbirdSprite, hummingbirdAnimationClock, hummingbirdCurrentFrame);

	// Move hummingbird
	moveHummingbird(birdX, birdY, hummingbirdSpeed,honeycombX, honeycombY, honeycombActive,newHoneycombX, newHoneycombY, newHoneycombActive,targetHoneycombIndex, isTargetNewHoneycomb, honeycombWaitClock);

	// Draw hummingbird
	drawHummingbird(window, hummingbirdSprite);
	//draw beehives
	drawBeehives(window);
        window.draw(groundRectangle);
        
        // Draw remaining lives
	for (const auto& lifeSprite : lifeSprites) {
	    window.draw(lifeSprite);
	}
	
	// Check if "Player Died!" message should be displayed
	if (showPlayerDiedMessage) {
	    if (playerDiedClock.getElapsedTime().asSeconds() < 1) {
		window.draw(playerDiedText); // Render the text
	    } else {
		showPlayerDiedMessage = false; // Stop displaying after 1 second
	    }
	}
	
	checkForBonus(window); // Check for bonus lives periodically
	// Update score dynamically
        scoreText.setString("Score: " + std::to_string(playerScore));
        window.draw(scoreText);
        window.display();
        window.clear();
    }
    updateLeaderboard(playerName, playerScore); // Save score after level ends


}

void BosslevelSetUp(RenderWindow &window ,string music , string background){

    initializeLevel(maxBees, maxRandomHoneycombs,maxNewBees);
    
    Font font;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }
    
    Text scoreText("Score: 0", font, 30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(resolutionX - 200, resolutionY - 50);
    
    bool exitLevel = false; // Flag for exiting the level

    // Initializing Background Music.
    if (!bgMusic.openFromFile(music)) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(50);
    bgMusic.setLoop(true);
    bgMusic.play();
    
    
    //initialising background
    if (!backgroundTexture.loadFromFile(background)) {
    cout << "Error loading background image!" << endl;
	}

	// Set the texture to the sprite
	backgroundSprite.setTexture(backgroundTexture);

	// Optionally scale the background to fit the window if needed
	backgroundSprite.setScale(
	    static_cast<float>(resolutionX) / backgroundTexture.getSize().x,
	    static_cast<float>(resolutionY) / backgroundTexture.getSize().y
	);


    // Initializing Player and Player Sprites.
    float player_x = ((gameRows / 2) * boxPixelsX);
    float player_y = ((gameColumns - 4) * boxPixelsY);

    Texture playerTexture;
    playerTexture.loadFromFile("Textures/spray.png");
    playerSprite.setTexture(playerTexture);
    
    //bullet sfx :
    if (!fireSoundBuffer.loadFromFile("Sound Effects/fire.wav")) {
	    cout << "Error loading fire.wav!" << endl;
	}
	fireSound.setBuffer(fireSoundBuffer);
	fireSound.setVolume(60); 
    if (!hitBuffer.loadFromFile("Sound Effects/hit.wav")) {
	    cout << "Error loading hit sound!" << endl;
	}
	hitSound.setBuffer(hitBuffer);

    
        // Load life sprite texture
	if (!lifeTexture.loadFromFile("Textures/spray.png")) {
	    cout << "Error loading life sprite texture!" << endl;
	}

	// Initialize life sprites
	lifeSprites.clear();
	for (int i = 0; i < playerLives - 1; i++) { // Display 2 lives (extra ones)
	    Sprite lifeSprite;
	    lifeSprite.setTexture(lifeTexture);
	    lifeSprite.setScale(0.6f, 0.6f); 
	    lifeSprite.setPosition(10 + i * 40, resolutionY - 50); // Position sprites
	    lifeSprites.push_back(lifeSprite);
	}
	
	
    if (!playerDiedFont.loadFromFile("Fonts/arial.ttf")) { 
    cout << "Error loading player died font!" << endl;
	}

	playerDiedText.setFont(playerDiedFont);
	playerDiedText.setString("Player Died!");
	playerDiedText.setCharacterSize(50); 
	playerDiedText.setFillColor(Color::Red);
	playerDiedText.setStyle(Text::Bold);

	FloatRect textBounds = playerDiedText.getLocalBounds();
	playerDiedText.setOrigin(textBounds.width / 2, textBounds.height / 2);
	playerDiedText.setPosition(resolutionX / 2, resolutionY / 2);


    
    // Initializing Bullet and Bullet Sprites
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(1, 1);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    
    
    // Loading power-up texture
    if (!heightDecTexture.loadFromFile("Sprites/Height_Dec.png")) {
    	cout << "Error loading Height_Dec.png!" << endl;
    }


    // Worker bees loading and initialization
    Texture beeTexture;
    if (!beeTexture.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Error loading bee texture!" << endl;
    }
    
     // New Fast Killer Bee Initialization
    Texture newBeeTexture;
    if (!newBeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading new bee texture!" << endl;
    }
    
    // Initialize bees
    for (int i = 0; i < maxBees; i++) {
        beeX[i] = 0; // Initially placing all bees at top-left
        beeY[i] = 0;
        beeActive[i] = false; // Set all bees inactive initially
        beeMovingRight[i] = rand() % 1;
        beeFacingRight[i] = true; // Default to facing right
        honeycombX[i] = 0;
        honeycombY[i] = 0;
        honeycombActive[i] = false;
    }
    
    // Initialize new Fast bees
    for (int i = 0; i < maxNewBees; i++) {
        newBeeX[i] = resolutionX - boxPixelsX; // Spawn at the right edge
        newBeeY[i] = 0;                        // Spawn at the top
        newBeeActive[i] = false;
        newBeeMovingRight[i] = false;          // Initially moving left
        newBeeFacingRight[i] = false;          // Initially facing left
    }
    
    // Honeycomb texture
    Texture honeycombTexture;
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
    	cout << "Error loading honeycomb texture!" << endl;
    }
    
    // Load new honeycomb texture
	if (!newHoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
	    cout << "Error loading new honeycomb texture!" << endl;
	}


    // Initial honeycomb 
    generateInitialHoneycombs();
    
    
    //initialising obstacles
    if (!obstacleTexture.loadFromFile("Textures/obstacles.png")) {
    cout << "Error loading obstacle texture!" << endl;
    }
    
    //behive texture
    if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
    cout << "Error loading hive texture!" << endl;
    }
    generateBeehives();

    
    //humming bird:
    initializeHummingbird(hummingbirdTexture, hummingbirdSprite);
    
    // The ground on which player moves
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    bool canFire = true; // this variable to controls firing(fire when space bar released)

    while (window.isOpen() && !exitLevel) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                exitLevel = true; // Exit level and return to menu
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                bgMusic.stop(); 
                exitLevel = true; // Return to menu when Escape is pressed
            }
        }


        // Spawn the first five bees one by one
        if (initialBeesSpawned < initialBees && initialBeeSpawnClock.getElapsedTime().asMilliseconds() > 800) {
            beeActive[initialBeesSpawned] = true;    // Activate the bee
            beeX[initialBeesSpawned] = 0;           // Spawn at the top-left
            beeY[initialBeesSpawned] = 0;           // At the top
            beeMovingRight[initialBeesSpawned] = true;  // Set movement direction
            beeFacingRight[initialBeesSpawned] = true;  // Ensure it faces right
            initialBeesSpawned++;                   // Increment the counter
            initialBeeSpawnClock.restart();         // Reset the clock
        }
        // Spawn additional bees after a delay of 2.5 seconds
	if (totalBeesSpawned < maxBees && delayedBeeSpawnClock.getElapsedTime().asSeconds() >= 1) {
	    beeActive[totalBeesSpawned] = true;     // Activate the bee
	    beeX[totalBeesSpawned] = 0;             // Spawn at the top-left
	    beeY[totalBeesSpawned] = 0;             // At the top
	    beeMovingRight[totalBeesSpawned] = rand() % 2; // Random direction
	    beeFacingRight[totalBeesSpawned] = beeMovingRight[totalBeesSpawned]; // Set direction
	    totalBeesSpawned++;                     // Increment the total spawned counter
	    delayedBeeSpawnClock.restart();         // Reset the spawn delay clock
	}
	
	// Spawn initial new Fast bees
	if (totalNewBeesSpawned < initialNewBees && initialNewBeeSpawnClock.getElapsedTime().asMilliseconds() > 700) {
	    spawnNewBees(); 
	}

	// Spawn additional new Fast bees after delay
	if (totalNewBeesSpawned < maxNewBees && delayedNewBeeSpawnClock.getElapsedTime().asSeconds() >= 2) {
	    spawnNewBees(); 
	}

        //Player Lives Syestem :
        
        if (playerDied) {
	    playerLives--; // Decrease lives
	    
	    // Trigger the "Player Died!" message
	    showPlayerDiedMessage = true;
	    playerDiedClock.restart();

	    if (playerLives > 0) {
		// Reset the level state
		 obstacles.clear();

		// Update life sprites
		lifeSprites.pop_back();

		// Reset player position
		player_x = ((gameRows / 2) * boxPixelsX);
		player_y = ((gameColumns - 4) * boxPixelsY);

		// Reset playerDied flag
		playerDied = false;
	    } else {
		// Game Over Logic
		bgMusic.stop(); // Stop background music
		gameOverClock.restart(); // Start the clock for game over screen

		// Game Over Screen
		while (gameOverClock.getElapsedTime().asSeconds() < 3) {
		    window.clear();

		    // Display custom game over screen
		    Texture gameOverTexture;
		    Sprite gameOverSprite;
		    if (gameOverTexture.loadFromFile("Textures/gameover.jpg")) {
		        gameOverSprite.setTexture(gameOverTexture);
		        gameOverSprite.setScale(
		            static_cast<float>(resolutionX) / gameOverTexture.getSize().x,
		            static_cast<float>(resolutionY) / gameOverTexture.getSize().y
		        );
		        window.draw(gameOverSprite);
		    }
		    window.display();
		}

		// Return to main menu
		resetPlayerState(); // Reset lives, flags, and sprites
	        inMenu = true;      // Set the state to return to the main menu
	        return;             // Exit the current level loop
	    }
	}
        
        // Draw the background
        window.draw(backgroundSprite);
        setFixedBackgroundBrightness(backgroundSprite, 0.3f); // Set brightness to 70%

        // Handle collisions for new bees with bullets
	if (bullet_exists) {
	    checkBulletNewBeeCollision(bullet_x, bullet_y, bullet_exists, honeycombTexture);
	}
        // Check for collisions between bees and the player
	checkBeePlayerCollision(player_x, player_y, playerSprite);


        // Handle player movement
        // Call movePlayer with obstacles vector passed as an argument
	movePlayer(player_x, bulletClock, playerSprite, obstacles);


            // Handle bullet firing
            // Handle bullet firing
	    if (Keyboard::isKeyPressed(Keyboard::Space) && canFire) {
		fireBullet(player_x, player_y, bullet_x, bullet_y, bullet_exists);
		canFire = false; // Prevent firing until the key is released
	    }

	    // Reset canFire when space is released
	    if (!Keyboard::isKeyPressed(Keyboard::Space)) {
		canFire = true;
	    }

        if (bullet_exists == true) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        } else {
            bullet_x = player_x;
            bullet_y = player_y;
        }
        //Honey comb collssion with bullet:
        if (bullet_exists) {
	    moveBullet(bullet_y, bullet_exists, bulletClock);
	    drawBullet(window, bullet_x, bullet_y, bulletSprite);
	    checkBulletHoneycombCollision(bullet_x, bullet_y, bullet_exists); 
	} else {
	    bullet_x = player_x;
	    bullet_y = player_y;
	}
	
	// Check collision of bullet with random honeycombs
	if (bullet_exists) {
	    checkBulletRandomHoneycombCollision(bullet_x, bullet_y, bullet_exists);
	    updateBeehives(bullet_x, bullet_y, bullet_exists);
	}

        updatePowerUps();


        drawPlayer(window, player_x, player_y, playerSprite);
        
        checkPlayerPowerUpCollision(playerSprite);


        // Handle bee movement
        moveBees(groundRectangle, obstacleTexture);
        
        // New bee updates
        moveNewBees(groundRectangle, obstacleTexture); 
        drawNewBees(window, newBeeTexture);           



        // Check bullet collision with bees
        if (bullet_exists) {
            checkBulletCollision(bullet_x, bullet_y, bullet_exists, honeycombTexture);
        }
        
        // Check if the level is complete
        if (isLevelComplete()) {
            //beehive scoring
            for (int i = 0; i < maxBeehives; i++) {
		    if (beehiveActive[i]) {
			updateScoreForBeehive(beehiveY[i]); // Pass the Y position of the active beehive
		    }
	    }
	    
	    updateLeaderboard(playerName, playerScore);
    	    saveLeaderboard(); // Save the updated leaderboard to the f

            // Display player's name and score
            displayEndLevelStats(window, playerName, playerScore);
            advanceToNextLevel(window);
            return; // Exit the current level loop
        }
        
        if (powerUpActive && powerUpTimer.getElapsedTime().asSeconds() > 5.0f) {
	    powerUpActive = false;

	    // Reset player speed and size
	    if (currentPowerUpType == SPEED_INC || currentPowerUpType == SPEED_DEC) {
		playerSpeed = defaultPlayerSpeed; // Reset to default speed
	    }
	    if (currentPowerUpType == HEIGHT_INC || currentPowerUpType == HEIGHT_DEC) {
		// Keep the player's bottom level with the ground when resetting height
		FloatRect playerBounds = playerSprite.getGlobalBounds();
		float currentBottom = playerBounds.top + playerBounds.height;

		playerSprite.setScale(1.0f, 1.0f); // Reset to normal size

		playerBounds = playerSprite.getGlobalBounds(); // Update bounding box after scaling
		float newBottom = playerBounds.top + playerBounds.height;
		float offset = currentBottom - newBottom;
		playerSprite.move(0, offset); // Adjust the vertical position
	    }
	}
	
	if (powerUpActive) {
	    float timeLeft = 5.0f - powerUpTimer.getElapsedTime().asSeconds(); // Calculate remaining time
	    powerUpTimerText.setString("Power-Up: " + std::to_string(static_cast<int>(timeLeft)) + "s");
	}



        
        
        for (const auto& powerUp : powerUps) {
	    if (powerUp.active) {
		window.draw(powerUp.sprite);
	    }
	}

        // Draw bees
        drawBees(window, beeTexture);
        drawHoneycombs(window, honeycombTexture);
        // Draw honeycombs from new bees
	drawNewHoneycombs(window);
        // Draw random honeycombs
	drawRandomHoneycombs(window, honeycombTexture);
	// Draw obstacles
	drawObstacles(window, obstacleTexture, obstacles);
	
	// Spawn obstacles when new bees hit the ground
	for (int i = 0; i < maxNewBees; i++) {
	    if (newBeeActive[i] && newBeeY[i] >= resolutionY - 2 * boxPixelsY) {
		spawnObstacle(newBeeX[i], newBeeY[i], player_x, player_y, obstacles);
		spawnObstacle(newBeeX[i], newBeeY[i], player_x, player_y, obstacles);
		newBeeActive[i] = false; // Deactivate the new bee
	    }
	}
	// Spawn obstacles when bees hit the ground
	for (int i = 0; i < maxBees; i++) {
	    if (beeActive[i] && beeY[i] >= resolutionY - 2 * boxPixelsY) {
		spawnObstacle(beeX[i], beeY[i], player_x, player_y, obstacles);
		beeActive[i] = false; // Deactivate the bee
	    }
	}
	
	// Update hummingbird animation
	updateHummingbirdAnimation(hummingbirdSprite, hummingbirdAnimationClock, hummingbirdCurrentFrame);

	// Move hummingbird
	moveHummingbird(birdX, birdY, hummingbirdSpeed,honeycombX, honeycombY, honeycombActive,newHoneycombX, newHoneycombY, newHoneycombActive,targetHoneycombIndex, isTargetNewHoneycomb, honeycombWaitClock);

	// Draw hummingbird
	drawHummingbird(window, hummingbirdSprite);
	//draw beehives
	drawBeehives(window);
	window.draw(powerUpTimerText);
        window.draw(groundRectangle);
       

        // Draw remaining lives
	for (const auto& lifeSprite : lifeSprites) {
	    window.draw(lifeSprite);  
	}
	
	// Check if "Player Died!" message should be displayed
	if (showPlayerDiedMessage) {
	    if (playerDiedClock.getElapsedTime().asSeconds() < 1) {
		window.draw(playerDiedText); // Render the text
	    } else {
		showPlayerDiedMessage = false; // Stop displaying after 1 second
	    }
	}
	
	checkForBonus(window); // Check for bonus lives periodically
	// Update score dynamically
        scoreText.setString("Score: " + std::to_string(playerScore));
        window.draw(scoreText);
        window.display();
        window.clear();
    }
    updateLeaderboard(playerName, playerScore); // Save score after level ends


}


void playLevel1(RenderWindow &window) {

    // Initialization code for level one
    resetLevelState();
    //variables specific setting for level
    maxBees = 20;
    initialBees = 5;
    maxRandomHoneycombs = 3;
    
    maxNewBees = 5;  // Maximum number of new bees
    initialNewBees = 1;  // Initial number of new bees
    fastbeeSpeed = 10;   
    
    maxBeehives = 1;  
    playerScore = 0; // Reset score at the start of the level
   
    //////////////////////////////////////////////////////////////////SetUp//////////////////////////////////////
    levelSetUp(window,"Music/Music3.ogg","Textures/level1.jpg");
}

void playLevel2(RenderWindow &window) {
    // Logic for Level 2
    
    // Initialization code for level 2
    resetLevelState();
    //variables specific setting for level
    maxBees = 30;
    initialBees = 10;
    maxRandomHoneycombs = 6;
    
    maxNewBees = 10;  // Maximum number of new bees
    initialNewBees = 1;  // Initial number of new bees
    fastbeeSpeed = 10;   
    
    maxBeehives = 2; 
   
    //////////////////////////////////////////////////////////////////SetUp//////////////////////////////////////
    levelSetUp(window ,"Music/Music3.ogg","Textures/level2.jpg");
}

void playLevel3(RenderWindow &window) {
    // Logic for Level 3

    // Initialization code for level 3
    resetLevelState();
    //variables specific setting for level
    maxBees = 40;
    initialBees = 20;
    maxRandomHoneycombs = 9;
    
    maxNewBees = 18;  // Maximum number of new bees
    initialNewBees =1;  // Initial number of new bees
    fastbeeSpeed = 10;   
    
    maxBeehives = 5; 
 
    //////////////////////////////////////////////////////////////////SetUp//////////////////////////////////////
    levelSetUp(window ,"Music/Music3.ogg","Textures/level3.jpg");
}

void playBossLevel(RenderWindow &window) {
    // Logic for the Boss Level
    // Initialization code for Boss Level
    resetLevelState();
    //variables specific setting for level
    maxBees = 60;
    initialBees = 30;
    maxRandomHoneycombs = 7;
    
    maxNewBees = 30;  // Maximum number of new bees
    initialNewBees = 8;  // Initial number of new bees
    fastbeeSpeed = 10;   
    
    maxBeehives = 8; 
    
    

    Font powerUpFont;
	if (!powerUpFont.loadFromFile("Fonts/arial.ttf")) {
	    cout << "Error loading font!" << endl;
	}
	Text powerUpTimerText;
	powerUpTimerText.setFont(powerUpFont);
	powerUpTimerText.setCharacterSize(20);
	powerUpTimerText.setFillColor(Color::White);
	powerUpTimerText.setPosition(resolutionX / 2 - 50, resolutionY - 30); // Bottom middle

   
    //////////////////////////////////////////////////////////////////SetUp//////////////////////////////////////
    BosslevelSetUp(window ,"Music/risk.ogg","Textures/bosslevel.jpg");
}





