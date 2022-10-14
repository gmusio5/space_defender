#include <stdbool.h>

/* COLORS DEFINITION */
/************************/
#define COLOR_EMPTY 0
#define BG_RED 1
#define BG_WHITE 2
#define BG_YELLOW 3
#define BG_GREEN 4
#define BG_BLUE 5

#define FG_YELLOW 6
#define FG_RED 7
#define FG_GREEN 8
#define FG_WHITE 9
#define FG_BLUE 10

#define BG_DAMAGE_1 11
#define BG_DAMAGE_2 12
#define BG_DAMAGE_3 13
/************************/




/* LOGO PROPERTIES */
/********************************************************************************/
#define LOGO_ROWS 7
#define LOGO_DIM_LETTER 9
#define LOGO_DELAY 4000
#define NO_DELAY 0


#define NUM_LETTERS_SPACE_DEFENDER 14
#define SPACE_DEFENDER_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_SPACE_DEFENDER)

#define NUM_LETTERS_GAME_OVER 9
#define GAME_OVER_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_GAME_OVER)

#define NUM_LETTERS_VICTORY 7
#define VICTORY_OFFSET (LOGO_DIM_LETTER * NUM_LETTERS_VICTORY)
/********************************************************************************/

/* SCREEN PROPERTIES */
#define SCREEN_Y 45
#define SCREEN_X 165
#define SCREEN_OFFSET 3

#define SPACESHIP_ROWS 6
#define SPACESHIP_COLS 6

#define ENEMY_ROWS 3
#define ENEMY_COLS 3

typedef struct Coordinates{
    float x;
    float y;
} coordinates;

int maxX, maxY;


/* ENTITY GRAPHICS */
/*********************************************************************************************/
int graphicsSpaceShip[SPACESHIP_ROWS][SPACESHIP_COLS] = {
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY},
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, BG_DAMAGE_1, BG_WHITE, BG_YELLOW},
        {BG_DAMAGE_2, BG_WHITE, BG_WHITE, BG_WHITE, BG_DAMAGE_1, COLOR_EMPTY},
        {BG_RED, BG_WHITE, BG_WHITE, BG_WHITE, BG_WHITE, COLOR_EMPTY},
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, BG_WHITE, BG_DAMAGE_1, BG_YELLOW},
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY}
};

int graphicsSpaceShipDestroyed[SPACESHIP_ROWS][SPACESHIP_COLS] = {
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, BG_YELLOW},
        {BG_RED, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, BG_WHITE, COLOR_EMPTY},
        {COLOR_EMPTY, COLOR_EMPTY, BG_WHITE, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY},
        {COLOR_EMPTY, BG_WHITE, COLOR_EMPTY, BG_WHITE, BG_WHITE, COLOR_EMPTY},
        {COLOR_EMPTY, BG_WHITE, COLOR_EMPTY, BG_WHITE, COLOR_EMPTY, COLOR_EMPTY},
        {COLOR_EMPTY, BG_RED, COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY, BG_YELLOW}
};

int graphicsEnemyLvl1[ENEMY_ROWS][ENEMY_COLS] = {
        {COLOR_EMPTY, BG_RED, BG_WHITE},
        {BG_RED, BG_RED, COLOR_EMPTY},
        {COLOR_EMPTY, BG_RED, BG_WHITE}
};

int graphicsEnemyLvl2[ENEMY_ROWS][ENEMY_COLS] = {
        {BG_DAMAGE_3, COLOR_EMPTY, BG_RED},
        {COLOR_EMPTY, COLOR_EMPTY, COLOR_EMPTY},
        {BG_RED, COLOR_EMPTY, BG_RED}
};
/*********************************************************************************************/



/* LOGOS */
/********************************************************************************************************************************************/
char logoSpaceDefender[][SPACE_DEFENDER_OFFSET + 1] = {
        " _______  _______  _______  _______  _______           ______   _______  _______  _______  __    _  ______   _______  ______  ",
        "|       ||       ||   _   ||       ||       |         |      | |       ||       ||       ||  |  | ||      | |       ||   _  | ",
        "|  _____||    _  ||  |_|  ||    ___||    ___|         |  _    ||    ___||    ___||    ___||   |_| ||  _    ||    ___||  | | | ",
        "| |_____ |   |_| ||       ||   |    |   |___          | | |   ||   |___ |   |___ |   |___ |       || | |   ||   |___ |  |_| | ",
        "|_____  ||    ___||   _   ||   |    |    ___|         | |_|   ||    ___||    ___||    ___||  _    || |_|   ||    ___||  _   | ",
        " ____|  ||   |    |  | |  ||   |___ |   |___          |       ||   |___ |   |    |   |___ | | |   ||       ||   |___ | | |_  |",
        "|_______||___|    |__| |__||_______||_______|         |______| |_______||___|    |_______||_|  |__||______| |_______||_|   |_|"

};

char logoGameOver[][GAME_OVER_OFFSET + 1] = {
        " _______  _______  _     _  _______           _______  _     _  _______  ______  ",
        "|   ____||   _   || |   | ||       |         |       || |   | ||       ||   _  | ",
        "|  |     |  |_|  ||  |_|  ||    ___|         |   _   || |   | ||    ___||  | | | ",
        "|  | ____|       ||       ||   |___          |  | |  || |   | ||   |___ |  |_| | ",
        "|  | |_ ||   _   ||  ___  ||    ___|         |  |_|  ||  |_|  ||    ___||  _   | ",
        "|  |__| ||  | |  || |   | ||   |___          |       | |     | |   |___ | | |_  |",
        "|_______||__| |__||_|   |_||_______|         |_______|  |___|  |_______||_|   |_|"

};

char logoVictory[][VICTORY_OFFSET + 1] = {  
        " _     _  _______  _______  _______  _______  ______   __   __ ",
        "| |   | ||_     _||       ||       ||       ||   _  | |  | |  |",
        "| |   | |  |   |  |    ___||_     _||   _   ||  | | | |  |_|  |",
        "| |   | |  |   |  |   |      |   |  |  | |  ||  |_| |  |     | ",
        "|  |_|  |  |   |  |   |      |   |  |  |_|  ||  _   |   |   |  ",
        " |     |  _|   |_ |   |___   |   |  |       || | |_  |  |   |  ",
        "  |___|  |_______||_______|  |___|  |_______||_|   |_|  |__ |  " 
}; 
/********************************************************************************************************************************************/





/* ENTITY GRAPHICS MANIPULATION */
/****************************************************************************/
void printEntity(int cols, int matrix[][cols], coordinates );

void deleteEntity(int rows, int cols, coordinates );
/****************************************************************************/


/* Calculating enemy position upon chosen deployment */
coordinates createEnemyCoordinates(int enemyId){
    int i; 
    coordinates pos; 
 
    /* Arrow deployment */ 
    for (i = 0; i < M; i++) { 
        if (i == 0){ 
            pos.x = maxX - (ENEMY_COLS * ((M - 1) / 2)) - SCREEN_OFFSET - 10; 
            pos.y = maxY/2; 
        } 
        else if (i % 2 == 0)
            pos.y += (ENEMY_ROWS + 1) * i; 
        
        else if(i % 2 != 0){ 
            pos.y -= (ENEMY_ROWS+1) * i; 
            pos.x += ENEMY_COLS + 1; 
        } 

        if(i == enemyId)
            break;
    } 

 
    return pos;

}

/* Generating spaceship initial coordinates */
coordinates createSpaceshipCoordinates(){
    coordinates pos;

    pos.x = 10;
    pos.y = maxY / 2;

    return pos;
}




void printChar(coordinates pos, char entityType, int color);

void printLogo (int cols, char logo[][cols], int , int);



void printBorder(int delay);

bool checkCollision(coordinates , coordinates , int , int );

