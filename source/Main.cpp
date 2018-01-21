#include <allegro.h>
#include <stdio.h>
#include "Declarations.h"

int main(int argc, char *argv[]) 
{
    initGame(); //GameLoop.cpp
    
    Character character;
    characterInit(character);
    gameLoop(character); //GameLoop.cpp

    return 0;
}
END_OF_MAIN()

void characterInit(Character &character)
{
    character.body = load_bitmap("Images/Dwarf/dwarf.bmp", NULL);
    character.posX = (SCREEN_WIDTH / 2) - (character.body->w / 2);
    character.posY = SCREEN_HEIGHT - 70 - character.body->h;
    character.jumpAbility = 0;
    character.frame = 1;
}
