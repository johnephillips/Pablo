#include "Declarations.h"
#include <allegro.h>

int loadWorld(int num, BITMAP *bg, Platform &ground, Platform platform[], int &p, int &GRAVITY, int &maxCoin, int &countdown, Character &character)
{
    FILE *world;
    bool lastWorld = 0;
    switch (num)
    {
        case 1:
            world = fopen("World1.txt", "r");
            break;
        case 2:
            world = fopen("World2.txt", "r");
            break;
        case 3:
            world = fopen("World3.txt", "r");
            break;
        case 4:
            world = fopen("World4.txt", "r");
            break;
        default:
            lastWorld = 1;
            break;
    }
    if (!lastWorld)
    {
        if (world == NULL)
        {
            printf("Error opening world\n");
            system("pause");
        }
        
        int groundHeight;
        fscanf(world, "Ground height: %d\n", &groundHeight);
 
        character.posX = (SCREEN_WIDTH / 2) - (character.body->w / 2);
        character.posY = SCREEN_HEIGHT - groundHeight - character.body->h;

        ground.image = create_bitmap(SCREEN_WIDTH, groundHeight);

        fscanf(world, "Gravity: %d\n", &GRAVITY);
        fscanf(world, "Max Coin: %d\n", &maxCoin);
        fscanf(world, "Number of platforms: %d\n", &p);
    
        createPlatforms(world, num, platform, p);
        fclose(world);
    
        switch (num)
        {
            case 1:
                {
                int checkerSize = 10;
                int colour = BLUE;
            
                countdown = 30;
                //create a checkered background
                //cycle through rows
                for (int i = 0; i < SCREEN_HEIGHT / checkerSize; i++)
                {
                    //cycle through columns
                    for (int j = 0; j < (SCREEN_WIDTH / checkerSize) + 1; j++)
                    { 
                        rectfill(bg, j * checkerSize, i * checkerSize,  (j + 1) * checkerSize, (i + 1) * checkerSize, colour);  
                        if (colour == BLUE)
                        {
                            colour = GREEN;
                            continue;
                        }
                        else if (colour == GREEN)
                        {
                            colour = PURPLE;
                            continue;
                        }
                        else
                        {
                            colour = BLUE;
                            continue;
                        }
                    }
                }
                rectfill(ground.image, 0, 0, SCREEN_WIDTH, ground.image->h, GROUNDCOL);
                blit(ground.image, bg, 0, 0, 0, SCREEN_HEIGHT - ground.image->h, ground.image->w, ground.image->h);
                }
                break;
            case 2:
                 {
                     countdown = 35;
                     BITMAP *temp = load_bitmap("Images/sombrero background.bmp", NULL);
                     blit(temp, bg, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                     rectfill(ground.image, 0, 0, SCREEN_WIDTH, ground.image->h, BLACK);
                     blit(ground.image, bg, 0, 0, 0, SCREEN_HEIGHT - ground.image->h, ground.image->w, ground.image->h);
                 }
                 break;
            case 3:
                 {
                     countdown = 58;
                     BITMAP *temp = load_bitmap("Images/cactus background.bmp", NULL);
                     blit(temp, bg, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                     rectfill(ground.image, 0, 0, SCREEN_WIDTH, ground.image->h, PURPLE);
                     blit(ground.image, bg, 0, 0, 0, SCREEN_HEIGHT - ground.image->h, ground.image->w, ground.image->h);
                 }
                 break;
            case 4:
                 {
                     countdown = 75;
                     BITMAP *temp = load_bitmap("Images/lava.bmp", NULL);
                     blit(temp, bg, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                     rectfill(ground.image, 0, 0, SCREEN_WIDTH, ground.image->h, RED);
                     blit(ground.image, bg, 0, 0, 0, SCREEN_HEIGHT - ground.image->h, ground.image->w, ground.image->h);
                 }
                 break;
            default:
                break;
        }
        return 0;
    }
    return 1;
}
