#include <allegro.h>
#include "Declarations.h"
#include <stdlib.h>
#include <stdio.h>
volatile long iSpeed = 0;
int countdown = 0;

void initGame()
{
    allegroSetup();
    srand(time(0));
    install_timer();
    
    LOCK_VARIABLE(iSpeed);
    LOCK_FUNCTION(speed);
    
    LOCK_VARIABLE(countdown);
    LOCK_FUNCTION(timer);
    
    install_int_ex(speed, BPS_TO_TIMER(60));
    install_int_ex(timer, BPS_TO_TIMER(1));
}

void gameLoop(Character character)
{
    FONT *myfont;
    myfont = load_font("Verdana 28.pcx", NULL, NULL);
    bool quit;
    
    BITMAP *buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    titleScreen(myfont, buffer, quit, iSpeed);
    
    iSpeed = 0;
    BITMAP *bg = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    Platform border[3];
    createBorders(border);
    Coin coin;
    coinInit(coin);
    Platform ground;
    
    Platform platform[maxP];
    int p;
    int GRAVITY;
    int worldNum = 5;
    loadWorld(worldNum, bg, ground, platform, p, GRAVITY, coin.maxCoin, countdown, character);
    int playableScreenHeight = SCREEN_HEIGHT - ground.image->h;
    int frameCount = 0;
    
    KillingBall killingBall[2];
    for (int i = 0; i < 2; i++)
    {
        killingBall[i].image = load_bitmap("Images/killing ball.bmp", NULL);
        setupKillingBall(killingBall[i]);
        killingBall[i].initialised = 0;
    }

    while (!quit)
    {
        while (iSpeed > 0)
        {
            if (key[KEY_ESC])
            {
                quit = 1;
                break;
            }
            
            character.velX = checkMovement();
            character.attemptedVelX = checkMovement();
            if (character.velY < 18) //terminal velocity
                character.velY += GRAVITY;

            bool bSideCollision = 0;
            bool bTopBotCollision = 0;
            int iCornerCollision = 0;
            for (int i = 0; i < 3; i++)
            {
                checkMoveCollision(character.body, character.posX, character.posY, character.velX, character.velY, character.jumpAbility,
                    border[i].image, border[i].posX, border[i].posY, 0, 0, bSideCollision, bTopBotCollision, iCornerCollision); 
            }
            for (int i = 0; i < p; i++)
            {
                checkMoveCollision(character.body, character.posX, character.posY, character.velX, character.velY, character.jumpAbility,
                    platform[i].image, platform[i].posX, platform[i].posY, 0, 0, bSideCollision, bTopBotCollision, iCornerCollision); 
            }
           
            checkGroundCollision(character.body, character.posY, character.velY, character.jumpAbility, ground.image);

            //characterWrap(character.body, character.posX, character.wrap);

            //bool bSideWrapCollision = 0;
            //bool bTopBotWrapCollision = 0;
            //int iCornerWrapCollision = 0;
            //if (character.wrap != 0)
            //{
                /*if (character.wrap > 0)
                {
                    BITMAP *leftWrap = create_bitmap(character.wrap, character.body->h);
                    int wrapPosX = 0 - (character.body->w - character.wrap);
                    for (int i = 0; i < p; i++)
                    {
                        checkMoveCollision(leftWrap, wrapPosX, character.posY, character.velX, character.velY, character.jumpAbility, 
                            platform[i].image, platform[i].posX, platform[i].posY, 0, 0, bSideWrapCollision, bTopBotWrapCollision, iCornerWrapCollision); 
                    }
                    character.posX = SCREEN_WIDTH - (wrapPosX);
                }
                else if (character.wrap < 0)
                {
                    for (int i = 0; i < p; i++)
                    {
                        checkMoveCollision(character.body, character.wrap, character.posY, character.velX, character.velY, character.jumpAbility, 
                            platform[i].image, platform[i].posX, platform[i].posY, 0, 0, bSideWrapCollision, bTopBotWrapCollision, iCornerWrapCollision); 
                    }
                }*/
            //}

            checkJump(character.velY, character.jumpAbility);

            bSideCollision = 0;
            bTopBotCollision = 0;
            iCornerCollision = 0;
            for (int i = 0; i < p; i++)
            {
                checkMoveCollision(character.body, character.posX, character.posY, character.velX, character.velY, character.jumpAbility, 
                    platform[i].image, platform[i].posX, platform[i].posY, 0, 0, bSideCollision, bTopBotCollision, iCornerCollision); 
            }
            
            for (int i = 0; i < 3; i++)
            {
                checkMoveCollision(character.body, character.posX, character.posY, character.velX, character.velY, character.jumpAbility,
                    border[i].image, border[i].posX, border[i].posY, 0, 0, bSideCollision, bTopBotCollision, iCornerCollision); 
            }

            if (bSideCollision == 0 && iCornerCollision == 0)//&& bSideWrapCollision == 0 && iCornerWrapCollision == 0)
                character.posX += character.velX;
            if (bTopBotCollision == 0 && iCornerCollision == 0)// && bTopBotWrapCollision == 0 && iCornerWrapCollision == 0)
                character.posY += character.velY;

            iSpeed--;
            frameCount++;

            if (frameCount >= 0 && frameCount < 10)
            {
                if (character.attemptedVelX > 0)
                    character.frame = 2;
                else if (character.attemptedVelX < 0)
                    character.frame = 5;
            }
            else if (frameCount >= 10 && frameCount < 20)
            {
                if (character.attemptedVelX > 0)
                    character.frame = 3;
                else if (character.attemptedVelX < 0)
                    character.frame = 6;
            }
            else if (frameCount >= 20 && frameCount < 30)
            {
                if (character.attemptedVelX > 0)
                    character.frame = 4;
                else if (character.attemptedVelX < 0)
                    character.frame = 7;
            }
            if (character.attemptedVelX == 0)
                character.frame = 1;
            if (frameCount >= 30)
                frameCount = 0;
 
            //Killing ball movement
            for (int i = 0; i < 2; i++)
            { 
                if (killingBall[i].initialised == 1)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        redKillingBall(killingBall[i].image, killingBall[i].posX, killingBall[i].posY, killingBall[i].velX, killingBall[i].velY, border[j].image, border[j].posX, border[j].posY, 0, 0);
                    }
                    redKillingBall(killingBall[i].image, killingBall[i].posX, killingBall[i].posY, killingBall[i].velX, killingBall[i].velY, ground.image, 0, SCREEN_HEIGHT - ground.image->h, 0, 0);
                    
                    killingBall[i].posX += killingBall[i].velX;
                    killingBall[i].posY += killingBall[i].velY;
                    
                    //Killing ball collision
                    if (checkCollision(character.body, character.posX, character.posY, character.velX, character.velY, killingBall[i].image, 
                       killingBall[i].posX, killingBall[i].posY, killingBall[i].velX, killingBall[i].velY) || nonVelCollision(character.body, character.posX, character.posY, killingBall[i].image, 
                       killingBall[i].posX, killingBall[i].posY))
                    {
                        printScreen(character, platform, p, bg, buffer, coin, myfont, countdown, killingBall);
                        printLose(buffer, myfont);
                        iSpeed = 0;
                        loadWorld(worldNum, bg, ground, platform, p, GRAVITY, coin.maxCoin, countdown, character);
                        coin.coinCount = 0;
                        moveCoin(coin, character, platform, p, border, playableScreenHeight);
                        setupKillingBall(killingBall[i]);
                        killingBall[0].velX = 4;
                        killingBall[0].velY = 4;
                        killingBall[0].posX = 30;
                        killingBall[0].posY = 50;
                        killingBall[1].velX = -4;
                        killingBall[1].velY = -4;
                        killingBall[1].posX = 1100;
                        killingBall[1].posY = 50;
                    }
                    if (i == 0)
                    {
                        if (killingBall[i].rotation > 255)
                            killingBall[i].rotation = 0;
                        else
                            killingBall[i].rotation += 2;
                    }
                    else
                    {
                        if (killingBall[i].rotation < 0)
                            killingBall[i].rotation = 255;
                        else
                            killingBall[i].rotation -= 2;
                    }
                }
            }
            //coin collision
            if (checkCollision(character.body, character.posX, character.posY, character.velX, character.velY, coin.image, 
                coin.posX, coin.posY, 0, 0) || nonVelCollision(character.body, character.posX, character.posY, coin.image, 
                coin.posX, coin.posY))
            {
                coin.coinCount += 1;
                if (coin.coinCount < coin.maxCoin)
                    moveCoin(coin, character, platform, p, border, playableScreenHeight);
                else
                    coin.posX = -100;
            }
            if (coin.coinCount < coin.maxCoin)
            {
                printScreen(character, platform, p, bg, buffer, coin, myfont, countdown, killingBall);
                clear_bitmap(buffer);
            }
            if (countdown < 1)
            {
                printScreen(character, platform, p, bg, buffer, coin, myfont, countdown, killingBall);
                printLose(buffer, myfont);
                iSpeed = 0;
                loadWorld(worldNum, bg, ground, platform, p, GRAVITY, coin.maxCoin, countdown, character);
                coin.coinCount = 0;
                moveCoin(coin, character, platform, p, border, playableScreenHeight);
            }
            else if (coin.coinCount >= coin.maxCoin) 
            {   
                printScreen(character, platform, p, bg, buffer, coin, myfont, countdown, killingBall);
                worldNum += 1;
                if (worldNum == 3)
                    setupKillingBall(killingBall[0]);
                else if (worldNum == 4)
                {
                    setupKillingBall(killingBall[0]);
                    setupKillingBall(killingBall[1]);
                    killingBall[0].velX = -4;
                    killingBall[0].velY = -4;
                    killingBall[0].posX = 30;
                    killingBall[0].posY = 50;
                    killingBall[1].velX = 4;
                    killingBall[1].velY = 4;
                    killingBall[1].posX = 1100;
                    killingBall[1].posY = 50;
                }
                if (loadWorld(worldNum, bg, ground, platform, p, GRAVITY, coin.maxCoin, countdown, character) == 0)
                {    
                    printLevelBeat(buffer, myfont, worldNum - 1);
                    iSpeed = 0;
                    blit(bg, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    coin.coinCount = 0;
                    moveCoin(coin, character, platform, p, border, playableScreenHeight);
                    for (int i = 0; i < 1; i++)
                    {
                        printScreen(character, platform, p, bg, buffer, coin, myfont, countdown, killingBall);
                    }
                }
                else
                {
                    for (int i = 0; i < 1; i++)
                        killingBall[i].initialised = 0;
                    printWon(buffer, myfont);
                    quit = 1;
                    break;
                }
            }
        }
    }
    remove_mouse();
    destroy_font(myfont);
    destroy_bitmap(ground.image);
    destroy_bitmap(coin.image);
    for (int i = 0; i < p; i++)
        destroy_bitmap(platform[i].image);
    destroy_bitmap(character.body);
    for (int i = 0; i < 1; i++)
        destroy_bitmap(killingBall[i].image);
    destroy_bitmap(buffer);
}

void speed()
{
     iSpeed++;
}
END_OF_FUNCTION(speed)

void timer()
{
     if (countdown > 0)
         countdown -= 1;
}
END_OF_FUNCTION(timer)

int checkMovement()
{
    if (key[KEY_LEFT] && key[KEY_RIGHT])
        return 0;
    else if (key[KEY_LEFT])
        return -5;
    else if (key[KEY_RIGHT])
        return 5;    
    else 
        return 0;
}

void checkJump(int &velY, int &jumpStatus)
{
    if (jumpStatus >= 1)
    {
        if (jumpStatus == 1) 
        {
            if (key[KEY_SPACE])
            {
                velY -= 18;
                jumpStatus = 2;
            }
        }
        else if (jumpStatus == 2)
        {
            if (!key[KEY_SPACE])
                jumpStatus = 3;
        }
        else
        {  
            if (key[KEY_SPACE])
            {
                jumpStatus = 0;
                velY = 1;
                velY -= 18;
            }
        }
                
    }
}

/*
void characterWrap(BITMAP *sprite, int &posX, int &wrap)
{
    if (posX > SCREEN_WIDTH)
    {
        posX = 0;    
        wrap = 0;
    }
    else if (posX + sprite->w > SCREEN_WIDTH)
    {
        //will be positive number              
        wrap = posX + sprite->w - SCREEN_WIDTH;
    }
    if (posX + sprite->w < 0)
        posX = SCREEN_WIDTH - sprite->w;
    else if (posX < 0)
    {
        //will be negative number
        wrap = 0 + posX;
    }
    else
        wrap = 0;
}
*/



