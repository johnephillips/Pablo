#include "Declarations.h"
#include <allegro.h>

void createPlatforms(FILE *world, int worldNum, Platform createdPlatform[], int p)
{
    int colour;
    switch (worldNum)
    {
        case 1:
            colour = makecol(64, 167, 179);
            break;
        case 2:
             colour = BLACK;
             break;
        case 3:
             colour = PURPLE;
             break;
        default:
            break;
    }
             
    int temp;
    int height;
    int width;
    fscanf(world, "Height of platforms: %d\n", &height);
    fscanf(world, "Width of platforms: %d\n", &width);
    fscanf(world, "Location of platforms:\n");
    for (int i = 0; i < p; i++)
    {
        fscanf(world, "%d: %d, %d\n", &temp, &createdPlatform[i].posX, &createdPlatform[i].posY);
        
        createdPlatform[i].image = create_bitmap(width, height);
        
        switch (worldNum)
        {
            case 1:
                rectfill(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w, createdPlatform[i].image->h, colour);
                rect(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w - 1, createdPlatform[i].image->h - 1, BLACK);

                if (colour == makecol(64, 167, 179))
                    colour =  makecol(208, 103, 40);
                else if (colour == makecol(208, 103, 40))
                    colour = makecol(204, 57, 134);
                else
                    colour = makecol(64, 167, 179);
                break;
           case 2:
                rectfill(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w, createdPlatform[i].image->h, colour);
                rect(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w - 1, createdPlatform[i].image->h - 1, GREEN);
                break;
           case 3:
                rectfill(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w, createdPlatform[i].image->h, colour);
                rect(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w - 1, createdPlatform[i].image->h - 1, BLACK);
                break;
           case 4:
                rectfill(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w, createdPlatform[i].image->h, RED);
                rect(createdPlatform[i].image, 0, 0, createdPlatform[i].image->w - 1, createdPlatform[i].image->h - 1, BLACK);
                break;
           default:
               break;
       }
    }
}

void createBorders(Platform border[])
{
    border[0].image = create_bitmap(SCREEN_WIDTH + 2, 1);
    border[0].posX = -1;
    border[0].posY = -1;
    border[1].image = create_bitmap(1, SCREEN_HEIGHT + 2);
    border[1].posX = -1;
    border[1].posY = -1;
    border[2].image = create_bitmap(1, SCREEN_HEIGHT + 2);
    border[2].posX = SCREEN_WIDTH + 1;
    border[2].posY = -1;
}

void coinInit(Coin &coin)
{
    coin.coinCount = 0;
    coin.maxCoin = 0;
    coin.image = create_bitmap(30, 30);
    coin.image = load_bitmap("Images/Coin.bmp", NULL);
    coin.posX = 400;
    coin.posY = 240;
}

void moveCoin(Coin &coin, Character character, Platform platform[], int p, Platform border[], int playableScreenHeight)
{
    int newPosX = coin.posX;
    int newPosY = coin.posY;
    bool collision = 1;
    while (collision)
    {
        collision = 0;
        
        newPosX = rand() % (SCREEN_WIDTH - coin.image->w);
        newPosY = rand() % (playableScreenHeight - coin.image->h);

        if (nonVelCollision(character.body, character.posX, character.posY, coin.image, newPosX, newPosY))
            collision = 1;
        for (int i = 0; i < p; i++)
        {
            if (nonVelCollision(platform[i].image, platform[i].posX, platform[i].posY, coin.image, newPosX, newPosY))
                collision = 1;
        }
        for (int i = 0; i < 3; i++)
        {
            if (nonVelCollision(border[i].image, border[i].posX, border[i].posY, coin.image, newPosX, newPosY))
                collision = 1;
        }
        if (collision == 0)
        {
            if ((newPosX > coin.posX) && (!(newPosX > coin.posX + 300) || !(newPosX < coin.posX + 600)))
            {
                collision = 1;
                continue;
            }
            if ((newPosX < coin.posX) && (!(newPosX < coin.posX - 300) || !(newPosX > coin.posX - 600)))
            {
                collision = 1;
                continue;
            }
            if ((newPosY > coin.posY) && (!(newPosY > coin.posY + 300) || !(newPosY < coin.posY + 600)))
            {
                collision = 1;
                continue;
            }
            if ((newPosY < coin.posY) && (!(newPosY < coin.posY - 300) || !(newPosY > coin.posY - 600)))
            {
                collision = 1;
                continue;
            }
        }
    } 
    coin.posX = newPosX;
    coin.posY = newPosY;
}

void setupKillingBall(KillingBall &killingBall)
{
    if ((rand() % 2) + 1 == 1)
        killingBall.velX = 4;
    else 
        killingBall.velX = -4;
    if ((rand() % 2) + 1 == 1)
        killingBall.velY = 4;
    else 
        killingBall.velY = -4;

    killingBall.rotation = 0;
    killingBall.posX = rand() % (SCREEN_WIDTH - killingBall.image->w);
    killingBall.posY = rand() % 400;
    killingBall.initialised = 1;
}
