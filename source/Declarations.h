#ifndef DECLARATIONS
#define DECLARATIONS

#include <stdio.h>
#include <allegro.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define BLUE makecol(72, 189, 196)
#define DARKBLUE makecol(0, 0, 160)
#define GREEN makecol(60, 160, 80)
#define PURPLE makecol(170, 70, 140)
#define YELLOW makecol(255, 255, 0)
#define RED makecol(250, 28, 36)
#define GROUNDCOL makecol(160, 50, 50)
#define BLACK makecol(0, 0, 0)
#define WHITE makecol(255, 255, 255)
#define maxP 20

struct Character
{
    int velX, velY, posX, posY, attemptedVelX;
    int frame;
    int jumpAbility;
    int wrap;
    BITMAP *body;
};

struct KillingBall
{
    int velX, velY, posX, posY, rotation;
    bool initialised;
    BITMAP *image;
};

struct Platform
{
    int posX, posY;
    BITMAP *image;
};

struct Coin
{
    int posX, posY;
    int coinCount, maxCoin;
    BITMAP *image;
};

struct Button
{
    int width, height, posX, posY;
    bool clicked;
    BITMAP *image;
};

void allegroSetup();
void speed();
void timer();
void initGame();
void gameLoop(Character character);
int loadWorld(int num, BITMAP *bg, Platform &ground, Platform platform[], int &p, int &GRAVITY, int &maxCoin, int &countdown, Character &character);
void characterInit(Character &character);
//void characterWrap(BITMAP *sprite, int &posX, int &wrap);
BITMAP *frame(int frame);
void createButton(Button &button, int y, FONT *myfont, char text[], BITMAP *destination); 
bool checkClick(int clickedX, int clickedY, int unClickedX, int unClickedY, Button button);
void titleScreen(FONT *myfont, BITMAP *buffer, bool &quit, volatile long int &iSpeed);
void printTitleScreen(int screenNum, BITMAP *buffer, FONT *myfont);
void createPlatforms(FILE *world, int worldNum, Platform createdPlatform[], int p);
void setupKillingBall(KillingBall &killingBall);
void redKillingBall(BITMAP *bitmap1, int &posX1, int &posY1, int &velX1, int &velY1, BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2);
void printLevelBeat(BITMAP *buffer, FONT *myfont, int worldNum);
void printLose(BITMAP *buffer, FONT *myfont);
void printWon(BITMAP *buffer, FONT *myfont);
void printScreen(Character character, Platform platform[], int p, BITMAP *bg, BITMAP *buffer, Coin coin, FONT *myfont, 
    int countdown, KillingBall killingBall[]);
void moveCoin(Coin &coin, Character character, Platform platform[], int p, Platform border[], int playableScreenHeight);
void createBorders(Platform border[]);
void coinInit(Coin &coin);
void checkGroundCollision(BITMAP *sprite, int &posY, int &velY, int &jumpStatus, BITMAP *ground);
void checkMoveCollision(BITMAP *bitmap1, int &posX1, int &posY1, int &velX1, int &velY1, int &bitmap1jump, 
    BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2, bool &bSideCollision, bool &bTopBotCollision, 
    int &iCornerCollision);
bool checkCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, int velY1, BITMAP *bitmap2, int posX2, int posY2, 
    int velX2, int velY2);
bool nonVelCollision(BITMAP *bitmap1, int posX1, int posY1, BITMAP *bitmap2, int posX2, int posY2);
void checkJump(int &velY, int &jumpStatus);
int checkMovement();
int sideCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, BITMAP *bitmap2, int posX2, int posY2, int velX2);
int topBotCollision(BITMAP *bitmap1, int posX1, int posY1, int velY1, BITMAP *bitmap2, int posX2, int posY2, int velY2);
int cornerCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, int velY1, BITMAP *bitmap2, int posX2, 
    int posY2, int velX2, int velY2);

#endif
