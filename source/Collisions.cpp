#include <allegro.h>
#include "Declarations.h"
#include <stdio.h>

//checks if the character collides with the side of a platform (1 for left, 2 for right)
int sideCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, BITMAP *bitmap2, int posX2, int posY2, int velX2)
{
    if (posY1 <= posY2 + bitmap2->h && posY1 + bitmap1->h >= posY2) 
    {
        if (posX1 + bitmap1->w < posX2)
        {
            if (posX1 + bitmap1->w + velX1 >= posX2 + velX2)
                return 1;
        }
        else if (posX1 > posX2 + bitmap2->w) 
        {
            if (posX1 + velX1 <= posX2 + bitmap2->w + velX2)
                return 2;
        }
    }
    return 0;
}

//checks if the character collides with the top or bottom of a platform (1 for top, 2 for bottom)
int topBotCollision(BITMAP *bitmap1, int posX1, int posY1, int velY1, BITMAP *bitmap2, int posX2, int posY2, int velY2)
{
    if (posX1 + bitmap1->w >= posX2 && posX1 <= posX2 + bitmap2->w)
    {
        if (posY1 + bitmap1->h < posY2)
        {
            if (posY1 + bitmap1->h + velY1 >= posY2 + velY2)
                return 1;
        }
        else if (posY1 > posY2 + bitmap2->h)
        {
            if (posY1 + velY1 <= posY2 + bitmap2->h + velY2)
                return 2;
        }
    } 
    return 0;
}

int cornerCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, int velY1, BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2)
{
    //top left platform
    if (posX1 + bitmap1->w < posX2 && posY1 + bitmap1->h < posY2)
    {
        if (velX1 > 0 && velY1 > 0)
        {
            if (posX1 + bitmap1->w + velX1 >= posX2 + velX2 && posY1 + bitmap1->h + velY1 >= posY2 + velY2)
                return 1;
        }
    }
    //top right platform
    if (posX1 > posX2 + bitmap2->w && posY1 + bitmap1->h < posY2)
    {
        if (velX1 < 0 && velY1 > 0)
        {
            if (posX1 + velX1 <= posX2 + bitmap2->w + velX2 && posY1 + bitmap1->h + velY1 >= posY2 + velY2)
            {
                return 2;
            }
        }   
    }
    //bottom left platform
    if (posX1 + bitmap1->w < posX2 && posY1 > posY2 + bitmap2->h)
    {
        if (velX1 > 0 && velY1 < 0)
        {
            if (posX1 + bitmap1->w + velX1 >= posX2 + velX2 && posY1 + velY1 <= posY2 + bitmap2->h + velY2)
            {
                return 3;
            }
        }
    }
    //bottom right platform
    if (posX1 > posX2 + bitmap2->w && posY1 > posY2 + bitmap2->h)
    {
        if (velX1 < 0 && velY1 < 0)
        {
            if (posX1 + velX1 <= posX2 + bitmap2->w + velX2 && posY1 + velY1 <= posY2 + bitmap2->h + velY2)
            {
                return 4;
            }
        }    
    }
    return 0;
}

void checkMoveCollision(BITMAP *bitmap1, int &posX1, int &posY1, int &velX1, int &velY1, int &bitmap1jump, BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2, bool &bSideCollision, bool &bTopBotCollision, int &iCornerCollision)
{
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, 0) == 1)
    {
        velX1 = 0;
        posX1 = posX2 - bitmap1->w - 1;
        bSideCollision = 1;
    }
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, 0) == 2)
    {
        velX1 = 0;
        posX1 = posX2 + bitmap2->w + 1;
        bSideCollision = 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, 0) == 1)
    {
        velY1 = 1;
        if (!key[KEY_SPACE])
            bitmap1jump = 1;
        posY1 = posY2 - bitmap1->h - 1;
        bTopBotCollision = 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, 0) == 2)
    {
        velY1 = 1;
        posY1 = posY2 + bitmap2->h + 1;
        bTopBotCollision = 1;
    }     
    switch (cornerCollision(bitmap1, posX1, posY1, velX1, velY1, bitmap2, posX2, posY2, 0, 0))
    {
        //top left platform
        case 1:
            velY1 = 0;
            velX1 = 0;
            posX1 = posX2 - bitmap1->w - 1;
            posY1 = posY2 - bitmap1->h;
            iCornerCollision = 1;
            break;
        //top right platform
        case 2:
            velY1 = 0;
            velX1 = 0;
            posX1 = posX2 + bitmap2->w + 1;
            posY1 = posY2 - bitmap1->h;
            iCornerCollision = 2;
            break;
        //bottom left platform
        case 3:
            velY1 = 0;
            velX1 = 0;
            posX1 = posX2 - bitmap1->w - 1;
            posY1 = posY2 + bitmap2->h + 1;
            iCornerCollision = 3;
            break;
         //bottom right platform
         case 4:
             velY1 = 0;
             velX1 = 0;
             posX1 = posX2 + bitmap2->w + 1;
             posY1 = posY2 + bitmap2->h + 1;
             iCornerCollision = 4;
             break;
         default: iCornerCollision = 0; break;           
    }     
}

void redKillingBall(BITMAP *bitmap1, int &posX1, int &posY1, int &velX1, int &velY1, BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2)
{
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, 0) == 1)
    {
        velX1 = -velX1;
        posX1 = posX2 - bitmap1->w - 1;
    }
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, 0) == 2)
    {
        velX1 = -velX1;
        posX1 = posX2 + bitmap2->w + 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, 0) == 1)
    {
        velY1 = -velY1;
        posY1 = posY2 - bitmap1->h - 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, 0) == 2)
    {
        velY1 = -velY1;
        posY1 = posY2 + bitmap2->h + 1;
    }     
}

bool nonVelCollision(BITMAP *bitmap1, int posX1, int posY1, BITMAP *bitmap2, int posX2, int posY2)
{
    if ((posY1 >= posY2 && posY1 <= posY2 + bitmap2->h) || (posY1 + bitmap1->h >= posY2 && posY1 + bitmap1->h <= posY2 + bitmap2->h))
    {
        //left corner bitmap 1
        if (posX1 <= posX2 + bitmap2->w && posX1 >= posX2)
            return 1;
        //right bitmap 1
        if (posX1 + bitmap1->w >= posX2 && posX1 + bitmap1->w <= posX2 + bitmap2->w)
            return 1;
    }
    if ((posY2 >= posY1 && posY2 <= posY1 + bitmap1->h) || (posY2 + bitmap2->h >= posY1 && posY2 + bitmap2->h <= posY1 + bitmap1->h))
    {
        //left bitmap 2
        if (posX2 <= posX1 + bitmap1->w && posX2 >= posX1)
            return 1;
        //right bitmap 2
        if (posX2 + bitmap2->w >= posX1 && posX2 + bitmap2->w <= posX1 + bitmap1->w)
            return 1;
    }
    if (posY1 >= posY2 && posY1 <= posY2 + bitmap2->h)
    {
        if (posX1 < posX2 && posX1 + bitmap1->w > posX2 + bitmap2->w)
            return 1;
    }
    if (posY2 >= posY1 && posY2 <= posY1 + bitmap1->h)
    {
        if (posX2 < posX1 && posX2 + bitmap2->w > posX1 + bitmap1->w)
            return 1;
    }
    return 0;
}

bool checkCollision(BITMAP *bitmap1, int posX1, int posY1, int velX1, int velY1, BITMAP *bitmap2, int posX2, int posY2, int velX2, int velY2)
{
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, velX2) == 1)
    {
        return 1;
    }
    if (sideCollision(bitmap1, posX1, posY1, velX1, bitmap2, posX2, posY2, velX2) == 2)
    {
        return 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, velY2) == 1)
    {
        return 1;
    }
    if (topBotCollision(bitmap1, posX1, posY1, velY1, bitmap2, posX2, posY2, velY2) == 2)
    {
        return 1;
    }     
    switch (cornerCollision(bitmap1, posX1, posY1, velX1, velY1, bitmap2, posX2, posY2, velX2, velY2))
    {
        //top left platform
        case 1:
            return 1;
            break;
        //top right platform
        case 2:
            return 1;
            break;
        //bottom left platform
        case 3:
            return 1;
            break;
         //bottom right platform
         case 4:
             return 1;
             break;
         default: break;           
    }
    return 0;
}

void checkGroundCollision(BITMAP *sprite, int &posY, int &velY, int &jumpStatus, BITMAP *ground)
{
    if (posY + sprite->h + velY >= SCREEN_HEIGHT - ground->h)
    {
        velY = 1;
        if (!key[KEY_SPACE])
            jumpStatus = 1;
        posY = SCREEN_HEIGHT - ground->h - sprite->h - 1;
    }
}
