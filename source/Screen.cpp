#include <allegro.h>
#include <stdio.h>
#include "Declarations.h"

void allegroSetup()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
}

void printScreen(Character character, Platform platform[], int p, BITMAP *bg, BITMAP *buffer, Coin coin, FONT *myfont, int countdown, KillingBall killingBall[])
{
    blit(bg, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    character.body = frame(character.frame);
    draw_sprite(buffer, character.body, character.posX, character.posY);
    if (character.wrap > 0)
        masked_blit(character.body, buffer, character.body->w - character.wrap, 0, 0, character.posY, character.wrap, character.body->h);
    if (character.wrap < 0)
        masked_blit(character.body, buffer, 0, 0, SCREEN_WIDTH - (character.wrap * -1), character.posY, character.wrap * -1, character.body->h);
    for (int i = 0; i < p; i++)
    {
        blit(platform[i].image, buffer, 0, 0, platform[i].posX, platform[i].posY, platform[i].image->w, platform[i].image->h);
        //textprintf_ex(buffer, font, platform[i].posX, platform[i].posY, YELLOW, -1, "%d", i + 1); //prints platform numbers
    }
    draw_sprite(buffer, coin.image, coin.posX, coin.posY);
    textprintf_ex(buffer, myfont, SCREEN_WIDTH - 175, 20, YELLOW, -1, "%d / %d", coin.coinCount, coin.maxCoin);
    textprintf_ex(buffer, myfont, 30, 20, YELLOW, -1, "%d", countdown);

    for (int i = 0; i < 2; i++)
    {
        if (killingBall[i].initialised == 1)
            rotate_sprite(buffer, killingBall[i].image, killingBall[i].posX, killingBall[i].posY, ftofix(killingBall[i].rotation));
    }

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
}

BITMAP *frame(int frame)
{
    BITMAP *bitmap;
    switch (frame)
    {
        case 1:
            bitmap = load_bitmap("Images/Dwarf/dwarf.bmp", NULL);
            break;
        case 2:
            bitmap = load_bitmap("Images/Dwarf/frame2.bmp", NULL);
            break;
        case 3:
            bitmap = load_bitmap("Images/Dwarf/frame3.bmp", NULL);
            break;
        case 4:
            bitmap = load_bitmap("Images/Dwarf/frame4.bmp", NULL);
            break;
        case 5:
            bitmap = load_bitmap("Images/Dwarf/frame5.bmp", NULL);
            break;
        case 6:
            bitmap = load_bitmap("Images/Dwarf/frame6.bmp", NULL);
            break;
        case 7:
            bitmap = load_bitmap("Images/Dwarf/frame7.bmp", NULL);
            break;
        default: break;
    }
    return bitmap;
}

void titleScreen(FONT *myfont, BITMAP *buffer, bool &quit, volatile long int &iSpeed)
{
    BITMAP *cursor = create_bitmap(29, 19);
    cursor = load_bitmap("Images/taco.bmp", NULL);
    set_mouse_cursor_bitmap(MOUSE_CURSOR_ALLEGRO, cursor);
    
    BITMAP *titleScreen = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    titleScreen = load_bitmap("Images/fiesta.bmp", NULL);   
    
    Button play;
    createButton(play, 220, myfont, "Play", titleScreen);
    
    Button instructions;
    createButton(instructions, 420, myfont, "Instructions", titleScreen);
    
    textprintf_centre_ex(titleScreen, myfont, SCREEN_WIDTH / 2, 20, BLUE, -1, "Pablo");
    rectfill(titleScreen, SCREEN_WIDTH / 2 - 220, 700, SCREEN_WIDTH / 2 + 220, 750, WHITE);
    textprintf_centre_ex(titleScreen, myfont, SCREEN_WIDTH / 2, 700, BLACK, -1, "Made by: John Phillips");
    
    bool instruction = 0;
    bool done = 0;
    bool click = 1;
    int clickedX = 0;
    int clickedY = 0;
    int unClickedX = 0;
    int unClickedY = 0;
    while (!play.clicked && !key[KEY_ESC])
    {
        click = 1;
        done = 0;
        while (!quit && !done)
        {
            while (iSpeed > 0)
            {
                blit(titleScreen, buffer, 0, 0, 0, 0, titleScreen->w, titleScreen->h);
                printTitleScreen(0, buffer, myfont);
                if (key[KEY_ESC])
                    quit = 1;
                poll_mouse();
                if (mouse_b&1 && click)
                {  
                    click = 0;
                    clickedX = mouse_x;
                    clickedY = mouse_y;
                }
                if (click == 0)
                {
                    if (!(mouse_b&1))
                    {
                        click = 1;
                    }
                    if (click == 1)
                    {
                        done = 1;
                        unClickedX = mouse_x;
                        unClickedY = mouse_y;
                    }
                }
                iSpeed--;
            }
        }
        if (checkClick(clickedX, clickedY, unClickedX, unClickedY, play))
        {
            play.clicked = 1;        
        }
        if (checkClick(clickedX, clickedY, unClickedX, unClickedY, instructions))
        {
            instructions.clicked = 1;        
        }
        
        done = 0;
        iSpeed = 0;   
        if (instructions.clicked)
        {
            BITMAP *instructionScreen;
            instructionScreen = load_bitmap("Images/fiesta.bmp", NULL);
            Button backToMain;      
            createButton(backToMain, 650, myfont, "Back To Main", instructionScreen);
            blit(instructions.image, instructionScreen, 0, 0, instructions.posX, 20, instructions.width, instructions.height);   
            BITMAP *killingBall = load_bitmap("Images/killing ball.bmp", NULL);
            BITMAP *coin = load_bitmap("Images/Coin.bmp", NULL);
            rectfill(instructionScreen, 150, 100, 1050, 630, WHITE);  
            masked_blit(killingBall, instructionScreen, 0, 0, 700, 500, killingBall->w, killingBall->h);   
            masked_blit(coin, instructionScreen, 0, 0, 1010, 370, coin->w, coin->h);
            destroy_bitmap(killingBall);
            destroy_bitmap(coin);
            textprintf_ex(instructionScreen, myfont, 160, 110, BLACK, -1, "-Use the arrow keys to move left and right.");
            textprintf_ex(instructionScreen, myfont, 160, 160, BLACK, -1, "-Use the spacebar to jump.");
            textprintf_ex(instructionScreen, myfont, 160, 210, BLACK, -1, "-Use the spacebar twice to double jump.");
            textprintf_ex(instructionScreen, myfont, 160, 260, BLACK, -1, "-Press enter to go to the next level.");
            textprintf_ex(instructionScreen, myfont, 160, 310, BLACK, -1, "-Press escape to quit at any point.");
            textprintf_ex(instructionScreen, myfont, 160, 360, BLACK, -1, "-Collect coins to advance to the next level.");
            textprintf_ex(instructionScreen, myfont, 160, 410, BLACK, -1, "-If the timer in the top left runs out, you");
            textprintf_ex(instructionScreen, myfont, 160, 460, BLACK, -1, "must restart the level.");
            textprintf_ex(instructionScreen, myfont, 160, 510, BLACK, -1, "-Blue stars will kill you.");
            while (instructions.clicked && !quit)
            {
                click = 1;
                while (!done && !quit)
                {
                    while (iSpeed > 0)
                    {
                        blit(instructionScreen, buffer, 0, 0, 0, 0, titleScreen->w, titleScreen->h);
                        printTitleScreen(0, buffer, myfont);
                        if (key[KEY_ESC])
                            quit = 1;
                        
                        poll_mouse();
                        if (mouse_b&1 && click)
                        {  
                            click = 0;
                            clickedX = mouse_x;
                            clickedY = mouse_y;
                        }
                        if (click == 0)
                        {
                            if (!(mouse_b&1))
                            {
                                click = 1;
                            }
                            if (click == 1)
                            {
                                done = 1;
                                unClickedX = mouse_x;
                                unClickedY = mouse_y;
                            }
                        }
                        iSpeed--;
                    }
                }  
                done = 0;
                
                if (checkClick(clickedX, clickedY, unClickedX, unClickedY, backToMain))
                {
                    instructions.clicked = 0;  
                }
            }
        }     
    }
    destroy_bitmap(play.image);
    destroy_bitmap(instructions.image);
    destroy_bitmap(cursor);
    destroy_bitmap(titleScreen);
    remove_mouse();
}

void printTitleScreen(int screenNum, BITMAP *buffer, FONT *myfont)
{
    if (screenNum == 0)
    {
        show_mouse(buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        clear_bitmap(buffer);          
    }
    else if (screenNum == 1)
    {
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        clear_bitmap(buffer);          
    }    
}

void createButton(Button &button, int y, FONT *myfont, char text[], BITMAP *destination)
{
    button.width = text_length(myfont, text);
    button.height = text_height(myfont);
    button.posX = (SCREEN_WIDTH / 2) - (button.width / 2);
    button.posY = y;
    button.image = create_bitmap(button.width, button.height);
    textprintf_ex(button.image, myfont, 0, 0, RED, DARKBLUE, text);    
    button.clicked = 0; 
    if (button.image == NULL)
    {
        printf("Error making button");
    }
    else
    {
        blit(button.image, destination, 0, 0, button.posX, button.posY, button.width, button.height); 
    }
}

bool checkClick(int clickedX, int clickedY, int unClickedX, int unClickedY, Button button)
{
    if (clickedX >= button.posX && unClickedX <= button.posX + button.width && 
        unClickedX >= button.posX && unClickedX <= button.posX + button.width)
    {
        if (clickedY >= button.posY && clickedY <= button.posY + button.height && 
            unClickedY >= button.posY && unClickedY <= button.posY + button.height)
            return 1;           
    }
    return 0;
             
}

void printLevelBeat(BITMAP *buffer, FONT *myfont, int worldNum)
{
    clear_keybuf();
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 550, WHITE, -1, "You Beat Level %d!!!", worldNum);
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 600, WHITE, -1, "Press enter to continue."); 
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!key[KEY_ENTER])
    {
    }  
}

void printLose(BITMAP *buffer, FONT *myfont)
{
    clear_keybuf();    
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 550, BLACK, -1, "YOU DIED"); 
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 600, WHITE, -1, "Press enter to continue."); 
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!key[KEY_ENTER])
    {
    }
}

void printWon(BITMAP *buffer, FONT *myfont)
{
    BITMAP *endScreen = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    endScreen = load_bitmap("Images/fiesta.bmp", NULL);  
    rectfill(endScreen, 350, 200, 850, 500, WHITE);      
    blit(endScreen, buffer, 0, 0, 0, 0, endScreen->w, endScreen->h);
    destroy_bitmap(endScreen);
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 250, BLACK, -1, "CONGRATULATIONS!!!!"); 
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 320, BLACK, -1, "YOU BEAT PABLO!!!!"); 
    textprintf_centre_ex(buffer, myfont, SCREEN_WIDTH / 2, 390, BLACK, -1, "Press escape to quit"); 
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!key[KEY_ESC])
    {
    }
}
